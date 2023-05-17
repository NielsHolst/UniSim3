/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
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
        initTemperature, initRh;
    // Outputs
    double
        temperature, rh,
        heatInflux;
    // Data
    QVector<const BudgetLayer *> layers;
    // Friends
    friend class Budget;
};

}

#endif
