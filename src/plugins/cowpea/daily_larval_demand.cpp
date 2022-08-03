#include <base/publish.h>
#include "daily_larval_demand.h"

using namespace base;

namespace cowpea {

PUBLISH(DailyLarvalDemand)

DailyLarvalDemand::DailyLarvalDemand(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(demandPerCapita).help("capita demand rates flowers or pods per larva per day-degree");
    Input(timeStep).help("time step in day-degrees  (Δτ_i ; °D)");
    Input(larvalDensity).help("individuals per plant");
    Output(value).help("ageing increment");
}

void DailyLarvalDemand::reset() {
    update();
}

void DailyLarvalDemand::update() {
    value = demandPerCapita * timeStep * larvalDensity;
}

}
