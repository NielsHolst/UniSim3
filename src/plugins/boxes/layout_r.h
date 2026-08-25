/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef LAYOUT_R_H
#define LAYOUT_R_H

#include <QMap>

#include <base/convert.h>
#include <base/exception.h>
#include <base/base_dllexport.h>

namespace boxes {
    enum LayoutR {Merged, Facetted};
}

namespace base  {
    template<> UNI_API boxes::LayoutR convert(QString source);
    template<class T> UNI_API T convert(boxes::LayoutR source); // undefined
    template<> UNI_API QString convert(boxes::LayoutR source);
}

#endif
