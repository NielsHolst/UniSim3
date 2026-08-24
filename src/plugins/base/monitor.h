/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef BASE_MONITOR_H

#include "base_dllexport.h"

namespace base {

class UNI_API Monitor {
public:
    Monitor(bool *flag) {
        _flag  = flag;
        *_flag = true;
    }

    ~Monitor() {
        *_flag = false;
    }

private:
    bool *_flag;
};

}

#endif

