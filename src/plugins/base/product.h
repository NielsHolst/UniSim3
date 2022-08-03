/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_PRODUCT_H
#define BASE_PRODUCT_H
#include "product_base.h"

#define PUBLISH(name) static Product<name> name##Product(#name, productList());

namespace base {

template <class T>
class Product : public ProductBase
{
public:
    Product(QString name, ProductList &list);
    Node* create(QString objectName, Box *parent) const;
};

template <class T>
Product<T>::Product(QString name, ProductList &list)
    :  ProductBase(name, list)
{
}

template <class T>
Node* Product<T>::create(QString objectName, Box *parent) const
{
    return new T(objectName, parent);
}

} //namespace

#endif
