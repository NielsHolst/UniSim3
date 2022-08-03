#include <base/publish.h>
#include "in_grams2.h"

using namespace base;

namespace cowpea {

PUBLISH(Ingrams2)

Ingrams2::Ingrams2(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(currentamount).help("light use efficiency in g/MJ");
    Input(conversion).help("light energy intercepted MJ/m2/d");



    Output(value).help("accrued g/m2/d");

}

void Ingrams2::reset() {
    update();
}

void Ingrams2::update() {

    value = currentamount * conversion;




}


}
