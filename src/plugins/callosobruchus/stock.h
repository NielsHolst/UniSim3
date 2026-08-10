#pragma once
#include <base/box.h>

namespace callosobruchus {

class Stock : public base::Box
{
public:
    Stock(QString name, Box *parent);
    void reset();
private:
    // Inputs
    double size, seedsPerKg;
    // Outputs
    double seedsTotal;
};

} //namespace
