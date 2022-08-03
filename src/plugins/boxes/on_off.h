/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef ON_OFF_H
#define ON_OFF_H
#include "on_off_base.h"

namespace boxes {

class OnOff : public OnOffBase<double>
{
public:
    OnOff(QString name, Box *parent);
private:
    // Methods
    void setSwitch();
    void setValue();
};

}

#endif
