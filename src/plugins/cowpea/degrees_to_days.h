#ifndef degreestodays_H
#define degreestodays_H
#include <base/box.h>

namespace cowpea {

class degreestodays : public base::Box
{
public:
    degreestodays(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Input
    double inflow, outflow, position, DD, currentDate, T0, Tmax, plusday, startvalue;
    // Output
    double value, counterMax, inflowTag, outflowTag;
};

}

#endif
