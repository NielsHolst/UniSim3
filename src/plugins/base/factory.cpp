/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "factory.h"
#include <base/node.h>
#include <base/publish.h>

using namespace base;

namespace BOXES_PLUGIN_NAME {

QString Factory::id() const {
    return BOXES_PLUGIN_NAME_TEXT;
}

QList<QString> Factory::inventory() const{
    return productList().keys();
}

Node* Factory::create(QString className, QString objectName, Box *parent) {
    ProductList::const_iterator p = productList().find(className);
    return (p == productList().end())
            ? 0
            : p.value()->create(objectName, parent);
}

} //namespace
