/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef DEW_POINT_H
#define DEW_POINT_H
#include <base/box.h>

namespace boxes {

class DewPoint : public base::Box
{
public: 
    DewPoint(QString name, Box *parent);
    // standard methods
    void reset();
    void update();
private:
    // Input
    double temperature, rh;
    // Output
    double value;
};

} //namespace
#endif
