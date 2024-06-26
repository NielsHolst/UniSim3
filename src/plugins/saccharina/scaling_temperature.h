/* Copyright 2019-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Teis Boderskov, Aarhus University [tebo atbios.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef SCALING_TEMPERATURE_H
#define SCALING_TEMPERATURE_H
#include <base/box.h>

namespace saccharina {

class ScalingTemperature : public base::Box
{
public:
    ScalingTemperature(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double T, Tmin, Topt1, Topt2, Tmax;
    // Outputs
    double value;
};

}

#endif
