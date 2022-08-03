#include <base/publish.h>
#include "potential_larva.h"

using namespace base;

namespace cowpea {

PUBLISH(PotentialLarva)

PotentialLarva::PotentialLarva(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(supplyOfFlowers).help("light use efficiency in g/MJ");
    Input(flowersPerLarva).help("light energy intercepted MJ/m2/d");




    Output(value).help("accrued g/m2/d");

}

void PotentialLarva::reset() {
    update();
}

void PotentialLarva::update() {

    value = supplyOfFlowers / flowersPerLarva;




}


}
