/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#define SACCHARINA_DOCUMENTATION_H
#ifndef SACCHARINA_DOCUMENTATION_H
#include <base/documentation.h>
#include <base/publish.h>

namespace saccharina {

class saccharinaDocumentation : public base::Documentation
{
public:
    saccharinaDocumentation(QString name, QObject *parent);
};

} //namespace

#endif
