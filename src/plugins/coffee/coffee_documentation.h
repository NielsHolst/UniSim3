/* Copyright 2020-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef COFFEE_DOCUMENTATION_H
#define COFFEE_DOCUMENTATION_H
#include <base/documentation.h>

namespace coffee {

class coffeeDocumentation : public base::Documentation
{
public:
    coffeeDocumentation(QString name, QObject *parent);
};

}

#endif
