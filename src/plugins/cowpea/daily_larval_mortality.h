#ifndef DAILY_LARVAL_MORTALITY_H
#define DAILY_LARVAL_MORTALITY_H
#include <base/box.h>

namespace cowpea {

class DailyLarvalMortality : public base::Box
{
public:
    DailyLarvalMortality(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Input
    double sdRatio, exponent, timeStep;
    // Output
    double value;
};

}

#endif
