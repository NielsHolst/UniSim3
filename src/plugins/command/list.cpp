/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box.h>
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/mega_factory.h>
// #include <base/factory_plug_in.h>
#include <base/publish.h>
#include "list.h"
#include "list_output.h"

using namespace base;

namespace command {

PUBLISH(list)
HELP(list, "list", "lists boxes, ports and plugins")

list::list(QString name, Box *parent)
    : Command(name, parent)
{
}

void list::doExecute() {
    // Drop "list"
    _args.removeAt(0);

    // Extract path (or plugin name)
    // Naked 'list'
    if (_args.isEmpty())
        _path.clear();
    // Some path and possibly options to follow
    else {
        _path = _args.first();
        _args.removeAt(0);
    }

    if (_path == "?")
        showHelp();
    else {
        extractOptions();
        listBoxes();
    }
}

void list::showHelp() {
    QString s = "Use one of these formats:\n"
                "list (defaults to list . br)\n"
                "list <path> (defaults to list <path> br)\n"
                "list <path> <options>\n\n"
            "<options> (can be combined) are\n"
            "b (to show Boxes)\n"
            "r (to show Recursively)\n"
            "p (to show Ports)\n"
            "i (to show Input ports)\n"
            "o (to show Output ports)\n"
            "a (to show Auxiliary ports)\n"
            "s (to show ports in Short format; combines with pio)\n"
            "m (to show iMported ports)\n"
            "x (to show eXported ports)\n";
     dialog().information(s);
}

void list::extractOptions() {
    QString ops = _args.join("");
    _options = convert<ListOptionSet>(ops);
}

void list::listBoxes() {
    Box *current = environment().current();
    if (!current) {
        dialog().error("No script loaded");
        return;
    }
    if (_path.isEmpty())
        _path = ".";
    QVector<Box*> boxes = current->findMany<Box*>(_path);
    if (boxes.isEmpty()) {
        dialog().error("Nothing matches: '" + _path +"'");
        return;
    }

    // Show output
    ListOutput output(boxes, _options);
    dialog().information(output.toString());
}

}
