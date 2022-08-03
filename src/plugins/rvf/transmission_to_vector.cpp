/* Copyright 2012-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Clement N. Mweya, National Institute for Medical Research, Tukuyu, Tanzania [cmweya at nimr.or.tz].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "transmission_to_vector.h"

using namespace base;

namespace rvf {
  
PUBLISH(TransmissionToVector)
  
TransmissionToVector::TransmissionToVector(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(infectiousHosts).help("Number of infectious hosts");
    Input(allHosts).help("Total number of all hosts");
    Input(propAttackersAttacked).help("Proportion of successful attackers").unit("[0;1]");
    Output(proportion).help("Proportion of vectors attacking infectious hosts").unit("[0;1]");
}

void TransmissionToVector::reset() {
    update();
}

void TransmissionToVector::update() {
    proportion = infectiousHosts/allHosts * propAttackersAttacked;
}

} //namespace

