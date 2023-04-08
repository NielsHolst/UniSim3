/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box_builder.h>
#include <base/publish.h>
#include <base/vector_op.h>
#include "shelter.h"
#include <base/vector_op.h>

using std::max;
using namespace base;

namespace vg {

PUBLISH(Shelter)

Shelter::Shelter(QString name, Box *parent)
    : Box(name, parent)
{
    help("holds parameters of the greenhouse shelter");
}

void Shelter::amend() {
    BoxBuilder builder(this);
    int numChildren = children().size();
    bool hasDefaultCover     = findMaybeOne<Box*>("./Cover::cover"),
         hasDefaultScreen    = findMaybeOne<Box*>("./Screen::screen"),
         hasDefaultCoverUtop = findMaybeOne<Box*>("./coverUtop");
    if (numChildren<4) {
        if (!hasDefaultCover) {
            builder.
            box().name("cover").
                aux("swReflectivityTop").     equals(0.1).
                aux("swTransmissivityTop").   equals(0.8).
                aux("swReflectivityBottom").  equals(0.1).
                aux("swTransmissivityBottom").equals(0.8).
                aux("lwReflectivityTop").     equals(0.15).
                aux("lwTransmissivityTop").   equals(0.20).
                aux("lwReflectivityBottom").  equals(0.15).
                aux("lwTransmissivityBottom").equals(0.20).
                aux("Ubottom").               equals(1.2).
                aux("heatCapacity").          equals(8400.).
            endbox();
        }
        if (!hasDefaultScreen) {
            builder.
            box().name("screen").
                aux("swReflectivityTop").     equals(0.93).
                aux("swTransmissivityTop").   equals(0.0).
                aux("swReflectivityBottom").  equals(0.43).
                aux("swTransmissivityBottom").equals(0.0).
                aux("lwReflectivityTop").     equals(0.93).
                aux("lwTransmissivityTop").   equals(0.0).
                aux("lwReflectivityBottom").  equals(0.43).
                aux("lwTransmissivityBottom").equals(0.0).
                aux("Utop").                  equals(1.2).
                aux("Ubottom").               equals(1.2).
                aux("heatCapacity").          equals(80.).
            endbox();
        }
        if (!hasDefaultCoverUtop) {
            builder.
            box("UWind").name("coverUtop").
            endbox();
        }
    }
    if (!findMaybeOne<Box*>("./roof1"))
        createFace(builder, "roof1");
    if (!findMaybeOne<Box*>("./roof2"))
        createFace(builder, "roof3");
    if (!findMaybeOne<Box*>("./side1"))
        createFace(builder, "side1");
    if (!findMaybeOne<Box*>("./side2"))
        createFace(builder, "side2");
    if (!findMaybeOne<Box*>("./end1"))
        createFace(builder, "end1");
    if (!findMaybeOne<Box*>("./end2"))
        createFace(builder, "end2");
}

void Shelter::createFace(BoxBuilder &builder, QString faceName) {
    builder.
        box("Box").name(faceName).
            box("Layer").name("cover").
                port("swReflectivityTop").     imports("../../cover[swReflectivityTop]").
                port("swTransmissivityTop").   imports("../../cover[swTransmissivityTop]").
                port("swReflectivityBottom").  imports("../../cover[swReflectivityBottom]").
                port("swTransmissivityBottom").imports("../../cover[swTransmissivityBottom]").
                port("lwReflectivityTop").     imports("../../cover[lwReflectivityTop]").
                port("lwTransmissivityTop").   imports("../../cover[lwTransmissivityTop]").
                port("lwReflectivityBottom").  imports("../../cover[lwReflectivityBottom]").
                port("lwTransmissivityBottom").imports("../../cover[lwTransmissivityBottom]").
                port("Utop").                  imports("../../coverUtop[value]").
                port("Ubottom").               imports("../../cover[Ubottom]").
                port("heatCapacity").          imports("../../cover[heatCapacity]").
            endbox().
            box("Layer").name("screen").
                port("swReflectivityTop").     imports("../../screen[swReflectivityTop]").
                port("swTransmissivityTop").   imports("../../screen[swTransmissivityTop]").
                port("swReflectivityBottom").  imports("../../screen[swReflectivityBottom]").
                port("swTransmissivityBottom").imports("../../screen[swTransmissivityBottom]").
                port("lwReflectivityTop").     imports("../../screen[lwReflectivityTop]").
                port("lwTransmissivityTop").   imports("../../screen[lwTransmissivityTop]").
                port("lwReflectivityBottom").  imports("../../screen[lwReflectivityBottom]").
                port("lwTransmissivityBottom").imports("../../screen[lwTransmissivityBottom]").
                port("Utop").                  imports("../../screen[Utop]").
                port("Ubottom").               imports("../../screen[Ubottom]").
                port("heatCapacity").          imports("../../screen[heatCapacity]").
            endbox().
        endbox();
}


} //namespace

