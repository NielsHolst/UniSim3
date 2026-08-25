/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef WRITE_H
#define WRITE_H

#include <base/command.h>
#include <base/boxscript_converter.h>
#include <base/base_dllexport.h>

namespace command {

class UNI_API write : public base::Command
{
public:
    write(QString name, base::Box *parent);
    ~write();
private:
    // Methods
    void doExecute();
    void extractArgs();
    void writeFile();
    void edit();
    // Data
    QString _filePath;
    base::BoxscriptConverter::Option _option;
    bool _doEdit;
};

}

#endif
