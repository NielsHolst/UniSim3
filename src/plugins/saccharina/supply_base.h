/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SUPPLY_BASE_H
#define SUPPLY_BASE_H
#include <base/box.h>

namespace saccharina {

class SupplyBase : public base::Box
{
public:
    SupplyBase(QString name, Box *parent);
    void reset() final;
    void update() final;
protected:
    // Inputs
    double demand;
    // Outputs
    double value;
private:
    double sdRatio;
    // Methods
    virtual void updateValue() = 0;
};

}

#endif
