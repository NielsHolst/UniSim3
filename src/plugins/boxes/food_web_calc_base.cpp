    /* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QSet>
#include <QStringList>
#include <QTextStream>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "food_web_calc_base.h"

using namespace base;
using namespace TestNum;

namespace boxes {

FoodWebCalcBase::FoodWebCalcBase(QString name, Box *parent)
    : Box(name, parent), _foodWeb(this), _a(this) {
    Input(timeStep).imports("..[timeStep]");
}

inline QString aph(QString s) {
    return "\"" + s + "\"";
}

inline QString sharp(QString s) {
    return "<" + s + ">";
}

void FoodWebCalcBase::amend() {
    collectFoodWeb();
    collectInputs();
    createOutputs();
}

void FoodWebCalcBase::reset() {
    _s.fill(0.);
    _S.fill(0.);
    _Stotal.fill(0.);
    _sdRatio.fill(0.);
    writeParameters();
}

void FoodWebCalcBase::update() {
    computeSearchRates();
    computeSupplies();
    computeLosses();
    pushOutputs();
}

void FoodWebCalcBase::computeSupplies() {
    // For every attacker j, compute the supply obtained from each of its resources i by eq. 12.
    _Stotal.fill(0.);
    _sdRatio.fill(0.);
    for (int j=0; j<_nAttackers; ++j) {
        const double &D(FoodWebCalcBase::D(j));
        if (D>0.) {
            // Compute total supply: S_j
            double sum_gsX(0);
            for (int i=0; i<_nResources; ++i) {
                sum_gsX += g(i,j)*s(i,j)*X(i);
            }
            double S_j = D*(1. - exp(-sum_gsX/D));
            // Split supply among prey: _S(i,j)
            for (int i=0; i<_nResources; ++i) {
                _Stotal[j] += _S(i,j) = (sum_gsX > 0.) ? g(i,j)*s(i,j)*X(i)/sum_gsX*S_j : 0.;
            }
            _sdRatio[j] = _Stotal.at(j)/D;
        }
        else {
            for (int i=0; i<_nResources; ++i) {
                _S(i,j) = 0.;
            }
        }
    }
}

void FoodWebCalcBase::pushOutputs() {
    for (Box *box : _dependents)
        box->evaluatePorts();
}

void FoodWebCalcBase::collectFoodWeb() {
    QVector<Box*> attackers = findMany<Box*>(pathToAttackers()),
                  resources = findMany<Box*>(pathToResources());
    _attackerNames = findNames(attackers);
    _resourceNames = findNames(resources);
    _nAttackers = _attackerNames.size();
    _nResources = _resourceNames.size();
    _foodWeb.resize(_nResources, _nAttackers);
    for (int j=0; j<_nAttackers; ++j) {
        for (int i=0; i<_nResources; ++i) {
            QString path = "../" + _attackerNames.at(j) + sharp(attackerClass()) + "/"
                                 + _resourceNames.at(i) + sharp(resourceClass());
            _foodWeb(i,j) = findMaybeOne<Box*>(path);
        }
    }
    _dependents = attackers;
    _dependents.append(resources);
}

QStringList FoodWebCalcBase::findNames(QVector<Box*> boxes) {
    QSet<QString> names;
    for (Box *box : boxes)
        names << box->name();
    #if QT_VERSION >= 0x050E00
        QStringList sorted(names.begin(), names.end());
    #else
        QStringList sorted(names.toList());
    #endif
    sorted.sort();
    return sorted;
}

void FoodWebCalcBase::collectInputs() {
    // Allocate input buffers
    _a.resize(_nResources, _nAttackers);
    _g.resize(_nResources, _nAttackers);
    _X.resize(_nResources);
    _Y.resize(_nAttackers);
    _D.resize(_nAttackers);

    // Collect a and g matrices
    for (int i=0; i<_nResources; ++i)
        for (int j=0; j<_nAttackers; ++j) {
            Box *box = _foodWeb.at(i,j);
            _a(i,j) = box ? box->port("attackRate")->valuePtr<double>() : nullptr;
            _g(i,j) = box ? box->port("gainFactor")->valuePtr<double>() : nullptr;
        }
    // Collect resource vector
    for (int i=0; i<_nResources; ++i) {
        QString resPath = "../*" + sharp(attackerClass()) + "/" + _resourceNames.at(i) + sharp(resourceClass());
        QVector<Box*> res = findMany<Box*>(resPath);
        if (res.isEmpty())
            ThrowException("Found no resource on this path").value(resPath).context(this);

        // Check that prey density paths are identical
        QSet<QString> densityPaths;
        for (Box *anotherRes : res) {
            QString path = anotherRes->port("density")->importPath();
            if (!path.isEmpty())
                densityPaths << path;
        }
        if (densityPaths.size() > 1) {
            ThrowException("Resources with same name must refer to the same source density").
                    value(_resourceNames.at(i)).
                    value2(QStringList(densityPaths.values()).join(",")).
                    context(this);
        }
        // Assign _X[i] to the first resource found on path
        // Note: All prey on the path are identical; the first one found is used
        _X[i] = res[0]->port("density")->valuePtr<double>();
    }
    // Collect attacker and demand vectors
    for (int j=0; j<_nAttackers; ++j) {
        QString attPath = "../" + _attackerNames.at(j) + sharp(attackerClass());
        QVector<Box*> att = findMany<Box*>(attPath);
        if (att.isEmpty()) {
            ThrowException("Found no attacker on this path").value(attPath).context(this);
        }
        else if (att.size() > 1) {
            ThrowException("Attackers must have unique names with FoodWeb").
                    value(_attackerNames.at(j)).
                    value2(att.size()).
                    context(this);
        }
        // Assign _Y[j] and _D[j] to the single attacker found on path
        _Y[j] = att[0]->port("density")->valuePtr<double>();
        _D[j] = att[0]->port("demand")->valuePtr<double>();
    }
}

QString FoodWebCalcBase::pathToAttackers() const {
    return "../*" + sharp(attackerClass());
}

QString FoodWebCalcBase::pathToResources() const {
    return pathToAttackers() + "/*" + sharp(resourceClass());
}

void FoodWebCalcBase::createOutputs() {
    // Allocate output buffers
    _s.resize(_nResources, _nAttackers);
    _S.resize(_nResources, _nAttackers);
    _Stotal.resize(_nAttackers);
    _sdRatio.resize(_nAttackers);

    // Create attacker-resource output ports
    for (int i=0; i<_nResources; ++i) {
        for (int j=0; j<_nAttackers; ++j) {
            QString suffix = "_" + _attackerNames.at(j) + "_" + _resourceNames.at(i);
            NamedOutput("s" + suffix, _s(i,j));
            NamedOutput("supply" + suffix, _S(i,j));
        }
    }

    // Create attacker output ports
    for (int j=0; j<_nAttackers; ++j) {
        QString suffix = "_" + _attackerNames.at(j);
        NamedOutput("supply" + suffix, _Stotal[j]);
        NamedOutput("sd_ratio" + suffix, _sdRatio[j]);    }

}

QString FoodWebCalcBase::showMatrix(base::Matrix2D<const double*> m) {
    QStringList rows, rowNames;
    for (int j=0; j<_nAttackers; ++j) {
        QStringList row;
        for (int i=0; i<_nResources; ++i) {
            double value = m.at(i,j) ? *m.at(i,j) : 0.;
            row << QString::number(value);
        }
        QString s = _attackerNames.at(j) + " = c(" + row.join(",") + ")";
        rows << s;
    }
    for (int i=0; i<_nResources; ++i)
        rowNames << aph(_resourceNames.at(i));

    QString s = QString() +
        "M = data.frame (\n" +
        rows.join(",\n") +
        "\n)\n" +
        "rownames(M) = " +
        "c(" + rowNames.join(",") + ")\n" +
        "M\n";
    return s;
}

QString FoodWebCalcBase::showDensities() {
    QStringList line;
    for (int j=0; j<_nAttackers; ++j) {
        line << (_attackerNames.at(j) + " = " + QString::number(*_Y.at(j)));
    }
    QString y = QString() +
        "Y = data.frame (\n" +
        line.join(",\n") +
        "\n)\n" +
        "Y\n";

    line.clear();
    for (int i=0; i<_nResources; ++i) {
        line << QString::number(*_X.at(i));
    }
    QString x = line.join("\n");


    return y+x;
}

inline QString sep(int i, int n) {
    return (i < n-1) ? "\t" : "\tNA\n" ;
}

void FoodWebCalcBase::writeParameters() {
    QDir dir = environment().resolveDir(Environment::Output);
    QString fileNamePath = dir.absoluteFilePath("food_web_parameters.txt");
    QFile file(fileNamePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        ThrowException("Cannot open output file").value(fileNamePath).context(this);

    QTextStream stream;
    stream.setDevice(&file);
    stream << "Resource\tParameter\t" << _attackerNames.join("\t") << "\tValue\n";
    stream << "NA\tY\t";
    for (int j=0; j<_nAttackers; ++j)
        stream <<Y(j) << sep(j, _nAttackers);
    stream << "NA\tD\t";
    for (int j=0; j<_nAttackers; ++j)
        stream << D(j) << sep(j, _nAttackers);
    for (int i=0; i<_nResources; ++i) {
        stream << _resourceNames.at(i) << "\ta\t";
        for (int j=0; j<_nAttackers; ++j)
            stream << a(i,j) << sep(j, _nAttackers);
    }
    for (int i=0; i<_nResources; ++i) {
        stream << _resourceNames.at(i) << "\tg\t";
        for (int j=0; j<_nAttackers; ++j)
            stream << g(i,j) << sep(j, _nAttackers);
    }
    for (int i=0; i<_nResources; ++i) {
        stream << _resourceNames.at(i) << "\tX\t" << na(_nAttackers) <<  X(i) << "\n";
    }
    stream << "NA\tdt\t" << na(_nAttackers) << timeStep << "\n";
}

QString FoodWebCalcBase::na(int count) const {
    QVector<QString> v;
    v.fill("NA", count);
    #if QT_VERSION >= 0x050E00
        QStringList list(v.begin(), v.end());
    #else
        QStringList list(v.toList());
    #endif
    return list.join("\t") + "\t";
}

} //namespace

