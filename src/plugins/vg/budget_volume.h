/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef BUDGET_VOLUME_H
#define BUDGET_VOLUME_H
#include <base/box.h>

namespace vg {

class BudgetLayer;

class BudgetVolume : public base::Box {
public:
    BudgetVolume(QString name, base::Box *parent);
    void reset() final;
    void addLayer(const BudgetLayer *layer);
    void addHeatInflux(double flux);
private:
    // Inputs
    double
        initTemperature, initRh, initCo2;
    // Outputs
    double
        temperature, rh, co2,
        heatInflux;
    // Data
    QVector<const BudgetLayer *> layers;
    double rollBackTemperature, rollBackRh;
    // Friends
    friend class Budget;
};

}

#endif
