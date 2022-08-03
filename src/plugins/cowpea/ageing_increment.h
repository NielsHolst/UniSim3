#ifndef AGEINGINCREMENT_H
#define AGEINGINCREMENT_H
#include <base/box.h>

namespace cowpea {

class AgeingIncrement : public base::Box
{
public:
    AgeingIncrement(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Input
    double incrementDD, sdRatio;
    // Output
    double value;
};

}

#endif
