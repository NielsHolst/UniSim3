/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef PROPORTIONS_H
#define PROPORTIONS_H
#include <base/box.h>

namespace saccharina {

class Proportions : public base::Box
{
public:
    Proportions(QString name, Box *parent);
    void reset();
protected:
    // Inputs
    double concStructureC, concStructureN,
        concStoreC, concStoreN, concStoreP,
        concWholeCmax, concWholeNmax, concWholePmax;
    // Outputs
    double idealStructure, idealStoreC, idealStoreN, idealStoreP;
};

}

#endif
