/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef TRANSITION_H
#define TRANSITION_H
#include <base/box.h>

namespace eq {

class Transition : public base::Box
{
public:
    Transition(QString name, QObject *parent);
    void reset();
    void update();
protected:
    // input
    double input, threshold, thresholdBand, minValue, maxValue, resetValue;
    bool increasingValue;
    // output
    double value;
};

} //namespace

#endif
