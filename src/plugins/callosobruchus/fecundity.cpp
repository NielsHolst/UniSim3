#include <base/exception.h>
#include <base/phys_math.h>
#include <base/publish.h>
#include "fecundity.h"

using namespace base;
using phys_math::minmax;

namespace callosobruchus {

PUBLISH(Fecundity)

Fecundity::Fecundity(QString name, Box *parent)
    : Box(name, parent)
{
    Doc("models female life-time fecundity");
    Input(sexRatio).equals(0.5).help("Proportion of females");
    Input(temperature).unit("oC").help("Store temperature");
    Input(slope).equals(1.279).unit("eggs per oC").help("Fecundity response on temperature");
    Input(intercept).equals(-23.1).unit("eggs").help("Fecundity response on temperature");
    Input(minTemperature).equals(25.).unit("oC").help("No extrapolation below this temperature");
    Input(maxTemperature).equals(40.).unit("oC").help("No extrapolation above this temperature");
    Output(value).unit("eggs").help("Total eggs laid per female");
}

void Fecundity::reset() {
    update();
}

void Fecundity::update() {
    const double T = minmax(minTemperature, temperature, maxTemperature);
    value = sexRatio*(slope*T - intercept);
}


} //namespace
