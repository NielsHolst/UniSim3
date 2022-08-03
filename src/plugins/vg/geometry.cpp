/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/phys_math.h>
#include <base/publish.h>
#include <base/vector_op.h>
#include "geometry.h"

using namespace base;
using phys_math::PI;

namespace vg {
	
PUBLISH(Geometry)

Geometry::Geometry(QString name, Box *parent)
	: Box(name, parent)
{
    help("defines the greenhouse geometry");
    Input(orientation).equals(90.).help("Compass direction of greenhouse long axis").unit("[0;360]");
    Input(numSpans).equals(1).help("Number of spans").unit("-");
    Input(spanWidth).equals(40.).help("Width of a span").unit("m");
    Input(length).equals(100.).help("Length of side wall").unit("m");
    Input(height).equals(2.5).help("Wall height").unit("m");
    Input(roofPitch).equals(26.).help("Pitch (slope) of roof").unit("[0;180]");
    Input(reflection).equals(0.1).help("Outer reflection of greenhouse construction (excl. cover").unit("[0;1]");

    Output(width).help("Total width of greenhouse across spans").unit("m");
    Output(groundArea).help("Total area covered by greenhouse").unit("m2");
    Output(roofArea).help("Total area of greenhouse roof").unit("m2");
    Output(sideWallsArea).help("Total area of side walls").unit("m2");
    Output(endWallsArea).help("Total area of end walls").unit("m2");
    Output(gablesArea).help("Total area of gables").unit("m2");
    Output(coverArea).help("Total area of greenhouse cover").unit("m2");
    Output(coverPerGroundArea).help("Area to ground cover ratio").unit("m2/m2");
    Output(volume).help("Total greenhouse volume").unit("m3");
    Output(averageHeight).help("Average height of total volume").unit("m");
    Output(roofHeight).help("Height of roof above wall height").unit("m");
}

void Geometry::reset() {
    roofHeight = sin(roofPitch*PI/180.)*spanWidth/2.;
    double roofWidth = hypot(roofHeight, spanWidth/2.);
    width = numSpans*spanWidth;
    groundArea = width*length;
    roofArea = 2*numSpans*roofWidth*length;
    sideWallsArea = 2*length*height;
    endWallsArea = 2*width*height;
    gablesArea = numSpans*roofHeight*spanWidth;
    coverArea = sideWallsArea + endWallsArea + gablesArea + roofArea;
    coverPerGroundArea = coverArea/groundArea;
    double roofVolume  = length*gablesArea/2.;
    volume = groundArea*height + roofVolume;
    averageHeight = volume/groundArea;
}

} //namespace

