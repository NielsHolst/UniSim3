/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Isaac Kwesi Abuley , Aarhus University [ikabuley at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
//definining headers
#ifndef POTATO_DEVELOPMENT_AT_TEMP_H
#define POTATO_DEVELOPMENT_AT_TEMP_H
#include <base/box.h>

namespace potato {

class PotatoDevelopmentAtTemp : public base::Box
{
public:
    PotatoDevelopmentAtTemp(QString name, QObject *parent);
    void reset();
    void update();
private:
    //inputs
    double temp, Topt, Tfloor, Tceiling;
    // outputs
    double step;
};

}

#endif
