/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef THRESHOLD_SIGNAL_H
#define THRESHOLD_SIGNAL_H

#include <base/base_signal.h>

namespace boxes {

class ThresholdSignal : public base::BaseSignal
{
public:
    ThresholdSignal(QString name, Box *parent);
    bool computeFlag();
    double computeSignal(bool flag);
private:
    double threshold, input, signalFlagged, signalUnflagged;
    bool flagAbove;
};

} //namespace
#endif
