/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include "boxscript_def.h"
#include "boxscript_config.h"

namespace boxscript { namespace parser
{
    BOOST_SPIRIT_INSTANTIATE(boxscript_type, iterator_type, context_type);
}}
