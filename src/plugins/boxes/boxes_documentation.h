/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BOXES_DOCUMENTATION_H
#define BOXES_DOCUMENTATION_H
#include <base/documentation.h>
#include <base/publish.h>

namespace boxes {

class boxesDocumentation : public base::Documentation
{
public:
    boxesDocumentation(QString name, Box *parent);
};

} //namespace

#endif
