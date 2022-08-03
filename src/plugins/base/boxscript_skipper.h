#ifndef BOXSCRIPT_SKIPPER_H
#define BOXSCRIPT_SKIPPER_H

#include <boost/spirit/home/x3.hpp>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
#pragma clang diagnostic ignored "-Wunneeded-internal-declaration"
#pragma clang diagnostic ignored "-Wmissing-variable-declarations"
#pragma clang diagnostic ignored "-Woverloaded-shift-op-parentheses"

namespace boxscript { namespace parser
{
    namespace x3 = boost::spirit::x3;
    using x3::char_;
    using x3::eol;
    using x3::eoi;
    using x3::lexeme;

    auto const single_line_comment = lexeme["//" >> *(char_ - eol) >> (eol|eoi)];
    auto const block_comment = lexeme[("/#" >> *(char_ - "#/")) > "#/"];
    auto const skipper = x3::space | single_line_comment | block_comment;
}}

#pragma clang diagnostic pop
#endif
