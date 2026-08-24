/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef BASE_READER_BOXSCRIPT_H
#define BASE_READER_BOXSCRIPT_H

#include "reader_base.h"
#include "base_dllexport.h"

namespace ast {
    struct Node;
}
namespace base {

class UNI_API ReaderBoxScript : public ReaderBase
{
public:
    ReaderBoxScript(BoxBuilder *builder);
    void parse(QString filePath);
};

}

#endif
