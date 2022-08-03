#ifndef JUMP_H
#define JUMP_H
#include <base/box.h>

namespace student {

class Jump : public base::Box
{
public:
    Jump(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    int initial;
    // Outputs
    int value;
    bool finished;
};

}

#endif
