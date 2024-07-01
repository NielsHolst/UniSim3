/* Copyright 2016-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef PESTTOX_DOCUMENTATION_H
#define PESTTOX_DOCUMENTATION_H
#include <base/documentation.h>
#include <base/publish.h>

namespace PestTox {

class PestToxDocumentation : public base::Documentation
{
public:
    PestToxDocumentation(QString name, Box *parent);
};

} //namespace

#endif
