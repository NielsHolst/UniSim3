#ifndef INLARVAE_H
#define INLARVAE_H
#include <base/box.h>

namespace cowpea {

class Inlarvae : public base::Box
{
public:
    Inlarvae(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Input
    double amount, conversion;
    // Output
    double value;
};

}

#endif
