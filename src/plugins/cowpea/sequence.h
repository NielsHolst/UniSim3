#ifndef SEQUENCE_H
#define SEQUENCE_H
#include <QVector>
#include <base/box.h>

namespace boxes {

class Sequence : public base::Box
{
public:
    Sequence(QString name, QObject *parent);
    void amend();
    void reset();
    void update();
private:
    // Inputs
    double min, max, inflow;
    QString by;
    QVector<double> values;
    // Outputs
    int counter, counterMax, offset;
    double value;
    // Data
    bool _useValues;
    // Methods
    void updateValue();
};

}

#endif
