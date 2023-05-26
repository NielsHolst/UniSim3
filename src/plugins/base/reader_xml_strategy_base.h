/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_READER_XML_STRATEGY_BASE_H
#define BASE_READER_XML_STRATEGY_BASE_H
#include <QXmlStreamReader>
#include "box_builder.h"

namespace base {

class ReaderXmlStrategyBase
{
public:
    ReaderXmlStrategyBase(QXmlStreamReader *reader, BoxBuilder *builder);
    QString currentInfo() const;
    virtual void parse() = 0;

    // Data
    QXmlStreamReader *_reader;
    BoxBuilder *_builder;
};

}

#endif
