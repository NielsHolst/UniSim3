/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef ENERGY_BUDGET_2_H
#define ENERGY_BUDGET_2_H
#include <ostream>
#include <QString>
#include <QVector>


namespace vg {

struct Radiation {
    QString name;
    const double *a , *r , *t ,
                 *a_, *r_, *t_;
    double E, E_, F, F_, A, A_;
    double netRadiation() const {
        return A + A_ - E - E_;
    }
    static QString columnLabels();
};
std::ostream& operator<<(std::ostream& os, const Radiation& x);

struct RadiationBudget {
    void initiateRadiation();
    void distributeRadDown();
    void distributeRadUp();
    void distributeRadiation();
    QVector<double> A() const;
    QVector<double> A_() const;
    QVector<Radiation> layers;
    int iterations;
};
std::ostream& operator<<(std::ostream& os, const RadiationBudget& x);

struct Heat {
    double *T;
    const double *C;
    static QString columnLabels();
};
std::ostream& operator<<(std::ostream& os, const Heat& x);

struct RadiationAndHeat {
    Radiation radiation;
    Heat heat;
    static QString columnLabels();
};
std::ostream& operator<<(std::ostream& os, const RadiationAndHeat& x);

struct RadiationAndHeatBudget {
    void updateLwEmission();
    //    void updateTemperatureByRadiation(const Layers &sw, double dt);
    QVector<RadiationAndHeat> layers;
};
std::ostream& operator<<(std::ostream& os, const RadiationAndHeatBudget& x);

struct Budget {
//    void updateInMinorSteps(double dt, int n);
    RadiationBudget sw, par;
    RadiationAndHeatBudget radAndheat;
    void transferSwAbsorption();
};



} //namespace

#endif
