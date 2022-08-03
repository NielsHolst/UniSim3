/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include "on_off.h"

using namespace base;

namespace boxes {

PUBLISH(OnOff)

OnOff::OnOff(QString name, Box *parent)
    : OnOffBase<double>(name, parent)
{
}

void OnOff::setSwitch() {
    isOn = (xOn <= x && x <= xOff);
}

void OnOff::setValue() {
    value = isOn ? valueOn : valueOff;
}


}
