/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "energy_saving.h"

using namespace base;

namespace vg {

PUBLISH(EnergySaving)

EnergySaving::EnergySaving(QString name, Box *parent)
    : Box(name, parent)
{
    help("looks up % energy saving of a screen product");
    Input(screenName).help("Name of screen product");
    Output(value).unit("%").help("Documented energy saving used for calibration");
}

void EnergySaving::reset() {
    QString path = "shelter/products/screens/" + screenName;
    value = findOne<Box*>(path)->port("energySaving")->value<double>();
}

} //namespace

