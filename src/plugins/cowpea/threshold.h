#ifndef THRESHOLD_H
#define THRESHOLD_H
#include <base/box.h>

namespace cowpea {

class Threshold : public base::Box
{
public:
    Threshold(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Input
    double content, inflow, application, threshold;
    // Output
    double value, outflow;
};

}

#endif
