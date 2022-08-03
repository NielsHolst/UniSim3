#ifndef PREY_H
#define PREY_H
#include <QVector>
#include <base/box.h>

namespace demo {

class Prey : public base::Box
{
public:
    Prey(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double initial, r, K, totalPrey, timeStep, loss;
    // Outputs
    double density, rActual;
};

}

#endif
