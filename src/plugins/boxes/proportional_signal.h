/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef PROPORTIONAL_SIGNAL_H
#define PROPORTIONAL_SIGNAL_H

#include <base/base_dllexport.h>
#include "sliding_signal.h"

namespace boxes {

class UNI_API ProportionalSignal : public SlidingSignal
{
public:
    ProportionalSignal(QString name, Box *parent);
    double slide(double proportion);
};
} //namespace


#endif
