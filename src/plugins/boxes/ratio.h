/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef RATIO_H
#define RATIO_H
#include <QList>
#include <QVector>
#include <base/box.h>

namespace boxes {

class Ratio : public base::Box
{
public:
    Ratio(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double denominator, divisor, zeroTolerance;
    bool allowInfinityResult;
    // Outputs
    double value;
};

}

#endif
