/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SCREEN_WALL_H
#define SCREEN_WALL_H
#include "screen.h"

namespace vg {

class ScreenWall : public Screen
{
public:
    ScreenWall(QString name, Box *parent);
private:
    // Input
    double faceArea;
    // Methods
    double computeMaxArea();
};

} //namespace


#endif
