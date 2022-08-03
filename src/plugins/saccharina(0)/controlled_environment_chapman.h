/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef CONTROLLED_ENVIRONMENT_CHAPMAN_H
#define CONTROLLED_ENVIRONMENT_CHAPMAN_H
#include "controlled_environment_base.h"

namespace saccharina {

class ControlledEnvironmentChapman : public ControlledEnvironmentBase
{
public:
    ControlledEnvironmentChapman(QString name, QObject *parent);
private:
    // Input
    double preIrradiation, preNitrogen;
    int startOfTreatment;
    // Methods
    void resetNitrogen();
    void resetIrradiation();
    void updateNitrogen();
    void updateIrradiation();
};

}

#endif
