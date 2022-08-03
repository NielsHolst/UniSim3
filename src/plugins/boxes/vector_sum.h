/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VECTOR_SUM_H
#define VECTOR_SUM_H
#include <QList>
#include <QVector>
#include <base/box.h>

namespace base {
    class Port;
}

namespace boxes {

class VectorSum : public base::Box
{
public:
    VectorSum(QString name, Box *parent);
    void initialize();
    void reset();
    void update();
private:
    // Inputs
    QString input;
    // Outputs
    QVector<double> value;
    // Data
    QList< const QVector<double>* > _inputPtrs;
    int _size;
};

}

#endif
