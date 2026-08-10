/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef CREATE_H
#define CREATE_H
#include <base/command.h>

namespace command {

class create : public base::Command
{
public:
    create(QString name, base::Box *parent);
private:
    QString _pluginName, _pluginFolderPath;

    void doExecute();
    void extractPluginName();
    void setWorkFolder();
    void createFolder();
    void writeFile(const char *text, QString fileName);
    void writeProjectFile();
    void writeDocumentationFileHeader();
    void writeDocumentationFileSource();
    void writeExampleHeader();
    void writeExampleSource();
    void writeBoxscript();
};

}

#endif
