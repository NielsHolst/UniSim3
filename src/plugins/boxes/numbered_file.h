/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef NUMBERED_FILE_H
#define NUMBERED_FILE_H
#include <base/box.h>

namespace boxes {

class NumberedFile : public base::Box
{
public:
    NumberedFile(QString name, Box *parent);
    void initialize();
    void reset();
    void update();
private:
    // Input
    QString fileName;
    int evaluationOrder, numberWidth;
    // Output
    QString value;
};

}

#endif
