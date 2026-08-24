/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef BASE_NBD_H
#define BASE_NBD_H

#include "base_dllexport.h"

namespace base {

class UNI_API Nbd {
public:
    Nbd();
    Nbd(double mu, double k);
    void reset(double mu, double k);
    double nextProbability();
private:
    bool _hasBeenReset;
    double _mu, _k, _nextProbability;
    int _x;
};

}

#endif

