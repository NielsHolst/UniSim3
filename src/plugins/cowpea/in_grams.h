#ifndef INGRAMS_H
#define INGRAMS_H
#include <base/box.h>

namespace cowpea {

class Ingrams : public base::Box
{
public:
    Ingrams(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Input
    double firstgen, amount, attackRate, duration;
    // Output
    double value;
};

}

#endif
