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
    Doc("models female life-time fecundiy");
    Input(size).equals(100.).unit("kg").help("Weight of seeds stored");
    Input(seedsPerKg).equals(7000.).unit("per kg").help("Number of seeds per kg put on storage");
    Input(undamagedTo1).help("No. of undamaged seeds -> parasitised once");
    Input(undamagedTo2).help("No. of undamaged seeds -> parasitised more than once");
    Output(seedsTotal).help("Total no. of seeds");
    Output(seedsUndamaged).help("No. of undamaged seeds");
    Output(seedsDamaged).help("No. of parasitised and exhausted seeds");
}

void Stock::reset() {
    seedsTotal = seedsUndamaged = size*seedsPerKg;
    update();
}

void Stock::update() {
    seedsDamaged   += undamagedTo1 + undamagedTo2;
    seedsUndamaged -= undamagedTo1 + undamagedTo2;
    damagePct = seedsDamaged/seedsTotal;
}


} //namespace
