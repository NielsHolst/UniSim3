/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_PRODUCT_BASE_H
#define BASE_PRODUCT_BASE_H

#include <QMap>
#include <QString>

namespace base {

class Box;
class Node;
class ProductBase;
using ProductList =  QMap<QString, ProductBase*>;

class ProductBase
{
public:
    ProductBase(QString name, ProductList &list);
    virtual Node* create(QString objectName, Box *parent) const = 0;
    QString id() const;
private:
    QString _id;
};

} //namespace

#endif
