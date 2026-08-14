#include <base/exception.h>
#include <base/phys_math.h>
#include <base/publish.h>
#include "stock.h"

using namespace base;
using phys_math::minmax;

namespace callosobruchus {

PUBLISH(Stock)

Stock::Stock(QString name, Box *parent)
    : Box(name, parent)
{
    help("models female life-time fecundiy");
    Input(size).equals(100.).unit("kg").help("Weight of seeds stored");
    Input(seedsPerKg).equals(7000.).unit("per kg").help("Number of seeds per kg put on storage");
    Output(seedsTotal).help("Total no. of seeds");
}

void Stock::reset() {
    seedsTotal = size*seedsPerKg;
}

} //namespace
