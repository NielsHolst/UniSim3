#pragma once
#include <base/box.h>

namespace callosobruchus {

class Stock : public base::Box
{
public:
    Stock(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double size, seedsPerKg, initDamagePct,
        undamagedTo1, undamagedTo2;
    // Outputs
    double seedsTotal, seedsDamaged, seedsUndamaged, damagePct;
};

} //namespace
