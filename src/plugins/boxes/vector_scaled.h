/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VECTOR_SCALED_H
#define VECTOR_SCALED_H
#include <QList>
#include <QVector>
#include <base/box.h>

namespace base {
    class Port;
}

namespace boxes {

class VectorScaled : public base::Box
{
public:
    VectorScaled(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    QVector<double> vector;
    double factor;
    // Outputs
    QVector<double> value;
};

}

#endif
