/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/dialog.h>
#include <base/publish.h>
#include "face.h"

using namespace base;

namespace vg {

PUBLISH(Face)

Face::Face(QString name, Box *parent)
    : Box(name, parent)
{
    help("models the greenhouse faces");
    Input(cover).equals("glass").help("A material listed in shelter/covers");
    Input(screens).help("Zero or more screen materials listed in shelter/screens; '+' separated");
    Input(area).unit("m2").help("Area of face");
    Input(weight).equals(1.).unit("[0;1]").help("Weight given to face in radiation budget");
    Output(screenMaterials).help("Vector of screen material names");
    Output(numScreens).help("Number of screens on this face");
}

void Face::amend() {
    static QMap<QString, QString> areaEqs =
    {
        {"roof1", "gh/geometry[roofArea] / 2"},
        {"roof2", "gh/geometry[roofArea] / 2"},
        {"side1", "gh/geometry[sideArea] / 2"},
        {"side2", "gh/geometry[sideArea] / 2"},
        {"end1" , "gh/geometry[endArea]  / 2"},
        {"end2" , "gh/geometry[endArea]  / 2"}
    };
    if (!areaEqs.contains(name()))
        ThrowException("Illegal name of face").value(name()).hint("Must be one of roof1, roof2, side1, side2, end1, end2");
    port("area")->computes(areaEqs.value(name()));
}

void Face::reset() {
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

