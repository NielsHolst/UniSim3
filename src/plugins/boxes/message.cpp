/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include <base/dialog.h>
#include "message.h"

using namespace base;

namespace boxes {

PUBLISH(Message)

Message::Message(QString name, Box *parent)
    : Box(name, parent)
{
    help("prints a message at the prompt");
    Input(text).help("Text message to print");
}

void Message::reset() {
}

void Message::update() {
    dialog().information(text);
}

} //namespace

