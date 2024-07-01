/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef BOXSCRIPT_CONFIG_H
#define BOXSCRIPT_CONFIG_H

#include "boxscript_error_handler.h"
#include "boxscript_skipper.h"

#include <boost/spirit/home/x3.hpp>

namespace boxscript { namespace parser
{
    // Our Iterator Type
    typedef std::string::const_iterator iterator_type;

    // The Phrase Parse Context
//    typedef x3::phrase_parse_context<x3::ascii::space_type>::type phrase_context_type;
    typedef x3::phrase_parse_context< typeof(boxscript::parser::skipper)>::type phrase_context_type;

    // Our Error Handler
    typedef error_handler<iterator_type> error_handler_type;

    // Combined Error Handler and Phrase Parse Context
    typedef x3::context<error_handler_tag,
                        std::reference_wrapper<error_handler_type>,
                        phrase_context_type>
                            context_type;
}}

#endif
