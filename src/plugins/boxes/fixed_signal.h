/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef FIXED_SIGNAL_H
#define FIXED_SIGNAL_H

#include <base/base_signal.h>

namespace boxes {

class FixedSignal : public base::BaseSignal
{
public:
    FixedSignal(QString name, Box *parent);
    bool computeFlag();
    double computeSignal(bool);
};

} //namespace
#endif
