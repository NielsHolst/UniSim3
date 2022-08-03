/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SLIDING_SIGNAL_H
#define SLIDING_SIGNAL_H

#include <base/base_signal.h>

namespace boxes {

class SlidingSignal : public base::BaseSignal
{
public:
    SlidingSignal(QString name, Box *parent);
    bool computeFlag();
    double computeSignal(bool);
    virtual double slide(double proportion) = 0;
protected:
    double input, threshold, thresholdBand, minSignal, maxSignal;
    bool increasingSignal;
};
} //namespace


#endif
