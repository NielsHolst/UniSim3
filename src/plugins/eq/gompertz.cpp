/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <algorithm>
#include <base/publish.h>
#include "gompertz.h"

using namespace base;

namespace eq {

PUBLISH(Gompertz)

Gompertz::Gompertz(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates y-increments for given x-increments along an integral Gompertz curve (increasing from 0 to 1)");

    QVector<double> one;
    one << 1;

    Input(x).help("Vector of current x-values");
    Input(x0).equals(10).help("Value of x at the lower fractile (y0)");
    Input(y0).equals(0.05).help("The lower fractile ]0;1]");
    Input(x1).equals(90).help("Value of x at the upper fractile (y1)");
    Input(y1).equals(0.95).help("The upper fractile ]0;1]");
    Input(dx).equals(one).help("By how much is x increasing? dx must be a vector macthing x or, a scalar");
    Output(dy).help("By how much will y increase in response to the increase in x (dx)? Will match size of x and dx" );
}

#define CHECK_Y(y) \
    if (y<=0 || y>1) \
        ThrowException(#y " must belong to the interval ]0;1]").context(this).value(y)

void Gompertz::reset() {
    CHECK_Y(y0);
    CHECK_Y(y1);
    double A = log(-log(y0)),
           B = log(-log(y1));
    _b = (B-A)/(x0-x1);
    _a = exp(A + _b*x0);

    dy.resize(x.size());
}

inline double F(double x, double a, double b) {
    return exp(-a*exp(-b*x));
}

double Gompertz::f(double x, double dx) {
    double y0 = F(x, _a, _b),
           y1 = F(x+dx, _a, _b);
    return (1-y0 > 1e-30) ? (y1-y0)/(1-y0) : 0.;

}

void Gompertz::update() {
    bool dxIsScalar = (dx.size() == 1);
    if (!dxIsScalar && x.size() != dx.size())
        ThrowException("x and dx vectors must match in size, or else dx could be a scalar").value1(x.size()).value2(dx.size()).context(this);

    const double *px = x.data();
    double *py = dy.data();

    if (dxIsScalar) {
        double dx0 = dx.at(0);
        for (int i=0; i <x.size(); ++i)
            *py++ = f(*px++, dx0);
    }
    else {
        const double *pdx = dx.data();
        for (int i=0; i <x.size(); ++i)
            *py++ = f(*px++, *pdx++);
    }
//    std::transform(x.constBegin(), x.constEnd(), y.begin(), [this](const double &x){return this->f(x);});
}

}
