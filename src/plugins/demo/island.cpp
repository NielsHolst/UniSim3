#include <base/publish.h>
#include "island.h"

using namespace base;

namespace demo {

PUBLISH(Island)

Island::Island(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(latitude).equals(60);
    Input(longitude).equals(20);
}

void Island::initialize() {
}



}
