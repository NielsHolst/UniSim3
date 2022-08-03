#ifndef COMPETITION_H
#define COMPETITION_H
#include <QVector>
#include <base/box.h>

namespace student {

class Competition : public base::Box
{
public:
    Competition(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    QVector<double> A;
    // Outputs
    double value;
};

}

#endif
