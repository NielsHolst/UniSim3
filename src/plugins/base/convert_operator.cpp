#include "convert_operator.h"

namespace base {

template<> QString convert(Operator op) {
    switch(op) {
    case Operator::Add:           return "+";
    case Operator::Subtract:      return "-";
    case Operator::Multiply:      return "*";
    case Operator::Divide:        return "/";
    case Operator::Exponentiate:  return "^";
    case Operator::Larger:        return ">";
    case Operator::LargerOrEqual: return ">=";
    case Operator::Less:          return "<";
    case Operator::LessOrEqual:   return "<=";
    case Operator::Equal:         return "==";
    case Operator::NotEqual:      return "!=";
    case Operator::And:           return "&&";
    case Operator::Or:            return "||";
    case Operator::Negate:        return "--";
    case Operator::Not:           return "!";
    case Operator::Comma:         return ",";
    }
    return QString();
}

template<> QString convert(Parenthesis pa) {
    switch(pa) {
    case Parenthesis::Left:       return "(";
    case Parenthesis::Right:      return ")";
    }
    return QString();
}

}


