/* Copyright 2019-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Teis Boderskov, Aarhus University [tebo atbios.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include "density.h"

using namespace base;

namespace saccharina {

Density::Density(QString name, Box *parent)
    : Box(name, parent)
{
    help("provides density in different units");
    Output(dw).unit("g dw/m").help("Total dry weight");
    Output(C).unit("g C/m").help("Carbon weight");
    Output(N).unit("g N/m").help("Nitrogen weight");
    Output(P).unit("g P/m").help("Phosphorus weight");
}

}
