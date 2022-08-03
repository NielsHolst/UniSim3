#ifndef INCREMENT_H
#define INCREMENT_H
#include <base/box.h>

namespace cowpea {

class Increment : public base::Box
{
public:
    Increment(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Input
    double Current, increment, StartDate, EndDate, tMax, dayOfYear;
    // Output
    double value;
};

}

#endif
