/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "layout_r.h"

using namespace boxes;

namespace base {

template<> LayoutR convert(QString source) {
    if (source == "facetted")
        return Facetted;
    if (source == "merged")
        return Merged;
    ThrowException("Unknown layout. One of these expected (facetted, merged)").value(source);
}

template<> QString convert(LayoutR source) {
    switch (source) {
        case Facetted: return "facetted";
        case Merged: return "merged";
    }
    return QString();
}


}
