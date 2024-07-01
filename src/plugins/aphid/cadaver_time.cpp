/* Copyright 2018-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Stéphanie Saussure, Norwegian Institute of Bioeconomy Research [stephanie.saussure at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include "cadaver_time.h"
#include <base/vector_op.h>

using vector_op::sum;
using namespace base;

namespace aphid {

PUBLISH(CadaverTime)

CadaverTime::CadaverTime(QString name, Box *parent)
    : Box(name, parent)
{
    help("computes day-degree time step accelerated by high RH");
    Input(isSporulating).help("Are cadavers sporulating?").unit("boolean");
    Input(timeStep).help("Time step $(\\tau)$").unit("DD");
    Input(rhAccelaration).help("Acceleration factor above RH threshold $(h)$").unit("-");
    Output(step).help("RH-corrected time step $(\\tau_{corrected})$").unit("DD");
    Output(total).help("Accumulated RH-corrected time steps since reset").unit("DD");
}

void CadaverTime::update() {
    step = isSporulating ? rhAccelaration*timeStep : timeStep;
    total += step;
}

}


