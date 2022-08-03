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
#include "food_web.h"

using namespace base;
using namespace TestNum;

namespace boxes {

PUBLISH(FoodWeb)

FoodWeb::FoodWeb(QString name, Box *parent)
    : Box(name, parent), _foodWeb(this), _a(this) {
    help("resolves food web acquisitions among Predator and Parasite objects");
    Input(timeStep).equals(1.).help("Simulation time step");
    Input(report).equals(true).help("Show food web report?");
}

void FoodWeb::amend() {
    collectFoodWeb();
    collectInputs();
    createOutputs();
}

void FoodWeb::reset() {
    _s.fill(0.);
    _S.fill(0.);
    _Stotal.fill(0.);
    _sdRatio.fill(0.);
    _Xloss.fill(0.);
    _XlossTotal.fill(0.);
    _mortality.fill(0.);
    writeParameters();
}

void FoodWeb::update() {
    computeOutputs();
    pushOutputs();
}

inline void info(QString s, int i, double x) {
    QString msg = s + +"[" + QString::number(i) +"] = " + QString::number(x);
//    dialog().information(msg);
}

void FoodWeb::computeOutputs() {
    _s.fill(0.);
    // (1) For every resource i, compute the search rates s_ij for all it attackers j by by eq. 8.
    for (int i=0; i<_nPrey; ++i) {
        // Compute total search rate: si_
        double sum_aY(0);
        for (int j=0; j<_nPredators; ++j)
            sum_aY += a(i,j)*Y(j);
        double si_ = 1. - exp(-sum_aY*timeStep);
        info("si_", i, si_);

        // Split search rate among predators: _s(i,j)
        for (int j=0; j<_nPredators; ++j) {
            _s(i,j) = (sum_aY > 0.) ? a(i,j)*Y(j)/sum_aY*si_ : 0.;
            info("_s", j, _s(i,j));
        }
    }
    // (2) For every attacker j, compute the supply obtained from each of its resources i by eq. 12.
    _Stotal.fill(0.);
    _sdRatio.fill(0.);
    for (int j=0; j<_nPredators; ++j) {
        const double &D(FoodWeb::D(j));
        if (D>0.) {
            // Compute total supply: S_j
            double sum_gsX(0);
            for (int i=0; i<_nPrey; ++i)
                sum_gsX += g(i,j)*s(i,j)*X(i);
            double S_j = D*(1. - exp(-sum_gsX/D));
            info("S_j", j, S_j);

            // Split supply among prey: _S(i,j)
            for (int i=0; i<_nPrey; ++i) {
                _Stotal[j] += _S(i,j) = (sum_gsX > 0.) ? g(i,j)*s(i,j)*X(i)/sum_gsX*S_j : 0.;
                info("_S", j, _S(i,j));
            }
            _sdRatio[j] = _Stotal.at(j)/D;
        }
        else {
            for (int i=0; i<_nPrey; ++i) {
                _S(i,j) = 0.;
            }
        }
    }
    // (3) For every attacker j, compute the loss incured  on each of its resources i by eq. 13.
    _XlossTotal.fill(0);
    _mortality.fill(0.);
    for (int j=0; j<_nPredators; ++j) {
        info("Predator", j, 0.);
        for (int i=0; i<_nPrey; ++i) {
            _XlossTotal[i] += _Xloss(i,j) = (g(i,j) > 0.) ? S(i,j)/g(i,j) : 0.;
            info("_Xloss", i, _Xloss(i,j));
        }
    }
    // (4) Compute total mortality on each ressource
    for (int i=0; i<_nPrey; ++i) {
        // Use snapTo for numerical inaccuracy at small numbers
        snapTo(_XlossTotal[i], X(i));
        double mortality = (X(i) > 0.) ? _XlossTotal[i]/X(i) : 0.;
        snapTo(mortality, .1);
        _mortality[i] =  mortality;
        info("_XlossTotal", i, _XlossTotal[i]);
        info("_mortality", i, _mortality[i]);
     }
}

void FoodWeb::pushOutputs() {
    for (Box *box : _descendents)
        box->updateImports();
}

inline QStringList setToList(QSet<QString> set) {
    QStringList list = QStringList(set.values());
    list.sort();
    return list;
}

void FoodWeb::collectFoodWeb() {
    QVector<Box*> predators = findMany<Box*>("./*<Predator>"),
                  prey = findMany<Box*>("./*<Predator>/*<Prey>"),
                  parasites = findMany<Box*>("./*<Parasite>"),
                  hosts = findMany<Box*>("./*<Parasite>/*<Host>");
    QSet<QString> predatorNamesUnique, preyNamesUnique,
                  parasiteNamesUnique, hostNamesUnique;
    for (Box *box : predators)
        predatorNamesUnique << box->name();
    for (Box *box : prey)
        preyNamesUnique << box->name();
    for (Box *box : parasites)
        parasiteNamesUnique << box->name();
    for (Box *box : hosts)
        hostNamesUnique << box->name();

    _predatorNames = setToList(predatorNamesUnique);
    _preyNames = setToList(preyNamesUnique);
    _parasiteNames = setToList(parasiteNamesUnique);
    _hostNames = setToList(hostNamesUnique);
    _nPredators = _predatorNames.size();
    _nPrey = _preyNames.size();
    _nParasites = _parasiteNames.size();
    _nHosts = _hostNames.size();

    if (report) {
        QString s = "Found %1 predators:\n  %2\nand %3 prey:\n  %4\n",
                t = "Found %1 parasites:\n  %2\nand %3 hosts:\n  %4\n";
        dialog().information(s.arg(_nPredators).arg(_predatorNames.join("\n  ")).
                               arg(_nPrey     ).arg(    _preyNames.join("\n  ")));
        dialog().information(t.arg(_nParasites).arg(_parasiteNames.join("\n  ")).
                               arg(_nHosts    ).arg(    _hostNames.join("\n  ")));
    }
    _foodWeb.resize(_nPrey, _nPredators);
    for (int j=0; j<_nPredators; ++j) {
        if (report) {
            QString s = "%1 attacks";
            dialog().information(s.arg(_predatorNames.at(j)));
        }
        for (int i=0; i<_nPrey; ++i) {
            QString path = "./" + _predatorNames.at(j) + "<Predator>/"
                                + _preyNames.at(i) + "<Prey>";
            _foodWeb(i,j) = findMaybeOne<Box*>(path);
            if (report && _foodWeb(i,j)) {
                QString s = "  %1";
                dialog().information(s.arg(_preyNames.at(i)));
            }
        }
    }

    _descendents = predators;
    _descendents.append(prey);
}

void FoodWeb::collectInputs() {
    // Allocate input buffers
    _a.resize(_nPrey, _nPredators);
    _g.resize(_nPrey, _nPredators);
    _X.resize(_nPrey);
    _Y.resize(_nPredators);
    _D.resize(_nPredators);

    // Collect a and g matrices
    for (int i=0; i<_nPrey; ++i)
        for (int j=0; j<_nPredators; ++j) {
            Box *box = _foodWeb.at(i,j);
            _a(i,j) = box ? box->port("attackRate")->valuePtr<double>() : nullptr;
            _g(i,j) = box ? box->port("gainFactor")->valuePtr<double>() : nullptr;
        }
    // Collect prey vector
    for (int i=0; i<_nPrey; ++i) {
        QString preyPath = "./*<Predator>/" + _preyNames.at(i) + "<Prey>";
        QVector<Box*> prey = findMany<Box*>(preyPath);
        if (prey.isEmpty())
            ThrowException("Found no prey on this path").value(preyPath).context(this);

        // Check that prey density paths are identical
        QSet<QString> densityPaths;
        for (Box *anotherPrey : prey) {
            QString path = anotherPrey->port("density")->importPath();
            if (!path.isEmpty())
                densityPaths << path;
        }
        if (densityPaths.size() > 1) {
            ThrowException("Prey with same name must refer to the same source density").
                    value(_preyNames.at(i)).
                    value2(QStringList(densityPaths.values()).join(",")).
                    context(this);
        }
        // Assign _X[i] to the first prey found on path
        // Note: All prey on the path are identical; the first one found is used
        _X[i] = prey[0]->port("density")->valuePtr<double>();
    }
    // Collect predator and demand vectors
    for (int j=0; j<_nPredators; ++j) {
        QString predatorPath = "./" + _predatorNames.at(j) + "<Predator>";
        QVector<Box*> predators = findMany<Box*>(predatorPath);
        if (predators.isEmpty()) {
            ThrowException("Found no predator on this path").value(predatorPath).context(this);
        }
        else if (predators.size() > 1) {
            ThrowException("Predators must have unique names with FoodWeb").
                    value(_predatorNames.at(j)).
                    value2(predators.size()).
                    context(this);
        }
        // Assign _Y[j] and _D[j] to the single predator found on path
        _Y[j] = predators[0]->port("density")->valuePtr<double>();
        _D[j] = predators[0]->port("demand")->valuePtr<double>();
    }
}

void FoodWeb::createOutputs() {
    // Allocate output buffers
    _s.resize(_nPrey, _nPredators);
    _S.resize(_nPrey, _nPredators);
    _Xloss.resize(_nPrey, _nPredators);
    _XlossTotal.resize(_nPrey);
    _mortality.resize(_nPrey);
    _Stotal.resize(_nPredators);
    _sdRatio.resize(_nPredators);

    // Create predator-prey output ports
    for (int i=0; i<_nPrey; ++i) {
        for (int j=0; j<_nPredators; ++j) {
            QString suffix = "_" + _predatorNames.at(j) + "_" + _preyNames.at(i);
            NamedOutput("s" + suffix, _s(i,j));
            NamedOutput("supply" + suffix, _S(i,j));
            NamedOutput("loss" + suffix, _Xloss(i,j));
        }
    }

    // Create predator output ports
    for (int j=0; j<_nPredators; ++j) {
        QString suffix = "_" + _predatorNames.at(j);
        NamedOutput("supply" + suffix, _Stotal[j]);
        NamedOutput("sdRatio" + suffix, _sdRatio[j]);    }

    // Create prey output ports
    for (int i=0; i<_nPrey; ++i) {
        QString suffix = "_" + _preyNames.at(i);
        NamedOutput("loss" + suffix, _XlossTotal[i]);
        NamedOutput("mortality" + suffix, _mortality[i]);
    }
}

inline QString aph(QString s) {
    return "\"" + s + "\"";
}

QString FoodWeb::showMatrix(base::Matrix2D<const double*> m) {
    QStringList rows, rowNames;
    for (int j=0; j<_nPredators; ++j) {
        QStringList row;
        for (int i=0; i<_nPrey; ++i) {
            double value = m.at(i,j) ? *m.at(i,j) : 0.;
            row << QString::number(value);
        }
        QString s = _predatorNames.at(j) + " = c(" + row.join(",") + ")";
        rows << s;
    }
    for (int i=0; i<_nPrey; ++i)
        rowNames << aph(_preyNames.at(i));

    QString s = QString() +
        "M = data.frame (\n" +
        rows.join(",\n") +
        "\n)\n" +
        "rownames(M) = " +
        "c(" + rowNames.join(",") + ")\n" +
        "M\n";
    return s;
}

QString FoodWeb::showDensities() {
    QStringList line;
    for (int j=0; j<_nPredators; ++j) {
        line << (_predatorNames.at(j) + " = " + QString::number(*_Y.at(j)));
    }
    QString y = QString() +
        "Y = data.frame (\n" +
        line.join(",\n") +
        "\n)\n" +
        "Y\n";

    line.clear();
    for (int i=0; i<_nPrey; ++i) {
        line << QString::number(*_X.at(i));
    }
    QString x = line.join("\n");


    return y+x;
}

inline QString sep(int i, int n) {
    return (i < n-1) ? "\t" : "\tNA\n" ;
}

void FoodWeb::writeParameters() {
    QDir dir = environment().resolveDir(Environment::Output);
    QString fileNamePath = dir.absoluteFilePath("food_web_parameters.txt");
    QFile file(fileNamePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        ThrowException("Cannot open output file").value(fileNamePath).context(this);

    QTextStream stream;
    stream.setDevice(&file);
    stream << "Prey\tParameter\t" << _predatorNames.join("\t") << "\tValue\n";
    stream << "NA\tY\t";
    for (int j=0; j<_nPredators; ++j)
        stream <<Y(j) << sep(j, _nPredators);
    stream << "NA\tD\t";
    for (int j=0; j<_nPredators; ++j)
        stream << D(j) << sep(j, _nPredators);
    for (int i=0; i<_nPrey; ++i) {
        stream << _preyNames.at(i) << "\ta\t";
        for (int j=0; j<_nPredators; ++j)
            stream << a(i,j) << sep(j, _nPredators);
    }
    for (int i=0; i<_nPrey; ++i) {
        stream << _preyNames.at(i) << "\tg\t";
        for (int j=0; j<_nPredators; ++j)
            stream << g(i,j) << sep(j, _nPredators);
    }
    QStringList naList;
    naList.fill("NA", _nPredators);
    QString na = naList.join("\t") + "\t";
    for (int i=0; i<_nPrey; ++i) {
        stream << _preyNames.at(i) << "\tX\t" << na <<  X(i) << "\n";
    }
    stream << "NA\tdt\t" << na << timeStep << "\n";
}

} //namespace

