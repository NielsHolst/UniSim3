/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef GET_FOLDER_H
#define GET_FOLDER_H

#include <base/command.h>
#include <base/base_dllexport.h>

namespace command {

class UNI_API get_folders : public base::Command
{
public:
    get_folders(QString name, base::Box *parent);
    static void showAllFolders();
private:
    void doExecute();
};

}

#endif
