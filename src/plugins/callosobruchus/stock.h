#pragma once
#include <base/box.h>
#include <base/base_dllexport.h>

namespace callosobruchus {

class UNI_API Stock : public base::Box
{
public:
    Stock(QString name, Box *parent);
    void reset() override;
private:
    // Inputs
    double size, seedsPerKg;
    // Outputs
    double seedsTotal;
};

} //namespace
