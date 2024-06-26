/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <base/box.h>

namespace vg {

class Geometry : public base::Box
{
public:
    Geometry(QString name, Box *parent);
    void initialize();
    void reset();
private:
    // Input
    double orientation, spanWidth, length, height, roofPitch, reflection;
    int numSpans;
    // Output
    double groundArea,
        roofArea, sideArea, endArea,
        coverArea, coverPerGroundArea,
        volume, averageHeight;
};

} //namespace


#endif
