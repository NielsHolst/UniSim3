/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_OPERATION_H
#define BASE_OPERATION_H

#include "value.h"

namespace operation {
    base::Value add     (const base::Value &a, const base::Value &b);
    base::Value subtract(const base::Value &a, const base::Value &b);
    base::Value multiply(const base::Value &a, const base::Value &b);
    base::Value divide  (const base::Value &a, const base::Value &b);
}

#endif
