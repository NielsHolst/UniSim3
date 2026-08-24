/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef BOXSCRIPT_H
#define BOXSCRIPT_H

#include "boxscript_ast.h"
#include "base_dllexport.h"

#include <boost/spirit/home/x3.hpp>

namespace boxscript
{
    namespace x3 = boost::spirit::x3;

    ///////////////////////////////////////////////////////////////////////////
    // boxscript public interface
    ///////////////////////////////////////////////////////////////////////////
    namespace parser
    {
        UNI_API struct boxscript_class;
        UNI_API typedef x3::rule<boxscript_class, ast::boxscript> boxscript_type;
        BOOST_SPIRIT_DECLARE(boxscript_type);
    }
    UNI_API parser::boxscript_type const& boxscript();
}

#endif
