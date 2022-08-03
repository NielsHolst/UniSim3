/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef FOLIAGE_H
#define FOLIAGE_H
#include <base/box.h>

namespace coffee {

class Foliage : public base::Box
{
public: 
    Foliage(QString name, QObject *parent);
    void update();
private:
    // Inputs
    double slaMax, fSlaMin,
        transpirationRatio, gLeafC;
    // Outputs
    double sla, gLai;
};

} //namespace
#endif
