/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef FOOD_WEB_H
#define FOOD_WEB_H
#include <base/box.h>
#include <base/matrix2D.h>

namespace boxes {

class FoodWeb : public base::Box
{
public: 
    FoodWeb(QString name, Box *parent);
    void amend();
    void reset();
    void update();
private:
    // Inputs
    double timeStep;
    bool report;

    // Inputs (indirect)
    int _nPredators, _nPrey, _nParasites, _nHosts;
    QStringList _predatorNames, _preyNames, _parasiteNames, _hostNames;
    base::Matrix2D<base::Box*> _foodWeb;
    base::Matrix2D<const double*> _a, _g;
    QVector<const double*> _X, _Y, _D;
    // Outputs (indirect)
    base::Matrix2D<double> _s, _S, _Xloss;
    QVector<double> _Stotal, _sdRatio, _XlossTotal, _mortality;
    // Data
    QVector<Box*> _descendents;
    // Methods
    void collectFoodWeb();
    void collectInputs();
    void createOutputs();
    QString showMatrix(base::Matrix2D<const double*> m);
    QString showDensities();
    void computeOutputs();
    void pushOutputs();
    void writeParameters();

    double X(int i) { return *(_X.at(i)); }
    double Y(int j) { return *(_Y.at(j)); }
    double D(int j) { return *(_D.at(j)); }
    double a(int i, int j) { return (_a.at(i,j))==nullptr ? 0. : *(_a.at(i,j)); }
    double g(int i, int j) { return (_g.at(i,j))==nullptr ? 0. : *(_g.at(i,j)); }
    double s(int i, int j) { return _s.at(i,j); }
    double S(int i, int j) { return _S.at(i,j); }
};

} //namespace
#endif
