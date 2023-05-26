/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_READER_XML_STRATEGY_GENERIC_H
#define BASE_READER_XML_STRATEGY_GENERIC_H

#include "reader_xml_strategy_base.h"

namespace base {

class ReaderXmlStrategyGeneric : public ReaderXmlStrategyBase
{
public:
    ReaderXmlStrategyGeneric(QXmlStreamReader *reader, BoxBuilder *builder);
    void parse();
private:
    // Data
    enum class Type {Box, Port, Aux};
    Type _type;
    // Methods
    void setElementType();
    void setBoxAttributes();
    void setPortAttributes(bool isAux);
};

}

#endif
