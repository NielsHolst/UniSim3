/* Copyright 2012-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Clement N. Mweya, National Institute for Medical Research, Tukuyu, Tanzania [cmweya at nimr.or.tz].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "water_level.h"

using namespace base;

namespace rvf {
  
PUBLISH(WaterLevel)
  
WaterLevel::WaterLevel(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(initialLevel).help("Initial water level").unit("mm");
    Input(rainfall).help("Rainfall").unit("mm/d");
    Input(lossRate).help("Daily loss rate").unit("[0;1]"); // changed to a relative loss rate
    Output(level);
}

void WaterLevel::reset() {
    level = initialLevel;
}

void WaterLevel::update() {
    level *= 1. - lossRate;
    level += rainfall;
}

} //namespace

