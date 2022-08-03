/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef WATER_VISCOSITY_H
#define WATER_VISCOSITY_H

#include <base/box.h>

namespace PestTox {

class WaterViscosity : public base::Box
{
public:
    WaterViscosity(QString name, Box *parent);
    void reset();
    void update();

private:
    // Input
    double temperature;

    // Output
    double value;

};

} //namespace


#endif
