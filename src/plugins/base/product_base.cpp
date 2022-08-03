/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "product_base.h"

namespace base {

ProductBase::ProductBase(QString id, ProductList &list)
    : _id(id)
{
    list[id] = this;
}

QString ProductBase::id() const {
    return _id;
}

} //namespace
