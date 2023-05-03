/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box_builder.h>
#include <base/publish.h>
#include "average_cover.h"

using namespace base;

namespace vg {

PUBLISH(AverageCover)

AverageCover::AverageCover(QString name, Box *parent)
    : Box(name, parent),
      AverageCoverOrScreen(name, parent)
{
    help("computes average cover radiation and heat parameters");
}

void AverageCover::amend() {
    // Collect the six faces
    _faces = findMany<Box*>("shelter/faces/*");
    if (_faces.size() != 6)
        ThrowException("Shelter must have 6 faces").value(_faces.size()).context(this);

    // Collect the materials referred to (one material from each face's cover)
    QSet<QString> materials;
    QString material;
    for (Box *face : _faces) {
        material = face->port("cover")->value<QString>();
        Box *cover = findMaybeOne<Box*>("shelter/covers/" + material);
        if (!cover)
            ThrowException("shelter/covers misses a material").value(material).
            hint("Used by " + face->fullName()).context(this);
        materials << material;
    }    

    // Build materials box with a box for each material used
    // and vectors of areas and weights of the faces that use each material
    BoxBuilder builder(this);
    builder.
    box().name("materials");
    for (const QString &material : qAsConst(materials))
        buildAverageMaterial(builder, material);
    builder.endbox();
}

QStringList AverageCover::collectFacesByMaterial(QString material) {
    QStringList result;
    for (Box *face : _faces) {
        if (face->port("cover")->value<QString>() == material)
            result << face->objectName();
    }
    return result;
}

QString AverageCover::pathToMaterialPorts(QString material, QString portName) const {
    return "shelter/covers/" + material + "[" + portName + "Adj]";
}

} //namespace


