/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef BASE_OPERATOR_H
#define BASE_OPERATOR_H

#include <QString>

#include "base_dllexport.h"

namespace base {

enum class UNI_API Operator {
    Add,
    Subtract,
    Multiply,
    Divide,
    Exponentiate,
    Larger,
    LargerOrEqual,
    Less,
    LessOrEqual,
    Equal,
    NotEqual,
    And,
    Or,
    Negate,
    Not,
    Comma
};

enum class UNI_API Parenthesis {
    Left,
    Right
};

UNI_API extern int precedence(Operator op);

UNI_API extern int arity(Operator op);

UNI_API Operator lookupOperator(QString op);

}
#endif
