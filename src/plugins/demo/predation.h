#ifndef PREDATION_H
#define PREDATION_H
#include <QStringList>
#include <QVector>
#include <base/data_frame.h>
#include <base/box.h>
#include <base/port.h>

namespace demo {

class Predation : public base::Box
{
public:
    Predation(QString name, QObject *parent);
    void amend();
//    void reset();
//    void update();
private:
    // Input
    QString apparancyFile;

    // Data
    base::DataFrame apparancy;
    QVector<base::Port*> preyPorts, predPorts;
    int nPrey, nPred;
    QVector<double> preyDensity, predDensity,
                    predDemands;

    // Methods
    QVector<base::Port*> findPorts(QStringList paths);
    void setupInputPorts();
    void setupInputPorts(QString prefix,
                         const QVector<base::Port*> &ports,
                         QVector<double> &density);

};

}
#endif
