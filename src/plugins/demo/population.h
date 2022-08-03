#ifndef POPULATION_H
#define POPULATION_H
#include <QVector>
#include <base/box.h>

namespace demo {

class Population : public base::Box
{
public:
    Population(QString name, QObject *parent);
    void initialize();
private:
    // Inputs
    double emigrationSlope;
};

}

#endif
