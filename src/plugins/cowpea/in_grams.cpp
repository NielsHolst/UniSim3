#include <base/publish.h>
#include "in_grams.h"

using namespace base;

namespace cowpea {

PUBLISH(Ingrams)

Ingrams::Ingrams(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(amount).help("light use efficiency in g/MJ");
    Input(attackRate).help("light energy intercepted MJ/m2/d");
    Input(firstgen).help("light energy intercepted MJ/m2/d");
    Input(duration).help("light energy intercepted MJ/m2/d");





    Output(value).help("accrued g/m2/d");

}

void Ingrams::reset() {
    update();
}

void Ingrams::update() {

    value = (firstgen + amount) * attackRate * duration;




}


}
