/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_READER_XML_H
#define BASE_READER_XML_H

#include <QFile>
#include <QMap>
#include <QSet>
#include <QXmlStreamReader>
#include "reader_base.h"

namespace base {

class ReaderXml : public ReaderBase
{
public:
    ReaderXml(BoxBuilder *builder);
    void parse(QString filePath);
private:
    // Data
    QXmlStreamReader _reader;
    QFile _file;
    enum class Type {Box, Port, Aux};
    Type _type;
    // Methods
    void openReader(QString filePath);
    void setElementType();
    void setBoxAttributes();
    void setPortAttributes(bool isAux);
};

}

#endif
