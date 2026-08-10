#pragma once
#include <base/box.h>

namespace callosobruchus {

class FixedPreferences : public base::Box
{
public:
    FixedPreferences(QString name, Box *parent);
    void reset();
private:
    // Inputs
    double p0, p1;
    // Outputs
    double p2;
};

} //namespace
