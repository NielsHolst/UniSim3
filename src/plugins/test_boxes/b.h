#ifndef B_H
#define B_H
#include <base/box.h>

namespace test_boxes {

class B : public base::Box
{
public:
    B(QString name, base::Box *parent);
private:
    int x;
};

}
#endif

