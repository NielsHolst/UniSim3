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
    // Find faces
    _faces = findMany<Box*>("./faces/*");
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


    BoxBuilder builder(this);
    builder.
    box().name("layers").
        box("AverageCover").name("cover").
        endbox().
        box().name("screens");
            for (int layer=0; layer<numLayers; ++layer) {
                QSet<QString> materials;
                for (const QStringList &screenNames : screenNamesByFace) {
                    materials << screenNames.at(layer);
                }
                builder.
                box("AverageScreen").name("screen" + QString::number(layer+1)).
                endbox();
            }
        builder.
        endbox(). // end screens
    endbox(); // end layers
}

} //namespace

