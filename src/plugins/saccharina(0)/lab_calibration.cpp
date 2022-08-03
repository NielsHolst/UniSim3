/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "lab_calibration.h"

using namespace base;

namespace saccharina {

PUBLISH(LabCalibration)

LabCalibration::LabCalibration(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates calibration variables from lab data");
    Input(step).imports("sim[step]");
    Input(length).imports("area[length]");
    Input(startOfTreatment).equals(1).unit("h").help("Starting time of treatment");
    Output(r).unit("dm/dm/day").help("Relative growth rate in leaf length");
}

void LabCalibration::update() {
    if (step==startOfTreatment) {
        _initLength = length;
    }
    double days = (step-startOfTreatment)/24.;
    r = (days<=0.) ? 0. : log(length/_initLength)/days;
}

}
