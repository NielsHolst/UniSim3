#include <base/publish.h>
#include "demand_per_capita.h"

using namespace base;

namespace cowpea {

PUBLISH(DemandPerCapita)

DemandPerCapita::DemandPerCapita(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(duration).help("larval duration in day-degrees");
    Input(perLarva).help("demand per larva");



    Output(value).help("per capita demand rate - flowers or pods per larva per day-degree");

}

void DemandPerCapita::reset() {
    update();
}

void DemandPerCapita::update() {

    value = perLarva / duration ;




}


}
