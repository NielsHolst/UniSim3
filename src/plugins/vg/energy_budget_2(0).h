/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef ENERGY_BUDGET_2_H
#define ENERGY_BUDGET_2_H
#include <ostream>
#include <QString>
#include <QVector>
#include <base/box.h>


namespace vg {

class EnergyBudget2: public base::Box
{
public:

    struct Radiation {
        QString name;
        const double *a , *r , *t ,
                     *a_, *r_, *t_;
        double E, E_, F, F_, A, A_;
        double netRadiation() const {
            return A + A_ - E - E_;
        }
//        static QString columnLabels();
    };

//    std::ostream& operator<<(std::ostream& os, const Radiation& x);

    struct Layer {
        Radiation sw, lw, par;
        double T, C;
    };


    class Layers {
    public:
        Layers() {}
        void distributeRadDown();
        void distributeRadUp();
        void distributeRadiation();
    //    void updateLwEmission();
    //    void updateTemperatureByRadiation(const Layers &sw, double dt);
    //    void updateInMinorSteps(double dt, int n);
    private:
        QVector<Radiation> layers;
        int iterations;
    };

    EnergyBudget2(QString name, Box *parent);
};


} //namespace

#endif
