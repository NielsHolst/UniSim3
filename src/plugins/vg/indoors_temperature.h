/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef INDOORS_TEMPERATURE_H
#define INDOORS_TEMPERATURE_H
#include <base/box.h>

namespace vg {

class IndoorsTemperature : public base::Box
{
    friend class EnergyBudget;
public:
    IndoorsTemperature(QString name, Box *parent);
    void reset() final;
    void update() final;
    double getTemperature() const;
    void setTemperature(double newValue);
protected:
    // Inputs
    double initTemperature, convectiveInflux,
        airInflux, outdoorsTemperature,
        timeStep, groundArea, volume;
    bool keepConstant, scanTemperatures;
    // Outputs
    double value, advectiveEnergyFlux;
    // Data
    int _tick;
private:
    // Methods
    void updateByAdvection(double influxVolume, double influxTemperature);
    double scannedTemperature() const;
};

} //namespace
#endif
