/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/dialog.h>
#include <base/publish.h>
#include <base/phys_math.h>
#include <base/solar_math.h>
#include "face.h"

using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(Face)

Face::Face(QString name, Box *parent)
    : Box(name, parent)
{
    help("models the greenhouse faces");
    Input(cover).equals("glass").help("A material listed in shelter/covers");
    Input(screens).help("Zero or more screen materials listed in shelter/screens; '+' separated");
    Input(area).unit("m2").help("Area of face");
    Input(faceSlope).unit("[0;180]").help("Slope of face to horizontal");
    Input(faceAzimuth).unit("[0;180]").help("Compass direction along face");
    Input(sunElevation).imports("sun[elevation]");
    Input(sunAzimuth).imports("sun[azimuth]");
    Input(lwWeight).help("Weight given to face in long-wave radiation budget");

    Output(swWeight).help("Weight given to face in short-wave radiation budget");
    Output(screenMaterials).help("Vector of screen material names");
    Output(numScreens).help("Number of screens on this face");
    Output(aoi).unit("[-90,90]").help("Angle of sunnlight incidence");
}

void Face::amend() {
    static QMap<QString, QString> areaEqs =
    {
        {"roof1", "geometry[roofArea] / 2"},
        {"roof2", "geometry[roofArea] / 2"},
        {"side1", "geometry[sideArea] / 2"},
        {"side2", "geometry[sideArea] / 2"},
        {"end1" , "geometry[endArea]  / 2"},
        {"end2" , "geometry[endArea]  / 2"}
    };
    static QMap<QString, QString> slopeEqs =
    {
        {"roof1", "geometry[roofPitch]"},
        {"roof2", "-geometry[roofPitch]"},
        {"side1", "90"},
        {"side2", "90"},
        {"end1" , "90"},
        {"end2" , "90"}
    };
    static QMap<QString, QString> azEqs =
    {
        {"roof1", "geometry[orientation]"},
        {"roof2", "geometry[orientation]"},
        {"side1", "geometry[orientation]"},
        {"side2", "-geometry[orientation]"},
        {"end1" , "geometry[orientation] + 90"},
        {"end2" , "-geometry[orientation] - 90"}
    };
    static QMap<QString, QString> lwEqs =
    {
        {"roof1", "1. - geometry[roofPitch]/180."},
        {"roof2", "1. - geometry[roofPitch]/180."},
        {"side1", "0.5"},
        {"side2", "0.5"},
        {"end1" , "0.5"},
        {"end2" , "0.5"}
    };
    if (!areaEqs.contains(name()))
        ThrowException("Illegal name of face").value(name()).hint("Must be one of roof1, roof2, side1, side2, end1, end2");

    port("area")->computes(areaEqs.value(name()));
    port("faceSlope")->computes(slopeEqs.value(name()));
    port("faceAzimuth")->computes(azEqs.value(name()));
    port("lwWeight")->computes(lwEqs.value(name()));
}

void Face::reset() {
    swWeight = 1.;
    QStringList list = screens.split("+", Qt::SkipEmptyParts);
    screenMaterials = QVector<QString>(list.cbegin(), list.cend());
    numScreens = screenMaterials.size();
    _cover.setPointers(findOne<Box*>("shelter/products/covers/" + cover));

    _screens.clear();
    for (QString &screen : screenMaterials) {
        LayerParametersPtrs p;
        p.setPointers(findOne<Box*>("shelter/products/screens/" + screen));
        _screens << p;
    }
}

inline double dsin(double x) {
    return sin(x*PI/180.);
}

void Face::update() {
    aoi = angleOfIncidence(sunElevation, sunAzimuth, faceSlope, faceAzimuth);
    Q_ASSERT(aoi >= -180. && aoi <= 180);
    // At night all faces have equal weight; there could be sw radiation from growth lights
    if (sunElevation < 0.)
        swWeight = 1.;
    // When face is backwards lit, it counts as zero
    else if (aoi < 0.)
        swWeight = 0.;
    // Else the sun is up and shining in the face of the surface
    else
        swWeight = dsin(aoi);
}

const LayerParametersPtrs& Face::coverParameters() const {
    return _cover;
}

const LayerParametersPtrs& Face::screenParameters(int index) const {
    Q_ASSERT(index < screens.size());
    return _screens.at(index);
}

const LayerParametersPtrs& Face::parameters(int index) const {
    if (index == 0)
        return _cover;
    Q_ASSERT(index-1 < screens.size());
    return _screens.at(index-1);
}

} //namespace

