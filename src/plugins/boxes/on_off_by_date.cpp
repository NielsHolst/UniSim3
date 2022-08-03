/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/

#include <base/publish.h>
#include "on_off_by_date.h"

using namespace base;

namespace boxes {

PUBLISH(OnOffByDate)

OnOffByDate::OnOffByDate(QString name, Box *parent)
    : OnOffBase<QDate, base::BareDate>(name, parent)
{
    port("x")   ->unit("date");
    port("xOn") ->unit("Bare date");
    port("xOff")->unit("Bare date");
}

void OnOffByDate::setSwitch() {
    BareDate date(x.month(), x.day());
    isOn = (xOn < xOff) ?
            // If xOn precedes xOff (within-year interval)
            date >= xOn && date <= xOff :
            // The other way round (between-year interval)
            date >= xOn || date <= xOff; // simple and surprising!
}

void OnOffByDate::setValue() {
    value = isOn ? valueOn : valueOff;
}


}
