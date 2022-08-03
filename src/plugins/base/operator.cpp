/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <map>
#include "exception.h"
#include "operator.h"

using std::map;

namespace base {

static map<Operator,int> precedences =
{
   {Operator::Add          , 6},
   {Operator::Subtract     , 6},
   {Operator::Multiply     , 7},
   {Operator::Divide       , 7},
   {Operator::Exponentiate , 9},
   {Operator::Larger       , 3},
   {Operator::LargerOrEqual, 3},
   {Operator::Less         , 3},
   {Operator::LessOrEqual  , 3},
   {Operator::Equal        , 3},
   {Operator::NotEqual     , 3},
   {Operator::And          , 5},
   {Operator::Or           , 4},
   {Operator::Negate       , 8},
   {Operator::Not          , 8},
   {Operator::Comma        , 0}
};

static map<Operator,int> arities =
{
   {Operator::Add          , 2},
   {Operator::Subtract     , 2},
   {Operator::Multiply     , 2},
   {Operator::Divide       , 2},
   {Operator::Exponentiate , 2},
   {Operator::Larger       , 2},
   {Operator::LargerOrEqual, 2},
   {Operator::Less         , 2},
   {Operator::LessOrEqual  , 2},
   {Operator::Equal        , 2},
   {Operator::NotEqual     , 2},
   {Operator::And          , 2},
   {Operator::Or           , 2},
   {Operator::Negate       , 1},
   {Operator::Not          , 1},
   {Operator::Comma        , 0}
};

int precedence(Operator op) {
    return precedences.at(op);
}

int arity(Operator op) {
    return arities.at(op);
}

Operator lookupOperator(QString op) {
    if (op.size() == 1) switch (op.at(0).toLatin1()){
    case '+': return Operator::Add;
    case '-': return Operator::Subtract;
    case '*': return Operator::Multiply;
    case '/': return Operator::Divide;
    case '^': return Operator::Exponentiate;
    case '>': return Operator::Larger;
    case '<': return Operator::Less;
    case '!': return Operator::Not;
    case ',': return Operator::Comma;
    default: ;
    }
    else if (op == ">=") return Operator::LargerOrEqual;
    else if (op == "<=") return Operator::LessOrEqual;
    else if (op == "==") return Operator::Equal;
    else if (op == "!=") return Operator::NotEqual;
    else if (op == "&&") return Operator::And;
    else if (op == "||") return Operator::Or;
    ThrowException("Unknown operator").value(op);
}

}

