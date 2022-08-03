/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef CLIP_H
#define CLIP_H
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <base/command.h>

namespace command {

class clip : public base::Command
{
public:
    clip(QString name, base::Box *parent);
private:
    // Methods
    void doExecute();
    void readFile(QString fileName);
    enum FileType {Boxes, Xml};
    FileType fileType(QString fileName);
};

}

#endif
