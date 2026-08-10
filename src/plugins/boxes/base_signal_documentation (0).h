/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef BASE_SIGNAL_DOCUMENATION_H
#define BASE_SIGNAL_DOCUMENATION_H

#include <base/base_signal.h>

namespace boxes {

class BaseSignalDocumentation : public base::BaseSignal
{
public:
    BaseSignalDocumentation(QString name, Box *parent);
    bool computeFlag();
    double computeSignal(bool);
};

} //namespace
#endif
