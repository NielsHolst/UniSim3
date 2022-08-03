#ifndef MinMaxValues_H
#define MinMaxValues_H
#include <base/box.h>

namespace cowpea {

class MinMaxValues : public base::Box
{
public:
    MinMaxValues(QString name, QObject *parent);
    void amend();
    void reset();
    void update();
private:
    // Input
    double input, date, counter, counterMax, by, inflowTag, outflowTag, currentminvalue, currentmaxvalue, previousminvalue, previousmaxvalue, countinterval;
    // Output
    double minvalue, maxvalue, offset;
};

}

#endif
