/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SENESCENCE_H
#define SENESCENCE_H
#include <base/box.h>

namespace coffee {

class Senescence : public base::Box
{
public: 
    Senescence(QString name, QObject *parent);
    void update();
private:
    // Inputs
    double ftccLmin, tccLmax, tccR,
        transpirationRatio, rootC, lai, leafN, leafC;
    // Outputs
    double dRootC, dLeafN, dLeafC, dLai;
};

} //namespace
#endif
