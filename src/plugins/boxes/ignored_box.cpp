/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include "ignored_box.h"
using namespace base;

namespace boxes {

PUBLISH(IgnoredBox)

IgnoredBox::IgnoredBox(QString name, Box *parent)
    : Box(name, parent) {
    Doc("is ignored during standard operations (`reset()`, `update()`, etc.");
    ignore(true);
}

} //namespace

