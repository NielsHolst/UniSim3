#ifndef LI_H
#define LI_H
#include <base/box.h>

namespace cowpea {

class LI : public base::Box
{
public:
    LI(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Input
    double k, LAI, radiation, startDate, endDate, dayOfYear;
    // Output
    double value;
};

}

#endif
