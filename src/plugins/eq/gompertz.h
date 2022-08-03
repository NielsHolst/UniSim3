/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef GOMPERTZ_H
#define GOMPERTZ_H
#include <base/box.h>

namespace eq {

class Gompertz : public base::Box
{
public:
    Gompertz(QString name, QObject *parent);
    void reset();
    void update();
private:
    // input
    QVector<double> x, dx;
    double x0,y0, x1,y1;
    // output
    QVector<double> dy;
    // methods
    double f(double x, double dx);
    // data
    double _a, _b;
};

}

#endif
