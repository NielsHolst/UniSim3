/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_OPERATOR_H
#define BASE_OPERATOR_H
#include <QString>

namespace base {

enum class Operator {
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

enum class Parenthesis {
    Left,
    Right
};

extern int precedence(Operator op);

extern int arity(Operator op);

Operator lookupOperator(QString op);

}
#endif
