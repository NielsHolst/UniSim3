/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef LIST_H
#define LIST_H
#include <base/command.h>
#include "list_options.h"

namespace command {

class list : public base::Command
{
public:
    list(QString name, base::Box *parent);

private:
    // Data
    ListOptionSet _options;
    QString _path;
    // Methods
    void doExecute();
    void showHelp();
    void extractOptions();
    void listBoxes();
};

}

#endif
