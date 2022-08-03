/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef LAYOUT_R_H
#define LAYOUT_R_H

#include <QMap>
#include <base/convert.h>
#include <base/exception.h>

namespace boxes {
    enum LayoutR {Merged, Facetted};
}

namespace base  {
    template<> boxes::LayoutR convert(QString source);
    template<class T> T convert(boxes::LayoutR source); // undefined
    template<> QString convert(boxes::LayoutR source);
}

#endif
