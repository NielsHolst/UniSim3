#include <base/publish.h>
#include "rgr.h"

using namespace base;

namespace cowpea {

PUBLISH(RGR)

RGR::RGR(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(t1).help("start value in days");
    Input(t2).help("end value in days");
    Input(LeafArea1).help("area of leaf at the start in cm2");
    Input(LeafArea2).help("area of leaf at end in cm2");

    Output(value).help("cm2/d");
}

void RGR::reset() {
    update();
}

void RGR::update() {
    value = log(LeafArea2) - log(LeafArea1) / t2 - t1;
}


}
