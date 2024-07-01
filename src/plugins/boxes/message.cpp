/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
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
    Input(when).help("In which step to print the message (reset, initialize, etc.)");
}

void Message::initialize() {
    if (when == "initialize")
        dialog().information(when + " " + text);
}

void Message::reset() {
    if (when == "reset")
        dialog().information(when + " " + text);
}

void Message::update() {
    if (when == "update")
        dialog().information(when + " " + text);
}

void Message::cleanup() {
    if (when == "cleanup")
        dialog().information(when + " " + text);
}

void Message::debrief() {
    if (when == "debrief")
        dialog().information(when + " " + text);
}

} //namespace

