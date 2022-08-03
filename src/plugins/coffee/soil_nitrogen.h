/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SOIL_NITROGEN_H
#define SOIL_NITROGEN_H
#include <base/box.h>

namespace coffee {

class SoilNitrogen : public base::Box
{
public: 
    SoilNitrogen(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double initial, fertilisation, mineralisation, uptake, leaching, emission;
    // Outputs
    double value;
};

} //namespace
#endif
