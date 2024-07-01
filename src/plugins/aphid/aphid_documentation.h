/* Copyright 2018-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Stéphanie Saussure, Norwegian Institute of Bioeconomy Research [stephanie.saussure at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef APHID_DOCUMENTATION_H
#define APHID_DOCUMENTATION_H
#include <base/documentation.h>
#include <base/publish.h>

namespace aphid {

class aphidDocumentation : public base::Documentation
{
public:
    aphidDocumentation(QString name, Box *parent);
};

} //namespace

#endif
