/* Copyright 2012-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Clement N. Mweya, National Institute for Medical Research, Tukuyu, Tanzania [cmweya at nimr.or.tz].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "transmission_to_host.h"

using namespace base;

namespace rvf {
  
PUBLISH(TransmissionToHost)
  
TransmissionToHost::TransmissionToHost(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(infectiousAttacks).help("Number of infectious attacks");
    Input(allAttacks).help("Total number of attacks");
    Input(susceptibleHosts).help("Number of susceptible hosts");
    Input(allHosts).help("Total number of all hosts");
    Output(proportion).help("Proportion of susceptible hosts attacked by infectious vectors").unit("[0;1]");
}

void TransmissionToHost::reset() {
    update();
}

void TransmissionToHost::update() {
    proportion = (allAttacks==0. || allHosts==0.) ? 0. :
                 infectiousAttacks/allAttacks * susceptibleHosts/allHosts;
}

} //namespace

