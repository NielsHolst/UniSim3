/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <base/exception.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "crop_component.h"
#include "remaining_partition.h"

using namespace base;

namespace xpest {
	
PUBLISH(RemainingPartition)

RemainingPartition::RemainingPartition(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(belowGroundPartition).imports("belowGroundPartition[yValue]");
    Input(parts).imports("allSiblings::*[netPartition]");
    Output(yValue).help("Remaining after sibling partitions");
}

void RemainingPartition::update() {
    double sum = 0.;
    for (double part : parts) {
        sum += part;
    }
    TestNum::snapTo(sum, 1.);
    if (sum > 1.) {
        ThrowException("Sum of net partitions is greater than 1").value(sum).context(this);
    }
    yValue = (1. - sum)/(1. - belowGroundPartition);
}


} //namespace

