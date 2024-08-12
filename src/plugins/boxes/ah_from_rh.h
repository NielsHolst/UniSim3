/* Copyright 20052RH_4 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef AH_FROM_RH_RH_H
#define AH_FROM_RH_H
#include <base/box.h>

namespace boxes {

class AhFromRh : public base::Box
{
public: 
    AhFromRh(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double rh, temperature;
    // Outputs
    double value;
};

} //namespace
#endif
