#ifndef BOXSCRIPT_H
#define BOXSCRIPT_H

#include "boxscript_ast.h"

#include <boost/spirit/home/x3.hpp>

namespace boxscript
{
    namespace x3 = boost::spirit::x3;

    ///////////////////////////////////////////////////////////////////////////
    // boxscript public interface
    ///////////////////////////////////////////////////////////////////////////
    namespace parser
    {
        struct boxscript_class;
        typedef x3::rule<boxscript_class, ast::boxscript> boxscript_type;
        BOOST_SPIRIT_DECLARE(boxscript_type);
    }
    parser::boxscript_type const& boxscript();
}

#endif
