/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef BOXSCRIPT_ERROR_HANDLER_H
#define BOXSCRIPT_ERROR_HANDLER_H

#include "boxscript.h"

#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>

#include <map>

namespace boxscript { namespace parser
{
    namespace x3 = boost::spirit::x3;

    ////////////////////////////////////////////////////////////////////////////
    //  Our error handler
    ////////////////////////////////////////////////////////////////////////////
    // X3 Error Handler Utility
    template <typename Iterator>
    using error_handler = x3::error_handler<Iterator>;

    // tag used to get our error handler from the context
    using error_handler_tag = x3::error_handler_tag;

    struct error_handler_base
    {
        template <typename Iterator, typename Exception, typename Context>
        x3::error_handler_result on_error(
            Iterator& first, Iterator const& last
          , Exception const& x, Context const& context);

    };

    ////////////////////////////////////////////////////////////////////////////
    // Implementation
    ////////////////////////////////////////////////////////////////////////////

    template <typename Iterator, typename Exception, typename Context>
    inline x3::error_handler_result
    error_handler_base::on_error(
        Iterator& /*first*/, Iterator const& /*last*/
      , Exception const& x, Context const& context)
    {
        std::string which = x.which();
        if (!which.empty() && which.at(0)!='\'')
            which = "<" + which + ">";
        std::string message = "Error! Expecting " + which + " here:";
        auto& error_handler = x3::get<error_handler_tag>(context).get();
        error_handler(x.where(), message);
        return x3::error_handler_result::fail;
    }
}}

#endif
