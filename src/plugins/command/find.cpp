/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box.h>
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include "find.h"
#include "general.h"

using namespace base;

namespace command {

PUBLISH(find)
HELP(find, "find <path expression>", "finds objects matching path expression")

find::find(QString name, Box *parent)
    : Command(name, parent)
{
}

void find::doExecute() {
    QVector<Node*> matches;
    int n = _args.size();
    if (n > 1) {
        Box *box = environment().current();
        if (box) {
            QString path;
            for (int i = 1; i < n; ++i)
                path += _args.at(i);
            matches = box->findMany<Node*>(path);
            if (matches.isEmpty())
                dialog().error("No matches");
            else
                dialog().information(info(matches));
        }
        else
            dialog().error("No matches; no objects loaded");
    }
    else
        dialog().error("Write: 'find <path expression>'");
}

}
