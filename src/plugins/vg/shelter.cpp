/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
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
    Input(propFrame).equals(0.2).unit("[0;1]").help("Proportion of shelter covered by the frame");
    Input(swReflectivityFrame).equals(0.2).unit("[0;1]").help("Short-wave reflectivity of the frame");
    Input(lwReflectivityFrame).equals(0.15).unit("[0;1]").help("Long-wave reflectivity of the frame");
    Input(specHeatCapacityFrame).equals(490).unit("J/g/K").help("Specific heat capacity of frame material");
    Input(weightFrame).equals(27.5).unit("kg/m2 ground").help("Weight of frame material");
    Output(heatCapacityFrame).unit("J/m2 ground/K").help("Heat capacity of frame");
}

void Shelter::amend() {
    // Find faces
    QVector<Box*> _faces = findMany<Box*>("./faces/*");
    if (_faces.size() != 6)
        ThrowException("Shelter must have 6 faces").value(_faces.size()).context(this);

    // Collect screens referred to
    int numLayers = -999;
    for (Box *face : _faces) {
        Port *port = face->peakPort("screens");
        if (!port)
            ThrowException("Face must have a port called 'screens'").
            value(face->fullName()).context(this);
        QStringList screenNames = port->value<QString>().split("+", Qt::SkipEmptyParts);
        if (numLayers == -999)
            numLayers = screenNames.size();
        else if (numLayers != screenNames.size())
            ThrowException("All faces must have the same number of screen layers").
            value(numLayers).value2(screenNames.size()).hint(port->fullName()).context(this);
    }

    // Build shelter layers; must be unique
    if (!findMaybeOne<Box*>("./layers")) {
        BoxBuilder builder(this);
        builder.
        box().name("layers").
            box("AverageCover").name("cover").
            endbox().
            box().name("screens");
                for (int layer=0; layer<numLayers; ++layer) {
                    builder.
                    box("AverageScreen").name("screen" + QString::number(layer+1)).
                    endbox();
                }
            builder.
            endbox(). // end screens
        endbox(); // end layers
        Box *layers = findOne<Box*>("./layers");
        layers->doWriteOnCommand(false);
    }
}

void Shelter::reset() {
    heatCapacityFrame = specHeatCapacityFrame*weightFrame*1e3; // convert kg to g
}

} //namespace

