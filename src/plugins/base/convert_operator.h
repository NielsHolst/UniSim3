/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_CONVERT_OPERATOR_H
#define BASE_CONVERT_OPERATOR_H

#include <QString>
#include "operator.h"

namespace base {

// Define conversions to type T from type U

template <class T, class U> T convert(U x);

template<> QString convert(Operator op);
template<> QString convert(Parenthesis pa);

}
#endif

