/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <base/publish.h>
#include "growth_stage.h"

using namespace base;

namespace xpest {

PUBLISH(GrowthStage)

GrowthStage::GrowthStage(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(from).imports("./*[from]");
    Input(growthStages).imports("./*[growthStage]");
    Input(completed).imports("./*[completed]");
    Output(growthStage).help("Growth stage value within this stage");
}

void GrowthStage::reset() {
    growthStage = from.at(0);
    _n = completed.size();
}

void GrowthStage::update() {
    int i = 0;
    while (i < _n - 1 && completed.at(i) )
        ++i;
    growthStage = growthStages.at(i);
}

} //namespace

