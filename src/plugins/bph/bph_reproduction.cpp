/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "bph_reproduction.h"

using namespace base;

namespace bph {

PUBLISH(BphReproduction)

BphReproduction::BphReproduction(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes BPH net reproduction from rice growth stages");
    Input(max).equals(30).help("Max. net reproduction");
    Input(vegetative).imports("environment[Vegetative]");
    Input(reproductive).imports("environment[Reproductive]");
    Input(ripening).imports("environment[Ripening]");
    Input(repScale).equals(0.5).help("Down-scaling in rice reproductive stage");
    Input(ripScale).equals(0.05).help("Down-scaling in rice reproductive stage");
    Output(value).help("BPH net reproductive rate");
}

void BphReproduction::reset() {
    update();
}

void BphReproduction::update() {
    double sum = vegetative + reproductive + ripening,
           scale = (sum > 0) ? (vegetative + repScale*reproductive + ripScale*ripening)/sum : 0;
    value = max*scale;
}


}
