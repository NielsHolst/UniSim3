#ifndef ISLAND_H
#define ISLAND_H
#include <QVector>
#include <base/box.h>

namespace demo {

class Island : public base::Box
{
public:
    Island(QString name, QObject *parent);
    void initialize();
private:
    // Inputs
    double latitude, longitude;
};

}

#endif
