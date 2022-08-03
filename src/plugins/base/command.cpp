/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "command.h"
#include "dialog.h"
#include "environment.h"
#include "exception.h"
#include "mega_factory.h"

namespace base {

QStringList Command::_help;

Command::Command(QString name, Box *parent)
    : Node(name, parent), _error(false)
{
    setClassName("base", "Command");
}

void Command::arguments(QStringList args) {
    _args = args;
}

QStringList Command::arguments() const {
    return _args;
}

void Command::execute() {
    try {
        doExecute();
        _error = false;
    }
    catch (Exception &ex) {
        _error = true;
        dialog().error(ex.what());
    }
}

bool Command::hasError() const {
    Command *parentCommand = parent<Command*>();
    return _error || (parentCommand && parentCommand->hasError());
}

void Command::helpText(QString help) {
    _help << help;
}

QStringList Command::help() {
    _help.sort();
    return _help;
}

void Command::submit(QStringList com) {
    using std::unique_ptr;
    unique_ptr<Command> command;
    try {
        command = unique_ptr<Command>( MegaFactory::create<Command>(com.first(), com.first()) );
        command->arguments(com);
        command->execute();
    }
    catch(Exception &ex) {
        dialog().error(ex.what());
    }
}

}
