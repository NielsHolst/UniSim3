/* Copyright 2020-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef XPEST_DOCUMENTATION_H
#define XPEST_DOCUMENTATION_H
#include <base/documentation.h>

namespace xpest {

class xpestDocumentation : public base::Documentation
{
public:
    xpestDocumentation(QString name, QObject *parent);
};

}

#endif
