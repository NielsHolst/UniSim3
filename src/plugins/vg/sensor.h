/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SENSOR_H
#define SENSOR_H
#include <base/box.h>

namespace vg {

class Sensor : public base::Box
{
public: 
    Sensor(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double
        indoorsTemperatureIn,
        indoorsRhIn,
        indoorsCo2In,
        indoorsWindspeedIn,
        outdoorsTemperatureIn,
        outdoorsRhIn,
        outdoorsCo2In,
        outdoorsGlobalRadiationIn,
        outdoorsWindSpeedIn,
        soilTemperatureIn,
        sinb;
    // Outputs
    double
        indoorsTemperature,
        indoorsRh,
        indoorsAh,
        indoorsCo2,
        indoorsWindspeed,
        outdoorsTemperature,
        outdoorsRh,
        outdoorsCo2,
        outdoorsGlobalRadiation,
        outdoorsWindSpeed,
        soilTemperature;
    // Data
    const double missing = -999;
    // Methods
    bool isMissing(double value) const;
    double estimateGlobalRadiation() const;
};

} //namespace
#endif
