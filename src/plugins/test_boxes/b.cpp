#include <base/publish.h>
#include "b.h"

using namespace base;

namespace test_boxes {

PUBLISH(B)

B::B(QString name, Box *parent)
    : Box(name, parent)
{
    Input(x);
}

}
