/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef BOXSCRIPT_AST_H
#define BOXSCRIPT_AST_H

#include <boost/optional.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include "path.h"
#include "value.h"
#include "base_dllexport.h"

namespace base {
    class BoxBuilder;
    class Expression;
}

namespace ast
{
    namespace x3 = boost::spirit::x3;

    struct Expression;
    struct Operand;

    typedef x3::forward_ast<Expression> GroupedExpression;

    UNI_API struct Time : x3::position_tagged {
        int hour, minute, second;
        base::Value value() const;
    };
    UNI_API std::ostream& operator<<(std::ostream& os, const Time& x);

    UNI_API struct BareDate : x3::position_tagged {
        int day, month;
        base::Value value() const;
    };
    UNI_API std::ostream& operator<<(std::ostream& os, const BareDate& x);

    UNI_API struct Bool : x3::position_tagged {
        std::string stringValue;
        base::Value value() const;
    };
    UNI_API std::ostream& operator<<(std::ostream& os, const Bool& x);

    UNI_API struct Date : x3::position_tagged {
        int day, month, year;
        base::Value value() const;
    };
    UNI_API std::ostream& operator<<(std::ostream& os, const Date& x);

    UNI_API struct DateTime : x3::position_tagged {
        Date date;
        Time time;
        base::Value value() const;
    };
    UNI_API std::ostream& operator<<(std::ostream& os, const DateTime& x);

    UNI_API struct FunctionCall : x3::position_tagged {
        typedef x3::forward_ast<Expression> Argument;

        std::string name;
        std::vector<Argument> arguments;
        void build(base::Expression *expression);
    };
    UNI_API std::ostream& operator<<(std::ostream& os, const FunctionCall& x);

    using PathNode = std::vector<std::string>;
    UNI_API std::ostream& operator<<(std::ostream& os, const PathNode& x);

    UNI_API struct PathAlternative : x3::position_tagged {
        boost::optional<std::string> root;
        std::vector<PathNode> nodes;
        boost::optional<PathNode> port;
        base::Path::Alternative value() const;
    };
    UNI_API std::ostream& operator<<(std::ostream& os, const PathAlternative& x);

    using Path = std::vector<ast::PathAlternative>;
    UNI_API std::ostream& operator<<(std::ostream& os, const Path& x);
    UNI_API base::Path value(Path &path);

    UNI_API struct QuotedString : x3::position_tagged {
        std::string stringValue;
        base::Value value() const;
    };
    UNI_API std::ostream& operator<<(std::ostream& os, const QuotedString& x);

    #define TYPE_NAME(x,y) case x : s=#y; break

//    struct integer {
//        int value;
//    };

    UNI_API struct Number : x3::variant<double, int>,
            x3::position_tagged
    {
        enum class Type{Double, Int};
        Type type() const {
            return static_cast<Type>(get().which());
        }
        std::string typeName() const {
            std::string s;
            switch (type()) {
                TYPE_NAME(Type::Double, Double);
                TYPE_NAME(Type::Int, Int);
            }
            return s;
        }
        using base_type::base_type;
        using base_type::operator=;
        base::Value value() const;
    };
    UNI_API std::ostream& operator<<(std::ostream& os, const Number& x);

    UNI_API struct Operand : x3::variant<DateTime, Date, BareDate, Time, Number,
                                 Path, FunctionCall, Bool, QuotedString, GroupedExpression>,
            x3::position_tagged
    {
        enum class Type{DateTime, Date, BareDate, Time, Number,
                        Path, FunctionCall, Bool, QuotedString, GroupedExpression};
        Type type() const {
            return static_cast<Type>(get().which());
        }
        std::string typeName() const {
            std::string s;
            switch (type()) {
                TYPE_NAME(Type::Number, Number);
                TYPE_NAME(Type::Date, Date);
                TYPE_NAME(Type::Time, Time);
                TYPE_NAME(Type::DateTime, DateTime);
                TYPE_NAME(Type::BareDate, BareDate);
                TYPE_NAME(Type::Path, Path);
                TYPE_NAME(Type::FunctionCall, FunctionCall);
                TYPE_NAME(Type::Bool, Bool);
                TYPE_NAME(Type::QuotedString, QuotedString);
                TYPE_NAME(Type::GroupedExpression, GroupedExpression);
            }
            return s;
        }
        using base_type::base_type;
        using base_type::operator=;
        void build(base::Expression *expression);
    };
    UNI_API std::ostream& operator<<(std::ostream& os, const Operand& x);

    UNI_API struct Operation : x3::position_tagged {
        std::string operator_;
        Operand operand;
        void build(base::Expression *expression);
    };
    UNI_API std::ostream& operator<<(std::ostream& os, const Operation& x);

    UNI_API struct Expression : x3::position_tagged {
        boost::optional<char> sign;
        Operand firstOperand;
        std::vector<Operation> operations;
        void build(base::BoxBuilder *builder);
        void build(base::Expression *expression);
    };
    UNI_API std::ostream& operator<<(std::ostream& os, const Expression& x);

    using IfExpression = std::vector<Expression>;

    UNI_API struct Assignment : x3::position_tagged {
        char qualifier;
        std::vector<std::string> names;
        char equals;
        boost::variant<Expression, IfExpression> expression;

        enum class Type{Expression, IfExpression};

        Type type() const {
            return static_cast<Type>(expression.which());
        }
        void build(base::BoxBuilder *builder);
        std::string toString() const;
    };
    UNI_API std::ostream& operator<<(std::ostream& os, const Assignment& x);
    UNI_API std::ostream& print     (std::ostream& os, const Assignment& x, int level);

    struct Box;
    using ChildBox = boost::recursive_wrapper<Box>;
    UNI_API std::ostream& operator<<(std::ostream& os, const ChildBox& x);
    UNI_API std::ostream& print     (std::ostream& os, const ChildBox& x, int level);

    UNI_API struct Box : x3::position_tagged {
        std::string className, objectName;
        std::vector<Assignment> assignments;
        std::vector<ChildBox> children;
        void build(base::BoxBuilder *builder);
    };
    UNI_API std::ostream& operator<<(std::ostream& os, const Box& x);
    UNI_API std::ostream& print     (std::ostream& os, const Box& x, int level);

    UNI_API struct boxscript : x3::position_tagged
    {
        Box root;
    };
    UNI_API std::ostream& operator<<(std::ostream& os, const boxscript& x);
    UNI_API std::ostream& print     (std::ostream& os, const boxscript& x);
}

#endif
