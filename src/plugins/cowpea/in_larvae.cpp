#include <base/publish.h>
#include "in_larvae.h"

using namespace base;

namespace cowpea {

PUBLISH(Inlarvae)

Inlarvae::Inlarvae(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(amount).help("light use efficiency in g/MJ");
    Input(conversion).help("light energy intercepted MJ/m2/d");




    Output(value).help("accrued g/m2/d");

}

void Inlarvae::reset() {
    update();
}

void Inlarvae::update() {

    value = amount / conversion;




}


}
