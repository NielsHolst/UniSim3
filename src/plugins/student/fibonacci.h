#ifndef FIBONACCI_H
#define FIBONACCI_H
#include <base/box.h>

namespace student {

class Fibonacci : public base::Box
{
public:
    Fibonacci(QString name, Box *parent);
    void reset();
    void update();
private:
    // Outputs
    int value;
    // Data
    int previous;
};

}

#endif
