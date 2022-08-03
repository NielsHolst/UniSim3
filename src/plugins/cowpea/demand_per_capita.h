#ifndef DEMANDPERCAPITA_H
#define DEMANDPERCAPITA_H
#include <base/box.h>

namespace cowpea {

class DemandPerCapita : public base::Box
{
public:
    DemandPerCapita(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Input
    double duration, perLarva;
    // Output
    double value;
};

}

#endif
