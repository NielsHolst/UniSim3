/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef BASE_MONITOR_H

namespace base {

class Monitor {
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

