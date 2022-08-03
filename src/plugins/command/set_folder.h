/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SET_FOLDER_H
#define SET_FOLDER_H
#include <base/command.h>

namespace command {

class set_folder : public base::Command
{
public:
    set_folder(QString name, base::Box *parent);
    static void showAllFolders();
private:
    void doExecute();
    void showFolder(QString folderType);
    void setFolder(QString folderType, QString folderPath);
    void setToDefault();
};

}

#endif
