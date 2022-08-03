#ifndef RGR_H
#define RGR_H
#include <base/box.h>

namespace cowpea {

class RGR : public base::Box
{
public:
    RGR(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Input
    int t1, t2, LeafArea1, LeafArea2;
    // Output
    int value;
};

}

#endif
