/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef CONTROLLED_ENVIRONMENT_BASE_H
#define CONTROLLED_ENVIRONMENT_BASE_H
#include <base/box.h>

namespace saccharina {

class ControlledEnvironmentBase : public base::Box
{
public:
    ControlledEnvironmentBase(QString name, QObject *parent);
    void reset() final;
    void update() final;
protected:
    // Inputs
    double temperature, irradiation, nitrogen, salinity, current,
        lightHours;
    // Outputs
    double T,I,N,S,U;
    // Methods
    int hour() const;
private:
    // Data
    int _hour;
    // Methods
    virtual void resetNitrogen() = 0;
    virtual void resetIrradiation() = 0;
    virtual void updateNitrogen() = 0;
    virtual void updateIrradiation() = 0;
};

}

#endif
