/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <base/publish.h>
#include "crop_component.h"

using namespace base;

namespace xpest {
	
PUBLISH(CropComponent)

CropComponent::CropComponent(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(partitioningCoefficient).imports("./partition[yValue]");
    Input(assimilate).imports("growth[gain]");
    Input(belowGroundPartition).imports("belowGroundPartition[yValue]");
    Input(isBelowGround).equals(false).help("Is this component below ground?");
    Input(sla).help("Specific leaf area").unit("m2/g)");
    Input(initMass).equals(100.).help("Initial biomass per ground area").unit("g/m2");
    Input(senescence).help("Proportion of daily biomass loss").unit("per d");
    Input(remobilizationIn).help("Remobilised assimilated inflow").unit("g/m2/d");
    Input(remobilizationOut).help("Remobilised assimilated outflow").unit("g/m2/d");
    Input(assimilateLoss).help("Proportional loss of assimilates").unit("[0;1]");

    Output(netPartition).help("Net partition").unit("[0;1]");
    Output(mass).help("Biomass per ground area").unit("g/m2");
    Output(lai).help("Leaf area index").unit("m2/m2");;
}

void CropComponent::reset() {
    mass = initMass;
    updateFromMass();
}

void CropComponent::update() {
    netPartition = partitioningCoefficient *
                    (isBelowGround ? belowGroundPartition : 1. - belowGroundPartition);
    mass *= 1. - senescence;
    mass += netPartition*assimilate*(1. - assimilateLoss) + remobilizationIn - remobilizationOut;
    updateFromMass();
}

void CropComponent::updateFromMass() {
    lai = mass*sla;
}

} //namespace

