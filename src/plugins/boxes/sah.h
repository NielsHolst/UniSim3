/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef SAH_H
#define SAH_H
#include <base/box.h>

namespace boxes {

class Sah : public base::Box
{
public: 
    Sah(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double temperature;
    // Outputs
    double value;
};

} //namespace
#endif
