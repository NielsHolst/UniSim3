/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SELECT_FILE_H
#define SELECT_FILE_H
#include <QStringList>
#include <base/box.h>

namespace boxes {

class SelectFile : public base::Box
{
public:
    SelectFile(QString name, Box *parent);
    void amend();
    void initialize();
    void reset();
private:
    // Input
    QString folder, filter;
    int selectFileNumber, fileNumberOffset;
    bool showFileNames;
    // Output
    int numFiles;
    QString fileName, fileNamePath;
    // Data
    QStringList _files;
    // Methods
    void reset(int fileNumber);
};

}

#endif
