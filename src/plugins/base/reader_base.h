/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_READER_BASE_H
#define BASE_READER_BASE_H
#include <QString>
#include "box_builder.h"

namespace base {

class BoxBuilder;

class ReaderBase
{
public:
    ReaderBase(BoxBuilder *builder);
    virtual ~ReaderBase(){}
    virtual void parse(QString filePath) = 0;
protected:
    // Data
    BoxBuilder *_builder;
    // Methods
    QString currentInfo() const;
};

}

#endif
