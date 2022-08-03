#ifndef INGRAMS2_H
#define INGRAMS2_H
#include <base/box.h>

namespace cowpea {

class Ingrams2 : public base::Box
{
public:
    Ingrams2(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Input
    double currentamount, conversion;
    // Output
    double value;
};

}

#endif
