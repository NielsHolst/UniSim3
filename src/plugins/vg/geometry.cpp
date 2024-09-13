/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
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
    Input(orientation).equals(90.).help("Orientation of greenhouse").unit("[0;180]");
    Input(numSpans).equals(1).help("Number of spans");
    Input(spanWidth).equals(40.).help("Width of a span").unit("m");
    Input(length).equals(100.).help("Length of side wall").unit("m");
    Input(height).equals(2.5).help("Wall height").unit("m");
    Input(roofPitch).equals(26.).help("Pitch (slope) of roof").unit("[0;180]");

    Output(sideArea).help("Total area of side walls").unit("m2");
    Output(endArea).help("Total area of end face").unit("m2");
    Output(roofArea).help("Total area of greenhouse roof").unit("m2");
    Output(coverArea).help("Total area of greenhouse cover").unit("m2");
    Output(groundArea).help("Total area covered by greenhouse").unit("m2");
    Output(volume).help("Total greenhouse volume").unit("m3");
    Output(coverPerGroundArea).help("Area to ground cover ratio").unit("m2/m2");
    Output(averageHeight).help("Average height of total volume").unit("m");
}

void Geometry::initialize() {
    reset();
}

void Geometry::reset() {
    // Compute all outputs
    double sinPitch    = sin(roofPitch*PI/180.),
           oneSideArea = length*height,
           oneEndArea  = spanWidth*height + spanWidth*spanWidth/4*sinPitch,
           oneRoofArea = spanWidth*length/2*sqrt(1+sinPitch*sinPitch);
    sideArea = 2*oneSideArea;
    endArea  = 2*oneEndArea*numSpans;
    roofArea = 2*oneRoofArea*numSpans;
    coverArea = sideArea + endArea + roofArea;
    groundArea = spanWidth*length*numSpans;
    volume = oneEndArea*length*numSpans;
    coverPerGroundArea = coverArea/groundArea;
    averageHeight = volume/groundArea;

    // Set all outputs const
    for (auto output : findMany<Port*>(".[output::*]"))
        output->setConstness(true);
}

} //namespace

