#include <base/publish.h>
#include "ageing_increment.h"

using namespace base;

namespace cowpea {

PUBLISH(AgeingIncrement)

AgeingIncrement::AgeingIncrement(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(incrementDD).help("daily increment in day-degrees");
    Input(sdRatio).help("daily supply and demand ratio");



    Output(value).help("ageing increment");

}

void AgeingIncrement::reset() {
    update();
}

void AgeingIncrement::update() {

    value = incrementDD * sdRatio;




}


}
