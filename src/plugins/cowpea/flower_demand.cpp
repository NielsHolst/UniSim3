#include <base/publish.h>
#include "flower_demand.h"

using namespace base;

namespace cowpea {

PUBLISH(FlowerDemand)

FlowerDemand::FlowerDemand(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(density).help("light use efficiency in g/MJ");
    Input(flowersPerLarva).help("light energy intercepted MJ/m2/d");



    Output(value).help("accrued g/m2/d");

}

void FlowerDemand::reset() {
    update();
}

void FlowerDemand::update() {

    value = density * flowersPerLarva;




}


}
