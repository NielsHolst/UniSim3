#include <iostream>
#include <sstream>
#include <typeinfo>
#include <boost/fusion/adapted/struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/move/utility.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>
#include <QFile>
#include <base/exception.h>
#include "exception_expectation.h"
#include "input_file_path.h"
#include "test_spirit.h"


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
#pragma clang diagnostic ignored "-Wunneeded-internal-declaration"
#pragma clang diagnostic ignored "-Wmissing-variable-declarations"
#pragma clang diagnostic ignored "-Woverloaded-shift-op-parentheses"

namespace x3 = boost::spirit::x3;
using std::cout;
using namespace::base;

namespace outcome {
    bool excepted, ok, exhausted;
    std::string remainder;
    void reset() {
        excepted = false;
        ok = true;
        exhausted = true;
    }
    bool check() {
        if (excepted)
            cout << "Parse excepted!\n";
        if (!ok)
            cout << "Parse not OK\n";
        if (!exhausted)
            cout << "Parse not exhausted\n";
        return !excepted && ok && exhausted;
    }
    bool notOk() {
        if (!excepted && !ok)
            return true;
        else {
            if (excepted)
                cout << "Parse excepted!\n";
            if (ok)
                cout << "Parse IS OK!\n";
            return false;
        }
    }
    bool notExhausted() {
        if (!excepted && ok && !exhausted)
            return true;
        else {
            if (excepted)
                cout << "Parse excepted!\n";
            if (!ok)
                cout << "Parse not OK!\n";
            if (exhausted)
                cout << "Parse IS exhausted\n";
            return false;
        }
    }
}

namespace ast {

    struct Bool : x3::position_tagged {
        std::string stringValue;
        friend std::ostream& operator<<(std::ostream& os, const Bool& x);
    };
    std::ostream& operator<<(std::ostream& os, const Bool& x) {
        return os << x.stringValue;
    }

    struct QuotedString : x3::position_tagged {
        std::string stringValue;
        friend std::ostream& operator<<(std::ostream& os, const QuotedString& x);
    };
    std::ostream& operator<<(std::ostream& os, const QuotedString& x) {
        return os << x.stringValue;
    }

    struct Summary : x3::position_tagged{
        std::string stringValue;
        friend std::ostream& operator<<(std::ostream& os, const Summary& x);
    };
    std::ostream& operator<<(std::ostream& os, const Summary& x) {
        return os << x.stringValue;
    }

    struct Date : x3::position_tagged {
        int day, month, year;
        friend std::ostream& operator<<(std::ostream& os, const Date& x);
    };
    std::ostream& operator<<(std::ostream& os, const Date& x) {
        return os << x.day << "/" << x.month << "/" << x.year;
    }

    struct Time : x3::position_tagged {
        int hour, minute, second;
        friend std::ostream& operator<<(std::ostream& os, const Time& x);
    };
    std::ostream& operator<<(std::ostream& os, const Time& x) {
        return os << x.hour << ":" << x.minute << ":" << x.second;
    }

    struct DateTime : x3::position_tagged {
        Date date;
        Time time;
        friend std::ostream& operator<<(std::ostream& os, const DateTime& x);
    };
    std::ostream& operator<<(std::ostream& os, const DateTime& x) {
        return os << x.date << "T" << x.time;
    }

    struct BareDate : x3::position_tagged {
        int day, month;
        friend std::ostream& operator<<(std::ostream& os, const BareDate& x);
    };
    std::ostream& operator<<(std::ostream& os, const BareDate& x) {
        return os << x.day << "/" << x.month;
    }

    struct BareDateTime : x3::position_tagged {
        BareDate date;
        Time time;
        friend std::ostream& operator<<(std::ostream& os, const BareDateTime& x);
    };
    std::ostream& operator<<(std::ostream& os, const BareDateTime& x) {
        return os << x.date << "T" << x.time;
    }

    struct Reference : x3::position_tagged {
        std::string path, port;
        boost::optional<Summary> summary;
        friend std::ostream& operator<<(std::ostream& os, const Reference& x);
    };
    std::ostream& operator<<(std::ostream& os, const Reference& x) {
        return os << x.path << "[" << x.port << "]";
    }

    struct Operand;
    struct Expression;

    struct FunctionCall : x3::position_tagged {
        typedef x3::forward_ast<Operand> Argument;

        std::string name;
        Argument argument;
        friend std::ostream& operator<<(std::ostream& os, const FunctionCall& x);
    };

    #define TYPE_NAME(x,y) case x : s=#y; break

    typedef x3::forward_ast<Expression> GroupedExpression;

    struct Operand : x3::variant<DateTime, BareDateTime, Date, BareDate, Time, double,
                                 Reference, FunctionCall, Bool, QuotedString, GroupedExpression>,
            x3::position_tagged
    {
        enum class Type{DateTime=0, BareDateTime=1, Date=2, BareDate=3, Time=4, Double=5,
                        Reference=6, FunctionCall=7, Bool=8, QuotedString=9, GroupedExpression=10};
        Type type() const {
            return static_cast<Type>(get().which());
        }
        std::string typeName() const {
            std::string s;
            switch (type()) {
                TYPE_NAME(Type::Double, Double);
                TYPE_NAME(Type::Date, Date);
                TYPE_NAME(Type::Time, Time);
                TYPE_NAME(Type::DateTime, DateTime);
                TYPE_NAME(Type::BareDate, BareDate);
                TYPE_NAME(Type::BareDateTime, BareDateTime);
                TYPE_NAME(Type::Reference, Reference);
                TYPE_NAME(Type::FunctionCall, FunctionCall);
                TYPE_NAME(Type::Bool, Bool);
                TYPE_NAME(Type::QuotedString, QuotedString);
                TYPE_NAME(Type::GroupedExpression, GroupedExpression);
            }
            return s;
        }
        using base_type::base_type;
        using base_type::operator=;
        friend std::ostream& operator<<(std::ostream& os, const Operand& x);
    };
    std::ostream& operator<<(std::ostream& os, const Operand& x) {
        bool isGrouped = (x.type() == Operand::Type::GroupedExpression);
        if (isGrouped) os << "(";
        os << x.get();
        if (isGrouped) os << ")";
        return os;

    }
    std::ostream& operator<<(std::ostream& os, const FunctionCall& x) {
        return os << x.name << "(" << x.argument.get() << ")" ;
    }

    struct Operation : x3::position_tagged {
        char operator_;
        Operand operand;
        friend std::ostream& operator<<(std::ostream& os, const Operation& x);
    };

    std::ostream& operator<<(std::ostream& os, const Operation& x) {
        return os << x.operator_ << x.operand;
    }

    struct Expression : x3::position_tagged {
        boost::optional<char> sign;
        Operand firstOperand;
        std::vector<Operation> operations;
        friend std::ostream& operator<<(std::ostream& os, const Expression& x);
    };

    std::ostream& operator<<(std::ostream& os, const Expression& x) {
        os << x.firstOperand;
        for (const Operation &operation : x.operations)
            os << operation;
        return os;
    }

    struct Assignment : x3::position_tagged {
        char qualifier;
        std::string portName;
        boost::optional<Summary> summary;
        char equals;
        Expression expression;
    };

    struct Box;
    typedef boost::recursive_wrapper<Box> ChildBox;
    struct Box : x3::position_tagged {
        std::string className, objectName;
        std::vector<Assignment> assignments;
        std::vector<ChildBox> children;
    };

}

BOOST_FUSION_ADAPT_STRUCT(
    ast::Assignment, qualifier, portName, summary, equals, expression
)
BOOST_FUSION_ADAPT_STRUCT(
    ast::BareDate, day, month
)
BOOST_FUSION_ADAPT_STRUCT(
    ast::BareDateTime, date, time
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
    ast::FunctionCall, name, argument
)
BOOST_FUSION_ADAPT_STRUCT(
    ast::Operation, operator_, operand
)
BOOST_FUSION_ADAPT_STRUCT(
    ast::QuotedString, stringValue
)
BOOST_FUSION_ADAPT_STRUCT(
    ast::Reference, path, port, summary
)
BOOST_FUSION_ADAPT_STRUCT(
    ast::Summary, stringValue
)
BOOST_FUSION_ADAPT_STRUCT(
    ast::Time, hour, minute, second
)

namespace ast {
    using x3::char_;
    using x3::double_;
    using x3::int_;
    using x3::digit;
    using x3::eol;
    using x3::eoi;
    using x3::lexeme;
    using x3::lit; // to ignore string

    struct assignment_class;
    struct bare_date_class;
    struct bare_date_time_class;
    struct bool__class;
    struct box_class;
    struct date_class;
    struct date_time_class;
    struct dots_class;
    struct expression_class;
    struct function_call_class;
    struct grouped_expression_class;
    struct joker_class;
    struct name_class;
    struct number_class;
    struct object_name_class;
    struct operand_class;
    struct operation_class;
    struct operator__class;
    struct path_class;
    struct port_class;
    struct qualified_name_class;
    struct quoted_string_class;
    struct reference_class;
    struct sign_class;
    struct summary_class;
    struct time_class;

    x3::rule<assignment_class, Assignment> const assignment = "assignment";
    x3::rule<bare_date_class, BareDate> const bare_date = "bare date";
    x3::rule<bare_date_time_class, BareDateTime> const bare_date_time = "bare date_time";
    x3::rule<bool__class, Bool> const bool_ = "bool";
    x3::rule<box_class, Box> const box = "box";
    x3::rule<date_class, Date> const date = "date";
    x3::rule<date_time_class, DateTime> const date_time = "date_time";
    x3::rule<dots_class, std::string> const dots = "dots";
    x3::rule<expression_class, Expression> const expression = "expression";
    x3::rule<function_call_class, FunctionCall> const function_call = "function call";
    x3::rule<grouped_expression_class, Expression> const grouped_expression = "grouped expression";
    x3::rule<joker_class, std::string> const joker = "joker";
    x3::rule<name_class, std::string> const name = "name";
    x3::rule<number_class, double> const number = "number";
    x3::rule<object_name_class, std::string> const object_name = "object name";
    x3::rule<operand_class, Operand> const operand = "operand";
    x3::rule<operation_class, Operation> const operation = "operation";
    x3::rule<operator__class, char> const operator_ = "operator";
    x3::rule<path_class, std::string> const path = "path";
    x3::rule<port_class, std::string> const port = "port";
    x3::rule<qualified_name_class, std::string> const qualified_name = "qualified_name";
    x3::rule<quoted_string_class, QuotedString> const quoted_string = "quoted_string";
    x3::rule<reference_class, Reference> const reference = "reference";
    x3::rule<sign_class, char> const sign = "sign";
    x3::rule<summary_class, Summary> const summary = "summary";
    x3::rule<time_class, Time> const time = "time";

//    auto const char_to_string = [](auto& ctx) { _val(ctx)= std::string(1, _attr(ctx)); };

    using boost::fusion::at_c;

    auto const dm = [](auto& ctx) {
        _val(ctx).day = at_c<0>(_attr(ctx));
        _val(ctx).month = at_c<1>(_attr(ctx));
    };

    auto const md = [](auto& ctx) {
        _val(ctx).day = at_c<1>(_attr(ctx));
        _val(ctx).month = at_c<0>(_attr(ctx));
    };

    auto const dmy_ymd = [](auto& ctx) {
        int i = at_c<0>(_attr(ctx)),
            j = at_c<1>(_attr(ctx)),
            k = at_c<2>(_attr(ctx));
        if (i<100) {
            _val(ctx).day = i;
            _val(ctx).month = j;
            _val(ctx).year = k;
        }
        else {
            _val(ctx).day = k;
            _val(ctx).month = j;
            _val(ctx).year = i;
        }
    };

    auto const mdy = [](auto& ctx) {
        int i = at_c<0>(_attr(ctx)),
            j = at_c<1>(_attr(ctx)),
            k = at_c<2>(_attr(ctx));
        _val(ctx).month = i;
        _val(ctx).day = j;
        _val(ctx).year = k;
    };

    auto const pipe_summary = [](auto& ctx) {
        std::string s = _attr(ctx);
        _val(ctx).stringValue = std::string("|") + s;
    };

    auto const assignment_def = (char_('.')|char_('+')) > name >> -summary > (char_('=')|char_('~')) > expression;
    auto const bare_date_def = (int_ >> '/' >> int_) [dm] |
                        ('/' >> int_ >> '/' >> int_) [md];
    auto const bare_date_time_def = bare_date >> (lit("T")|lit(" ")) >> time;
    auto const bool__def = lexeme[x3::string("TRUE") | x3::string("FALSE")];
    auto const box_def = qualified_name >> -name >> '{' >> *assignment >> *box > '}';
    auto const date_def = (int_ >> '/' >> int_ >> '/' > int_) [dmy_ymd] |
                          (int_ >> '.' >> int_ >> '.' > int_) [dmy_ymd] |
                          (int_ >> '-' >> int_ >> '-' > int_) [dmy_ymd] |
                   ('/' >> int_ >> '/' >> int_ >> '/' > int_) [mdy];
    auto const date_time_def = date >> (lit("T")|lit(" ")) >> time;
    auto const dots_def = lexeme[x3::repeat(1,3)[x3::string(".")]];
    auto const expression_def = -sign >> operand >> *operation;
    auto const function_call_def = name >> '(' >> operand > ')';
    auto const grouped_expression_def = '(' >> expression > ')';
    auto const joker_def = lexeme[x3::string("*")];
    auto const name_def = lexeme[char_("a-zA-Z") >> *char_("a-zA-Z0-9_")];
    auto const number_def = double_;
    auto const object_name_def = qualified_name | joker | dots;
    auto const operand_def = date_time | bare_date_time | date | bare_date | time | number |
                             reference | function_call | bool_ | quoted_string | grouped_expression;
    auto const operation_def = operator_ >> operand;
    auto const operator__def = char_("+")|char_("-")|char_("*")|char_("/")|char_("^");
    auto const path_def = -char_('/') >> object_name >> *(char_('/') >> object_name);
    auto const port_def = lit('[') >> (name | joker) > lit(']');
    auto const qualified_name_def = lexeme[name >> -(x3::string("::") > name)];
    auto const quoted_string_def = lexeme['"' >> *(char_ - '"') >> '"'];
    auto const reference_def = path >> port >> -summary;
    auto const sign_def = char_("+")|char_("-");
    auto const summary_def = lexeme[lit("|") > name] [pipe_summary];
    auto const time_def = int_ >> ':' > int_ >> -(':' > int_);

    auto const single_line_comment = lexeme["//" >> *(char_ - eol) >> (eol|eoi)];
    auto const block_comment = lexeme[("/*" >> *(char_ - "*/")) > "*/"];
    auto const skipper = x3::space | single_line_comment | block_comment;

    BOOST_SPIRIT_DEFINE(
                assignment, bare_date, bare_date_time, bool_, box, date, date_time,
                dots, expression, function_call, grouped_expression, joker,
                name, number, object_name, operand, operation, operator_,
                path, port, qualified_name, quoted_string, reference, sign,
                summary, time
                )

    // Error handler from www.boost.org/doc/libs/1_68_0/libs/spirit/example/x3/error_handling.cpp
    // See also www.codevamping.com/2018/12/spirit-x3-error-handling/

//    struct error_handler
//    {
//      template <typename Iterator, typename Exception, typename Context>
//      x3::error_handler_result on_error(
//          Iterator& first, Iterator const& last
//        , Exception const& x, Context const& context)
//      {
////          auto& error_handler = x3::get<x3::error_handler_tag>(context).get();
//          std::string message = "\nERROR! Expecting: " + x.which() + " here:";
//          cout << message << std::endl;
////          error_handler(x.where(), message);
//          return x3::error_handler_result::fail;
//      }
//    };

//    struct assignment_class : error_handler {};
//    struct bare_date_class : error_handler {};
//    struct bare_date_time_class : error_handler {};
//    struct bool__class : error_handler {};
//    struct box_class : error_handler {};
//    struct date_class : error_handler {};
//    struct date_time_class : error_handler {};
//    struct dots_class : error_handler {};
//    struct expression_class : error_handler {};
//    struct function_call_class : error_handler {};
//    struct grouped_expression_class : error_handler {};
//    struct joker_class : error_handler {};
//    struct name_class : error_handler {};
//    struct number_class : error_handler {};
//    struct object_name_class : error_handler {};
//    struct operand_class : error_handler {};
//    struct operation_class : error_handler {};
//    struct operator__class : error_handler {};
//    struct path_class : error_handler {};
//    struct port_class : error_handler {};
//    struct qualified_name_class : error_handler {};
//    struct quoted_string_class : error_handler {};
//    struct reference_class : error_handler {};
//    struct sign_class : error_handler {};
//    struct summary_class : error_handler {};
//    struct time_class : error_handler {};

}

template <class ResultType, class GrammarType>
ResultType my_parse(GrammarType rule, std::string input) {
    auto iter = input.begin();
    auto end_iter = input.end();
    outcome::reset();
    ResultType result = ResultType();
    try {
        outcome::ok = x3::parse(iter, end_iter, rule, result);
    }
    catch(...) {
        outcome::excepted = true;
    }
    outcome::exhausted = (iter==end_iter);
    outcome::remainder = std::string(iter, end_iter);
    return result;
}

template <class ResultType, class GrammarType>
ResultType my_phrase_parse(GrammarType rule, std::string input) {
    using x3::ascii::space;

    auto iter = input.begin();
    auto end_iter = input.end();
    outcome::reset();
    ResultType result;
    try {
        outcome::ok = x3::phrase_parse(iter, end_iter, rule, ast::skipper, result);
    }
    catch(...) {
        outcome::excepted = true;
    }
    outcome::exhausted = (iter==end_iter);
    outcome::remainder = std::string(iter, end_iter);
    return result;
}


template <class ResultType, class GrammarType>
ResultType my_file_parse(GrammarType rule, QString fileName) {
    using x3::ascii::space;

    QFile file;
    try {
        openInputFile(file, fileName);
    }
    catch (const Exception &ex) {
        cout << qPrintable(ex.what());
    }

    QString code = QString(file.readAll());
    std::string input = code.toStdString();

    std::string::iterator
            iter = input.begin(),
        end_iter = input.end();
    outcome::reset();
    ResultType result;
    try {
        outcome::ok = x3::phrase_parse(iter, end_iter, rule, ast::skipper, result);
    }
    catch (const x3::expectation_failure<std::string::iterator> &ex) {
        cout << "spirit exception\n";
        cout << ex.what() << "\n" << ex.which() << std::endl;
        outcome::excepted = true;
    }
    outcome::exhausted = (iter==end_iter);
    outcome::remainder = std::string(iter, end_iter);
    return result;
}

void TestSpirit::test_name() {
    auto
    name = my_parse<std::string>(ast::name, "abc");
    QVERIFY(outcome::check());
    QVERIFY(name=="abc");

    name = my_parse<std::string>(ast::name, "abc_");
    QVERIFY(outcome::check());
    QVERIFY(name=="abc_");

    name = my_parse<std::string>(ast::name, "_abc");
    QVERIFY(!outcome::notExhausted());

    name = my_parse<std::string>(ast::name, "9_abc");
    QVERIFY(outcome::notOk());

    name = my_parse<std::string>(ast::name, "abc/a");
    QVERIFY(outcome::notExhausted());
    QVERIFY(name=="abc");
    QVERIFY(outcome::remainder=="/a");
}

void TestSpirit::test_qualified_name() {
    auto
    name = my_parse<std::string>(ast::qualified_name, "abc::def");
    QVERIFY(outcome::check());
    QVERIFY(name=="abc::def");
}

void TestSpirit::test_summary() {
    auto
    name = my_parse<ast::Summary>(ast::summary, "|end");
    QVERIFY(outcome::check());
    QVERIFY(name.stringValue=="|end");
}

void TestSpirit::test_object_name() {
    auto name = my_parse<std::string>(ast::object_name, "abc");
    QVERIFY(outcome::check());
    QVERIFY(name=="abc");

    name = my_parse<std::string>(ast::object_name, "*");
    QVERIFY(outcome::check());
    QVERIFY(name=="*");

    name = my_parse<std::string>(ast::object_name, ".");
    QVERIFY(outcome::check());
    QVERIFY(name==".");

    name = my_parse<std::string>(ast::object_name, "...");
    QVERIFY(outcome::check());
    QVERIFY(name=="...");

    name = my_parse<std::string>(ast::object_name, "....");
    QVERIFY(outcome::notExhausted());
    QVERIFY(name=="...");

    name = my_parse<std::string>(ast::object_name, "a.");
    QVERIFY(outcome::notExhausted());
    QVERIFY(name=="a");

    name = my_parse<std::string>(ast::object_name, ".a");
    QVERIFY(outcome::notExhausted());
    QVERIFY(name==".");
}

void TestSpirit::test_reference() {
    auto
    ref = my_parse<ast::Reference>(ast::reference, "/abc/def[xyz]");
    QVERIFY(outcome::check());
    QVERIFY(ref.path=="/abc/def");
    QVERIFY(ref.port=="xyz");
    QVERIFY(ref.summary==boost::none);

    ref = my_parse<ast::Reference>(ast::reference, "/abc/def[xyz]|end");
    QVERIFY(outcome::check());
    QVERIFY(ref.path=="/abc/def");
    QVERIFY(ref.port=="xyz");
    QVERIFY(ref.summary!=boost::none);
    ast::Summary s = *ref.summary;
    QCOMPARE(s.stringValue, "|end");

    ref = my_parse<ast::Reference>(ast::reference, "..[xyz]");
    QVERIFY(outcome::check());
    QVERIFY(ref.path=="..");
    QVERIFY(ref.port=="xyz");

    ref = my_parse<ast::Reference>(ast::reference, "/abc/../def[xyz]");
    QVERIFY(outcome::check());
    QVERIFY(ref.path=="/abc/../def");
    QVERIFY(ref.port=="xyz");

    ref = my_parse<ast::Reference>(ast::reference, "/abc/../*[*]");
    QVERIFY(outcome::check());
    QVERIFY(ref.path=="/abc/../*");
    QVERIFY(ref.port=="*");

    ref = my_parse<ast::Reference>(ast::reference, "/quali::abc/fied::def[xyz]");
    QVERIFY(outcome::check());
    QVERIFY(ref.path=="/quali::abc/fied::def");
    QVERIFY(ref.port=="xyz");
}

void TestSpirit::test_operand() {
    auto
    op = my_parse<ast::Operand>(ast::operand, "24/12/2007");
    QVERIFY(outcome::check());
    QCOMPARE(op.type(), ast::Operand::Type::Date);
    auto date = boost::get<ast::Date>(op.get());
    QVERIFY(date.day==24 && date.month==12 && date.year==2007);

    op = my_parse<ast::Operand>(ast::operand, "24/12");
    QVERIFY(outcome::check());
    QCOMPARE(op.type(), ast::Operand::Type::BareDate);
    auto bd = boost::get<ast::BareDate>(op.get());
    QVERIFY(bd.day==24 && bd.month==12);

    op = my_parse<ast::Operand>(ast::operand, "11:12:13");
    QVERIFY(outcome::check());
    QCOMPARE(op.type(), ast::Operand::Type::Time);
    ast::Time time = boost::get<ast::Time>(op.get());
    QVERIFY(time.hour==11 && time.minute==12 && time.second==13);

    op = my_parse<ast::Operand>(ast::operand, "11:12");
    QVERIFY(outcome::check());
    QCOMPARE(op.type(), ast::Operand::Type::Time);
    time = boost::get<ast::Time>(op.get());
    QVERIFY(time.hour==11 && time.minute==12 && time.second==0);

    op = my_parse<ast::Operand>(ast::operand, "117");
    QVERIFY(outcome::check());
    QCOMPARE(op.type(), ast::Operand::Type::Double);
    double number = boost::get<double>(op.get());
    QCOMPARE(number, 117);

    op = my_parse<ast::Operand>(ast::operand, "1.17");
    QVERIFY(outcome::check());
    QCOMPARE(op.type(), ast::Operand::Type::Double);
    number = boost::get<double>(op.get());
    QCOMPARE(number, 1.17);

    op = my_parse<ast::Operand>(ast::operand, "abc/def[x]");
    QVERIFY(outcome::check());
    QCOMPARE(op.type(), ast::Operand::Type::Reference);
    ast::Reference ref = boost::get<ast::Reference>(op.get());
    QVERIFY(ref.path=="abc/def" && ref.port=="x");

    op = my_parse<ast::Operand>(ast::operand, "f(g(A[x]))");
    QVERIFY(outcome::check());
    QCOMPARE(op.type(), ast::Operand::Type::FunctionCall);

    op = my_parse<ast::Operand>(ast::operand, "FALSE");
    QVERIFY(outcome::check());
    QCOMPARE(op.type(), ast::Operand::Type::Bool);

    op = my_parse<ast::Operand>(ast::operand, "\"FALSE\"");
    QVERIFY(outcome::check());
    QCOMPARE(op.type(), ast::Operand::Type::QuotedString);

    op = my_parse<ast::Operand>(ast::operand, "(5-A[x]*8)");
    QVERIFY(outcome::check());
    QCOMPARE(op.type(), ast::Operand::Type::GroupedExpression);
}

void TestSpirit::test_sign() {
    auto
    sign = my_parse<char>(ast::sign, "+");
    QVERIFY(outcome::check());
    QCOMPARE(sign, '+');

    sign = my_parse<char>(ast::sign, "-");
    QVERIFY(outcome::check());
    QCOMPARE(sign, '-');

    sign = my_parse<char>(ast::sign, "X");
    QVERIFY(outcome::notOk());
}

void TestSpirit::test_bare_date() {
    auto
    date = my_parse<ast::BareDate>(ast::bare_date, "24/12");
    QVERIFY(outcome::check());
    QVERIFY(date.day==24 && date.month==12);

    date = my_parse<ast::BareDate>(ast::bare_date, "/12/24");
    QVERIFY(outcome::check());
    QVERIFY(date.day==24 && date.month==12);

}

void TestSpirit::test_date() {
    auto
    date = my_parse<ast::Date>(ast::date, "24/12/2007");
    QVERIFY(outcome::check());
    QVERIFY(date.day==24 && date.month==12 && date.year==2007);

    date = my_parse<ast::Date>(ast::date, "2007/12/24");
    QVERIFY(outcome::check());
    QVERIFY(date.day==24 && date.month==12 && date.year==2007);

    date = my_parse<ast::Date>(ast::date, "/12/24/2007");
    QVERIFY(outcome::check());
    QVERIFY(date.day==24 && date.month==12 && date.year==2007);
}

void TestSpirit::test_time() {
    auto
    time = my_parse<ast::Time>(ast::time, "13:14:15");
    QVERIFY(outcome::check());
    QVERIFY(time.hour==13 && time.minute==14 && time.second==15);

    time = my_parse<ast::Time>(ast::time, "16:17");
    QVERIFY(outcome::check());
    QVERIFY(time.hour==16 && time.minute==17 && time.second==0);
}

void TestSpirit::test_date_time() {
    auto
    dt = my_parse<ast::DateTime>(ast::date_time, "2007/12/24T13:14:15");
    QVERIFY(outcome::check());
    QVERIFY(dt.date.day==24 && dt.date.month==12 && dt.date.year==2007);
    QVERIFY(dt.time.hour==13 && dt.time.minute==14 && dt.time.second==15);

    dt = my_parse<ast::DateTime>(ast::date_time, "/12/24/2007 13:14");
    QVERIFY(outcome::check());
    QVERIFY(dt.date.day==24 && dt.date.month==12 && dt.date.year==2007);
    QVERIFY(dt.time.hour==13 && dt.time.minute==14 && dt.time.second==0);

}

void TestSpirit::test_bool() {
    auto
    b = my_parse<ast::Bool>(ast::bool_, "TRUE");
    QVERIFY(outcome::check());
    QVERIFY(b.stringValue=="TRUE");

    b = my_parse<ast::Bool>(ast::bool_, "FALSE");
    QVERIFY(outcome::check());
    QVERIFY(b.stringValue=="FALSE");

    b = my_parse<ast::Bool>(ast::bool_, "HECK");
    QVERIFY(outcome::notOk());
}

void TestSpirit::test_quoted_string() {
    auto
    s = my_parse<ast::QuotedString>(ast::quoted_string, "\"ABC\"");
    QVERIFY(outcome::check());
    QVERIFY(s.stringValue=="ABC");

    s = my_parse<ast::QuotedString>(ast::quoted_string, "ABC");
    QVERIFY(outcome::notOk());
}

void TestSpirit::test_function_call() {
    auto
    func = my_parse<ast::FunctionCall>(ast::function_call, "f(abc/A[x])");
    ast::Operand arg = func.argument.get();
    QVERIFY(outcome::check());
    QVERIFY(func.name=="f");
    auto ref = boost::get<ast::Reference>(arg);
    QVERIFY(ref.path=="abc/A" && ref.port=="x");

    func = my_parse<ast::FunctionCall>(ast::function_call, "sum(15.6)");
    arg = func.argument.get();
    QVERIFY(outcome::check());
    QVERIFY(func.name=="sum");
    double number = boost::get<double>(arg);
    QCOMPARE(number, 15.6);
}

void TestSpirit::test_operator() {
    char
    op = my_parse<char>(ast::operator_, "+");
    QCOMPARE(op, '+');
    op = my_parse<char>(ast::operator_, "-");
    QCOMPARE(op, '-');
    op = my_parse<char>(ast::operator_, "*");
    QCOMPARE(op, '*');
    op = my_parse<char>(ast::operator_, "/");
    QCOMPARE(op, '/');
    op = my_parse<char>(ast::operator_, "^");
    QCOMPARE(op, '^');
}


void TestSpirit::test_operation() {
    auto
    op = my_parse<ast::Operation>(ast::operation, "+A[x]");
    QVERIFY(outcome::check());
    QCOMPARE(op.operator_, '+');
    QCOMPARE(op.operand.type(), ast::Operand::Type::Reference);
    ast::Reference ref = boost::get<ast::Reference>(op.operand.get());
    QVERIFY(ref.path=="A" && ref.port=="x");

    op = my_parse<ast::Operation>(ast::operation, "^2");
    QVERIFY(outcome::check());
    QCOMPARE(op.operator_, '^');
    QCOMPARE(op.operand.type(), ast::Operand::Type::Double);
    double number = boost::get<double>(op.operand.get());
    QCOMPARE(number, 2.);

}

void TestSpirit::test_expression() {
    auto exp = my_parse<ast::Expression>(ast::expression, "3/7+sum(A[x])*14-abc[def]^2");
    std::stringstream str;
    str << exp;
    QVERIFY(outcome::check());
    QVERIFY(!exp.sign.has_value());
    QVERIFY(str.str() == "3/7+sum(A[x])*14-abc[def]^2");

    exp = my_parse<ast::Expression>(ast::expression, "5-A[x]*8/(B[y]-7)^C[x]");
    str.str("");
    str << exp;
    QVERIFY(outcome::check());
    QVERIFY(str.str() == "5-A[x]*8/(B[y]-7)^C[x]");

    exp = my_parse<ast::Expression>(ast::expression, "-(a[x]+9)");
    str.str("");
    str << exp;
    QVERIFY(outcome::check());
    QVERIFY(exp.sign.has_value());
    QVERIFY(*exp.sign=='-');
    QVERIFY(str.str() == "(a[x]+9)");
}

void TestSpirit::test_grouped_expression() {
    auto exp = my_parse<ast::Expression>(ast::grouped_expression, "(3/7+sum(A[x])*14-abc[def]^2)");
    std::stringstream str;
    str << exp;
    QVERIFY(outcome::check());
    QVERIFY(str.str() == "3/7+sum(A[x])*14-abc[def]^2");
}

void TestSpirit::test_skipper() {
    auto exp = my_phrase_parse<ast::Expression>(ast::grouped_expression, "(3/7 + sum(A[x])*14 - abc[def]^2)");
    std::stringstream str;
    str << exp;
    QVERIFY(outcome::check());
    QVERIFY(str.str() == "3/7+sum(A[x])*14-abc[def]^2");

    exp = my_phrase_parse<ast::Expression>(ast::grouped_expression, "(3/7 + sum(A[x])*14 /* comment */ - abc[def]^2) // comment + 5");
    str.str("");
    str << exp;
    QVERIFY(outcome::check());
    QVERIFY(str.str() == "3/7+sum(A[x])*14-abc[def]^2");
}

void TestSpirit::test_assignment() {
    ast::Assignment
    ass = my_phrase_parse<ast::Assignment>(ast::assignment, ".abc = 117");
    QVERIFY(outcome::check());
    QCOMPARE(ass.qualifier, '.');
    QCOMPARE(ass.portName, "abc");
    QCOMPARE(ass.equals, '=');
    std::stringstream str;
    str <<  ass.expression;
    QCOMPARE(str.str(), "117");
    QVERIFY(ass.summary==boost::none);

    ass = my_phrase_parse<ast::Assignment>(ast::assignment, ".abc|sum ~ 117");
    QVERIFY(outcome::check());
    QCOMPARE(ass.qualifier, '.');
    QCOMPARE(ass.portName, "abc");
    QCOMPARE(ass.equals, '~');
    str.str("");
    str <<  ass.expression;
    QCOMPARE(str.str(), "117");

    QVERIFY(ass.summary!=boost::none);
    ast::Summary s = *ass.summary;
    QCOMPARE(s.stringValue, "|sum");
}

void TestSpirit::test_box() {
    auto box = my_phrase_parse<ast::Box>(ast::box, "A  a{}");
    QVERIFY(outcome::check());
    QCOMPARE(box.className, "A");
    QCOMPARE(box.objectName, "a");

    box = my_phrase_parse<ast::Box>(ast::box, "B {}");
    QVERIFY(outcome::check());
    QCOMPARE(box.className, "B");
    QCOMPARE(box.objectName, "");

    box = my_phrase_parse<ast::Box>(ast::box, "A { B{} C{} }");
    QVERIFY(outcome::check());

    box = my_phrase_parse<ast::Box>(ast::box, "A { .x=B[y] B{ +abc = 1.23} C{} }");
    QVERIFY(outcome::check());
}

void TestSpirit::test_box_script() {
    auto
    box = my_phrase_parse<ast::Box>(ast::box, "// butterfly6.box\nSimulation sim {\n}\n");
    QVERIFY(outcome::check());

    box = my_file_parse<ast::Box>(ast::box, "butterfly6.box");
    QVERIFY(outcome::check());
}
