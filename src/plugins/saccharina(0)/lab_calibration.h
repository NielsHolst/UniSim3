/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef LAB_CALIBRATION_H
#define LAB_CALIBRATION_H
#include <base/box.h>

namespace saccharina {

class LabCalibration : public base::Box
{
public:
    LabCalibration(QString name, QObject *parent);
    void update();
private:
    // Inputs
    int step, startOfTreatment;
    double length;
    // Outputs
    double r;
    // Data
    double _initLength;
};

}

#endif
