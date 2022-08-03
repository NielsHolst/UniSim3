#ifndef EXPONENTIAL_GROWTH_H
#define EXPONENTIAL_GROWTH_H
#include <base/box.h>

namespace cowpea {

class ExponentialGrowth : public base::Box
{
public:
    ExponentialGrowth(QString name, QObject *parent);
    void reset();
    void update();
    int calcTimeDuration( double time);
private:
    // Input
    double a, e, k, t;
    // Output
    double value;
};

}

#endif
