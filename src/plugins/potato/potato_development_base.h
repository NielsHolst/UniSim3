/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Isaac Kwesi Abuley , Aarhus University [ikabuley at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef POTATO_DEVELOPMENT_BASE_H
#define POTATO_DEVELOPMENT_BASE_H
#include <base/box.h>

namespace potato {

class PotatoDevelopmentBase: public base::Box
{
public:
    PotatoDevelopmentBase(QString name, QObject *parent);
protected:
    // Inputs
    double timeStepDays;
    // Outputs
    double step,  total;
};

}
#endif
