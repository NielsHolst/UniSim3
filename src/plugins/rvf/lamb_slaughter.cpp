/* Copyright 2012-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Clement N. Mweya, National Institute for Medical Research, Tukuyu, Tanzania [cmweya at nimr.or.tz].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include "lamb_slaughter.h"

using namespace base;

namespace rvf {
  
PUBLISH(LambSlaughter)
  
LambSlaughter::LambSlaughter(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(maleMortality).help("Proportion of male lamb slaughtered").unit("[0;1]");
    Input(femaleMortality).help("Proportion of female lamb slaughtered").unit("[0;1]");
    Output(survival).help("Total lamb survival").unit("[0;1]");
    Output(sexRatio).help("Sex ratio of surviving lamb").unit("[0;1]");
}

void LambSlaughter::reset() {
    if (maleMortality + femaleMortality > 1.)
        ThrowException("Mortalities must add up to 1 at most")
                .value(maleMortality).value2(femaleMortality);
    update();
}

void LambSlaughter::update() {
    // Assuming 0.5 sex ratio of newborn lamb
    double survFemale = 1. - femaleMortality,
           survMale = 1. - maleMortality;
    sexRatio = survFemale/(survFemale + survMale);
    survival = 1. - (femaleMortality + maleMortality)/2.;
}

} //namespace

