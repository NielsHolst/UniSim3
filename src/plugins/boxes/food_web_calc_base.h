/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef FOOD_WEB_CALC_BASE_H
#define FOOD_WEB_CALC_BASE_H
#include <base/box.h>
#include <base/matrix2D.h>

namespace boxes {

class FoodWebCalcBase : public base::Box
{
public: 
    FoodWebCalcBase(QString name, Box *parent);
    void amend();
    void reset();
    void update() final;
protected:
    // Inputs
    double timeStep;
    // Inputs (indirect)
    int _nAttackers, _nResources;
    QStringList _attackerNames, _resourceNames;
    base::Matrix2D<base::Box*> _foodWeb;
    base::Matrix2D<const double*> _a, _g;
    QVector<const double*> _X, _Y, _D;
    // Outputs (indirect)
    base::Matrix2D<double> _s, _S;
    QVector<double> _Stotal, _sdRatio;
    // Data
    QVector<Box*> _dependents;
    // Methods
    void collectFoodWeb();
    void collectInputs();
    QString pathToAttackers() const;
    QString pathToResources() const;
    void createOutputs();
    QString showMatrix(base::Matrix2D<const double*> m);
    QString showDensities();
    void computeSupplies();
    void pushOutputs();
    void writeParameters();
    QString na(int count) const;

    virtual void computeSearchRates() = 0;
    virtual void computeLosses() = 0;
    virtual QString attackerClass() const = 0;
    virtual QString resourceClass() const = 0;

    double X(int i) { return *(_X.at(i)); }
    double Y(int j) { return *(_Y.at(j)); }
    double D(int j) { return *(_D.at(j)); }
    double a(int i, int j) { return (_a.at(i,j))==nullptr ? 0. : *(_a.at(i,j)); }
    double g(int i, int j) { return (_g.at(i,j))==nullptr ? 0. : *(_g.at(i,j)); }
    double s(int i, int j) { return _s.at(i,j); }
    double S(int i, int j) { return _S.at(i,j); }

private:
    QStringList findNames(QVector<Box*> boxes);
};

} //namespace
#endif
