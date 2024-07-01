/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <iostream>
#include <sstream>
#include <QSet>
#include <QString>
#include <QDate>
#include <QDateTime>
#include <QTime>
#include "bare_date.h"
#include "box_builder.h"
#include "boxscript_ast.h"
#include "convert.h"
#include "expression.h"
#include "operator.h"

namespace ast {

    inline QString str(const std::string &s) {
        return QString::fromStdString(s);
    }

    inline std::string spaces(int n) { return std::string(n, ' '); }

    inline std::string pad(int level) { return spaces(2*level); }

    inline std::string zpad(int i) {
        std::string s = std::to_string(i);
        return std::string(2 - s.length(), '0') + s;
    }

    std::string Assignment::toString() const {
        std::stringstream str;
        str << *this;
        return str.str();
    }

    std::ostream& operator<<(std::ostream& os, const Assignment& x) {
        return print(os, x, 0);
    }

    std::ostream& print(std::ostream& os, const Assignment& x, int level) {
        os << pad(level) << x.qualifier;

        int namesSize = 0;
        bool separate = false;
        for (auto name : x.names) {
            os << name;
            namesSize += name.size();
            if (separate) os << " ";
            separate = true;
        }
        namesSize += x.names.size() - 1;

        os << " " << x.equals;
        if (x.type() == Assignment::Type::Expression) {
            os << " " << boost::get<Expression>(x.expression) << std::endl;
        }
        else {
            const auto &e(boost::get<IfExpression>(x.expression));
            const int n = e.size(),
                      padding = 2*level + namesSize + 4;
            os << " if " << e.at(0) << " then " << e.at(1) << std::endl;
            for (int i=2; i<n-1; i+=2)
                os << spaces(padding) << "elsif " << e.at(i) << " then " << e.at(i+1) << std::endl;
            os << spaces(padding) << "else " << e.at(n-1) << std::endl;
        }
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const BareDate& x) {
        return os << x.day << "/" << x.month;
    }

    std::ostream& operator<<(std::ostream& os, const Bool& x) {
        return os << x.stringValue;
    }

    std::ostream& operator<<(std::ostream& os, const Box& x) {
        return print(os, x, 0);
    }

    std::ostream& print(std::ostream& os, const Box& x, int level) {
        os << pad(level) << x.className << " " << x.objectName << " {\n";
        for (auto y : x.assignments)
            print(os, y, level+1);
        for (auto y : x.children)
            print(os, y, level+1);
        os << pad(level) << "}\n";
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const boxscript& x) {
        return print(os, x.root, 0);
    }

    std::ostream& operator<<(std::ostream& os, const ChildBox& x) {
        return os << x.get().objectName;
    }

    std::ostream& print(std::ostream& os, const ChildBox& x, int level) {
        return print(os, x.get(), level);
    }

    std::ostream& operator<<(std::ostream& os, const Date& x) {
        return os << x.day << "/" << x.month << "/" << x.year;
    }

    std::ostream& operator<<(std::ostream& os, const DateTime& x) {
        return os << x.date << "T" << x.time;
    }

    std::ostream& operator<<(std::ostream& os, const Expression& x) {
        if (x.sign.has_value() && x.sign.value()!='+')
            os << x.sign.value();
        os << x.firstOperand;
        for (const Operation &operation : x.operations)
            os << operation;
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const FunctionCall& x) {
        std::stringstream str;
        str << x.name << "(";
        for (auto argument : x.arguments)
            str << argument.get() << ", ";
        std::string s = str.str();
        return os << s.erase(s.length()-2) << ")" ;
    }

    std::ostream& operator<<(std::ostream& os, const Number& x) {
        return os << x.get();
    }

    std::ostream& operator<<(std::ostream& os, const Operand& x) {
        bool isGrouped = (x.type() == Operand::Type::GroupedExpression);
        if (isGrouped) os << "(";
        os << x.get();
        if (isGrouped) os << ")";
        return os;

    }

    std::ostream& operator<<(std::ostream& os, const Operation& x) {
        return os << x.operator_ << x.operand;
    }

    std::ostream& operator<<(std::ostream& os, const Path &x) {
        bool sep = false;
        for (auto alternative : x) {
            if (sep)
                os << "|";
            os << alternative;
            sep = true;
        }
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const PathAlternative &x) {
        if (x.root.has_value())
            os << "/";
        bool sep = false;
        for (auto node : x.nodes) {
            if (sep)
                os << "/";
            os << node;
            sep = true;
        }
        os << "[";
        if (x.port.has_value())
            os << x.port.value();
        os << "]";
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const PathNode& x) {
        bool sep = false;
        for (auto name : x) {
            if (sep)
                os << "::";
            os << name;
            sep = true;
        }
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const QuotedString& x) {
        return os << '"' << x.stringValue << '"';
    }

    std::ostream& operator<<(std::ostream& os, const Time& x) {
        return os << zpad(x.hour) << ":" << zpad(x.minute) << ":" << zpad(x.second);
    }

    base::Value Time::value() const {
        return QTime(hour, minute, second);
    }

    base::Value BareDate::value() const {
        return base::BareDate(month, day);
    }

    base::Value Bool::value() const {
        return (stringValue == "TRUE");
    }

    base::Value Date::value() const {
        return QDate(year, month, day);
    }

    base::Value DateTime::value() const {
        QDate qdate = QDate(date.year, date.month, date.day);
        auto gtime = base::convertGuarded(time.hour, time.minute, time.second);
        qdate = qdate.addDays(gtime.second);
        return QDateTime(qdate, gtime.first, Qt::UTC);
    }

    void FunctionCall::build(base::Expression *expression) {
        auto f = base::Expression::FunctionCall(str(name), arguments.size());
        expression->push(f);
        for (auto arg : arguments) {
            arg.get().build(expression);
            expression->push(base::Operator::Comma);
        }
        expression->push(base::Expression::FunctionCallEnd());
    }

    base::Path::Alternative PathAlternative::value() const {
        base::Path::Alternative alternative;
        alternative.setRoot(root.has_value());
        for (auto node : nodes) {
            auto baseNode = base::Path::Node(node);
            alternative.addNode(baseNode);
        }
        if (port.has_value() && !port.value().empty())
            alternative.setPort( base::Path::Port(port.value()) );
        return alternative;
    }

    base::Path value(Path &path) {
        base::Path result;
        for (const auto &alternative : path)
            result.add(alternative.value());
        return result;
    }

    base::Value QuotedString::value() const {
        return QString::fromStdString(stringValue);
    }

    base::Value Number::value() const {
        base::Value val;
        if (type()==Type::Double)
            val = boost::get<double>(*this);
        else
            val = boost::get<int>(*this);
        return val;
    }

    void Operand::build(base::Expression *expression) {
        using boost::get;
        switch (type()) {
        case Type::DateTime:
            expression->push(get<DateTime    >(*this).value());  break;
        case Type::Date:
            expression->push(get<Date        >(*this).value()); break;
        case Type::BareDate:
            expression->push(get<BareDate    >(*this).value());  break;
        case Type::Time:
            expression->push(get<Time        >(*this).value()); break;
        case Type::Number:
            expression->push(get<Number      >(*this).value()); break;
        case Type::Path:
            expression->push(value(get<ast::Path>(*this)));        break;
        case Type::FunctionCall:
                             get<FunctionCall>(*this).build(expression); break;
        case Type::Bool:
            expression->push(get<Bool        >(*this).value()); break;
        case Type::QuotedString:
            expression->push(get<QuotedString>(*this).value()); break;
        case Type::GroupedExpression:
            expression->push(base::Parenthesis::Left);
                             get<GroupedExpression>(*this).get().build(expression);
            expression->push(base::Parenthesis::Right);
            break;
        }
    }

    void Operation::build(base::Expression *expression) {
        expression->push( base::lookupOperator(QString::fromStdString(operator_)) );
        operand.build(expression);
    }

    void Expression::build(base::BoxBuilder *builder) {
        base::Expression expression;
        build(&expression);
        expression.close();
        builder->equals(expression);
    }

    void Expression::build(base::Expression *e) {
        if (sign.has_value()) {
            if (sign.value()=='-')
                e->push(base::Operator::Negate);
            else if (sign.value()=='!')
                e->push(base::Operator::Not);
        }
        firstOperand.build(e);
        for (auto op : operations)
            op.build(e);
    }

    void Assignment::build(base::BoxBuilder *builder) {
        // Left-hand side
        bool isAuxPort = (qualifier != '.');
        QString typeName, portName;
        switch (names.size()) {
        case 1:
            portName = str(names.at(0));
            break;
        case 2:
            typeName = str(names.at(0));
            portName = str(names.at(1));
            break;
        default:
            ThrowException("Expected <name> or <type> <name> for variable").value(str(toString()));
        }

        if (isAuxPort)
            builder->aux(portName, typeName);
        else {
            if (!typeName.isEmpty())
                ThrowException("You cannot re-define the type of an existing port").value(str(toString()));
            builder->port(portName);
        }
        // Deal with assignment operator (=~) here
        //...

        // Right-hand side
        if (type() == Type::Expression)
            boost::get<Expression>(expression).build(builder);
        else { // IfExpression
            base::Expression expr;
            auto e = boost::get<IfExpression>(expression);
            // if
            expr.push(base::Expression::Conditional::If);
            // expression
            auto it = e.begin();
            (*it).build(&expr); it++;
            // then
            expr.push(base::Expression::Conditional::Then);
            // expression
            (*it).build(&expr); it++;
            while (it != e.end()-1) {
                // elsif
                expr.push(base::Expression::Conditional::Elsif);
                // expression
                (*it).build(&expr); it++;
                // then
                expr.push(base::Expression::Conditional::Then);
                // expression
                (*it).build(&expr); it++;
            }
            // else
            expr.push(base::Expression::Conditional::Else);
            // expression
            (*it).build(&expr); it++;
            // finished
            expr.close();
            builder->equals(expr);
        }
    }

    void Box::build(base::BoxBuilder *builder) {
        builder
        ->box(str(className))
        .name(str(objectName));
        for (auto assignment : assignments)
            assignment.build(builder);
        for (auto child : children)
            child.get().build(builder);
        builder->endbox();
    }


}

