/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_READER_BOXSCRIPT_H
#define BASE_READER_BOXSCRIPT_H

#include "reader_base.h"

namespace ast {
    struct Node;
}
namespace base {

class ReaderBoxScript : public ReaderBase
{
public:
    ReaderBoxScript(BoxBuilder *builder);
    void parse(QString filePath);
};

}

#endif
