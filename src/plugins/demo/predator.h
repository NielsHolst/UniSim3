#ifndef PREDATOR_H
#define PREDATOR_H
#include <QVector>
#include <base/box.h>

namespace demo {

class Predator : public base::Box
{
public:
    Predator(QString name, QObject *parent);
    void initialize();
private:
    // Inputs
    double r, demand, supply;
    QVector<double> prey, attackRates;
    // Output
    double sdRatio, rActual;
};

}

#endif
