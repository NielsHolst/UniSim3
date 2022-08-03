/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box.h>
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/port.h>
#include <base/publish.h>
#include "general.h"
#include "go.h"

using namespace base;

namespace command {

PUBLISH(go)
HELP(help_go, "go <path expression>", "goes to object matching path expression, must be unique")

go::go(QString name, Box *parent)
    : Command(name, parent)
{
}

void go::doExecute() {
    QVector<Node*> matches;
    if (_args.size() == 2) {
        QString path = _args.at(1);
        Box *box = environment().current();
        if (!box)
            box = Box::root();
        if (box) {
            matches = box->findMany<Node*>(path);
            if (matches.isEmpty())
                dialog().error("No matches");
            else if (matches.size() > 1) {
                dialog().error("Path must be unique; too many matches:\n" + info(matches));
            }
            else {
                Box *box = dynamic_cast<Box*>( matches.at(0) );
                if (!box)
                    dialog().error("Path does not match a box:\n" + info(matches));
                else {
                    environment().current(box);
                    dialog().information("Now at " + info(box));
                }
            }
        }
        else
            dialog().error("No matches; no objects loaded");
    }
    else
        dialog().error("Write: 'go <path expression>'");
}

}
