/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Isaac Kwesi Abuley , Aarhus University [ikabuley at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
//definining headers
#ifndef PTMAX_H
#define PTMAX_H
#include <base/box.h>

namespace potato {

class PTmax: public base::Box
{
public:
    PTmax(QString name, QObject *parent);
    void reset();
    void update();
private:
    //inputs
    double Tmax, Topt, Tfloor, Tceiling;
    // Outputs //Physiologic development for minimum temperature
    double PTmax;//PTmin is the name of the output of the Physologic Development with the Temperature mini in a day
    // Data

};

}

#endif
