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
#include "average_cover.h"

using namespace base;

namespace vg {

PUBLISH(AverageCover)

AverageCover::AverageCover(QString name, Box *parent)
    : Box(name, parent),
      AverageCoverOrScreen(name, parent)
{
    help("computes average cover radiation and heat parameters");
    Input(groundArea).imports("geometry[groundArea]");
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

    // If materials box already exists then remove it from boxscript
    Box *materialsBox = findMaybeOne<Box*>("./materials");
    if (materialsBox)
        ThrowException("Cover materials cannot be included in script. Please delete materials from boxscript and load it again").
                value(materialsBox->fullName()).context(this);

    // Build materials box with a box for each material used
    // and vectors of areas and weights of the faces that use each material
    BoxBuilder builder(this);
    builder.
    box().name("materials");
    for (const QString &material : qAsConst(materials))
        buildAverageMaterial(builder, material);
    builder.endbox();
}

void AverageCover::reset() {
    for (int i = 0; i < 6; ++i)
        _Uinsulations[i].clear();

    int i = 0;
    for (Box *face : _faces) {
        _areas[i] = face->port("area")->value<double>();

        QString coverMaterial = face->port("cover")->value<QString>();
        Box *cover = findOne<Box*>("shelter/covers/" + coverMaterial);
        _Rfaces[i] = 1./cover->port("Ubottom")->value<double>();

        for (QString screenMaterial : face->port("screenMaterials")->value<QVector<QString>>()) {
            if (screenMaterial != "none") {
                Box *screen = findOne<Box*>("shelter/screens/" + screenMaterial);
                // On one face, resistances are in a series
                _Uinsulations[i] <<  screen->port("UinsulationAdj")->valuePtr<double>();
            }
        }
        ++i;
    }
    AverageCoverOrScreen::reset();
}

void AverageCover::updateUbottomAdj() {
//    UbottomAdj = vector_op::sum(Ubottoms);
    double sumUface = 0;
    for (int i = 0; i < 6; ++i) {
        // On one face, resistances are in a series
        double Rface = _Rfaces[i];
        for (const double *Uinsulation : _Uinsulations[i])
            Rface += 1./(*Uinsulation);
        // Overall, the resistances of the six surfaces are in parallel
        sumUface += _areas[i]*1./Rface;
    }
    UbottomAdj = sumUface/groundArea;
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


