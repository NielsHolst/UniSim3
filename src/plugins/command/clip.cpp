/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stdio.h>
#include <QFileInfo>
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/publish.h>
#include "clip.h"

using namespace base;

namespace command {

PUBLISH(clip)
HELP(clip, "clip", "repeats latest output to clipboard")

clip::clip(QString name, Box *parent)
    : Command(name, parent)
{
}


void clip::doExecute() {
    if (_args.size() > 1)
        ThrowException("'clip' takes no arguments");
    environment().recreateClipboard();
}


}
