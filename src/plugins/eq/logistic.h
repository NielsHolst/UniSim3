/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef LOGISTIC_H
#define LOGISTIC_H
#include <base/box.h>

namespace eq {

class Logistic : public base::Box
{
public:
    Logistic(QString name, QObject *parent);
    void update();
protected:
    // input
    double x, r, K, timeStep;
    // output
    double value;
};

} //namespace

#endif
