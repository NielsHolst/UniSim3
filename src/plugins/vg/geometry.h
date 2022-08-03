/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <base/box.h>

namespace vg {

class Geometry : public base::Box
{
public:
    Geometry(QString name, Box *parent);
    void reset();
private:
    // Input
    double orientation, spanWidth, length, height, roofPitch, reflection;
    int numSpans;
    // Output
    double width, groundArea,
        roofHeight, roofArea, sideWallsArea, endWallsArea, gablesArea,
        coverArea, coverPerGroundArea,
        volume, averageHeight;
};

} //namespace


#endif
