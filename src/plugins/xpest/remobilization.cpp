/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <base/publish.h>
#include "crop_component.h"
#include "remobilization.h"

using namespace base;

namespace xpest {
	
PUBLISH(Remobilization)

Remobilization::Remobilization(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(currentGs).imports("stage[growthStage]");
    Input(from).help("Name of source crop component");
    Input(to).help("Name of receiving crop component");
    Input(proportion).help("Proportion to transfer, calculated atGs");
    Input(atGs).equals(1.).help("Beginning at growth stage");
    Input(untilGs).equals(2.).help("Ending at growth stage");
    Output(rate).help("Mobilisation rate").unit("g/m2/d");
}

void Remobilization::amend() {
    fromComponent = findOne<Box*>(from);
    toComponent = findOne<Box*>(to);
}

void Remobilization::reset() {
    rate = 0.;
    _prevGs = currentGs;
    _inEffect = false;
}

void Remobilization::update() {
    if (!_inEffect && currentGs >= atGs)
        setLine();
    rate = (currentGs > atGs && currentGs <= untilGs) ?
                -_slope*(currentGs - _prevGs) : 0.;
    _inEffect = _inEffect && currentGs < untilGs;
    _prevGs = currentGs;
    effectuate();
}

void Remobilization::setLine() {
    double amount = fromComponent->port("mass")->value<double>();
    _slope = -proportion*amount/(untilGs - atGs);
    _inEffect = true;
}

void Remobilization::effectuate() {
//    fromComponent->pushValue<double>("remobilizationOut", rate);
//    toComponent->pushValue<double>("remobilizationIn", rate);
}

} //namespace

