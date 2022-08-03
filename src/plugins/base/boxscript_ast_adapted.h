#ifndef BOXSCRIPT_AST_ADAPTED_H
#define BOXSCRIPT_AST_ADAPTED_H

#include "boxscript_ast.h"

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>

BOOST_FUSION_ADAPT_STRUCT(
    ast::boxscript, root
)
BOOST_FUSION_ADAPT_STRUCT(
    ast::Assignment, qualifier, names, equals, expression
)
BOOST_FUSION_ADAPT_STRUCT(
    ast::BareDate, day, month
)
BOOST_FUSION_ADAPT_STRUCT(
    ast::Bool, stringValue
)
BOOST_FUSION_ADAPT_STRUCT(
    ast::Box, className, objectName, assignments, children
)
BOOST_FUSION_ADAPT_STRUCT(
    ast::Date, day, month, year
)
BOOST_FUSION_ADAPT_STRUCT(
    ast::DateTime, date, time
)
BOOST_FUSION_ADAPT_STRUCT(
    ast::Expression, sign, firstOperand, operations
)
BOOST_FUSION_ADAPT_STRUCT(
    ast::FunctionCall, name, arguments
)
BOOST_FUSION_ADAPT_STRUCT(
    ast::Operation, operator_, operand
)
BOOST_FUSION_ADAPT_STRUCT(
    ast::PathAlternative, root, nodes, port
)
BOOST_FUSION_ADAPT_STRUCT(
    ast::QuotedString, stringValue
)
BOOST_FUSION_ADAPT_STRUCT(
    ast::Time, hour, minute, second
)

#endif
