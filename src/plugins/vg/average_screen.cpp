/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box_builder.h>
#include <base/exception.h>
#include <base/publish.h>
#include <base/vector_op.h>
#include "average_screen.h"

using namespace base;


namespace vg {

PUBLISH(AverageScreen)

AverageScreen::AverageScreen(QString name, Box *parent)
    : Box(name, parent),
      AverageCoverOrScreen(name, parent)
{
    help("computes average screen radiation and heat parameters");
}

void AverageScreen::amend() {
    // Collect the six faces
    _faces = findMany<Box*>("shelter/faces/*");
    if (_faces.size() != 6)
        ThrowException("Shelter must have 6 faces").value(_faces.size()).context(this);

    // Collect screens referred to
    int numLayers = 0;
    QVector<QStringList> screenNamesByFace;
    for (Box *face : _faces) {
        Port *port = face->peakPort("screens");
        if (!port)
            ThrowException("Face must have a port called 'screens'").
            value(face->fullName()).context(this);
        QStringList screenNames = port->value<QString>().split("+");
        if (numLayers == 0)
            numLayers = screenNames.size();
        else if (numLayers != screenNames.size())
            ThrowException("All faces must have the same number of screen layers").
            value(numLayers).value2(screenNames.size()).hint(port->fullName()).context(this);
        screenNamesByFace << screenNames;
    }

    // Collect materials
    int layerNumber = getNumberFromName();
    QSet<QString> materials;
    for (const QStringList &screenNames : screenNamesByFace) {
        materials << screenNames.at(layerNumber-1);
    }

    BoxBuilder builder(this);
    builder.
    box().name("materials");
    for (const QString &material : materials.values()) {
        if (material == "none")
            buildAverageMaterialNone(builder);
        else
            buildAverageMaterial(builder, material);
    }
    builder.endbox(); //end materials
}

int AverageScreen::getNumberFromName() const {
    bool ok;
    int result = objectName().mid(6).toInt(&ok);
    if (!ok)
        ThrowException("Screen number missing in box name").value(objectName()).context(this);
    return result;
}


void AverageScreen::buildAverageMaterialNone(BoxBuilder &builder) {
    QStringList myFaces = collectFacesByMaterial("none");
    builder.
    box("AverageMaterial").name("none").
        port("swTransmissivityTopAdj").equals(1.).
        port("swTransmissivityBottomAdj").equals(1.).
        port("lwTransmissivityTopAdj").equals(1.).
        port("lwTransmissivityBottomAdj").equals(1.).
        port("Utop").equals(1e-32).
        port("Ubottom").equals(1e-32).
        port("myAreas").  imports(pathToFaces(myFaces, "area")).
        port("myWeights").imports(pathToFaces(myFaces, "weight")).
    endbox();
}


QStringList AverageScreen::collectFacesByMaterial(QString material) {
    QStringList result;
    for (Box *face : _faces) {
        QStringList materials = face->port("screens")->value<QString>().split("+");
        if (materials.contains(material))
            result << face->objectName();
    }
    return result;
}

QString AverageScreen::pathToMaterialPorts(QString material, QString portName) const {
    return "shelter/screens/" + material + "[" + portName + "Adj]";
}

} //namespace

