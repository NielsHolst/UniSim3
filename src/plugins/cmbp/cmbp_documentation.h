/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** William Meikle, USDA [william.meikle at usda.gov].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef CMBP_DOCUMENTATION_H
#define CMBP_DOCUMENTATION_H
#include <base/documentation.h>
#include <base/publish.h>

namespace cmbp {

class cmbpDocumentation : public base::Documentation
{
public:
    cmbpDocumentation(QString name, Box *parent);
};

} //namespace

#endif
