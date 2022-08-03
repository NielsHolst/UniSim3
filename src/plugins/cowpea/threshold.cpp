#include <base/publish.h>
#include "threshold.h"

using namespace base;

namespace cowpea {

PUBLISH(Threshold)

Threshold::Threshold(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(content).help("current content of instars box");
    Input(application).help("application amount");
    Input(threshold).help("amount of larva at which application is use");
    Input(inflow).help("inflow to redirect OBs outflow to the cadavars");





    Output(value).help("application amount");
    Output(outflow).help("application amount");


}

void Threshold::reset() {
    update();
}

void Threshold::update() {

    if (content > threshold) {
        value = application + inflow;
    }

    else if (content < threshold) {
        value = inflow;
    }


}


}
