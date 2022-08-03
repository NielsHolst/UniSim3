/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "threshold_signal.h"

using namespace base;

namespace boxes {

PUBLISH(ThresholdSignal)

ThresholdSignal::ThresholdSignal(QString name, Box *parent)
    : BaseSignal(name, parent){
    help("sets a signal according to a threshold value");
    Input(input).help("Input that is compared to threshold");
    Input(threshold).help("Threshold value");
    Input(flagAbove).equals(true).help("Raise flag when input is at or above threshold?");
    Input(signalFlagged).equals(1.).help("Signal when flagged");
    Input(signalUnflagged).help("Signal when not flagged");
}

bool ThresholdSignal::computeFlag() {
    return flagAbove && (input >= threshold);
}

double ThresholdSignal::computeSignal(bool flag) {
    return flag ? signalFlagged : signalUnflagged;
}


} //namespace

