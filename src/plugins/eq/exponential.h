/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef EXPONENTIAL_H
#define EXPONENTIAL_H
#include <base/box.h>

namespace eq {

class Exponential : public base::Box
{
public:
    Exponential(QString name, QObject *parent);
    void reset();
    void update();
private:
    // input
    QVector<double> y;
    double r, dt, yMax;
    // output
    QVector<double> dy;
    // methods
    double fdy(double y);
};

}

#endif
