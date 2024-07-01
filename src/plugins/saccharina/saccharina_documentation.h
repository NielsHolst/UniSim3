/* Copyright 2019-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Teis Boderskov, Aarhus University [tebo atbios.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef SACCHARINA_DOCUMENTATION_H
#define SACCHARINA_DOCUMENTATION_H
#include <base/documentation.h>

namespace saccharina {

class saccharinaDocumentation : public base::Documentation
{
public:
    saccharinaDocumentation(QString name, Box *parent);
};

}

#endif
