/* Copyright 2012-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Clement N. Mweya, National Institute for Medical Research, Tukuyu, Tanzania [cmweya at nimr.or.tz].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "mosquito_fecundity.h"

using namespace base;

namespace rvf {

PUBLISH(MosquitoFecundity)

MosquitoFecundity::MosquitoFecundity(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(numBloodMeals).help("Number of blood meals acquired");
    Input(eggsPerBloodMeal).equals(40.).help("Number of eggs laid per blood meal");
    Input(propInfectedEggs).help("Proportion infected eggs laid").unit("[0;1]");
    Output(totalEggs).help("Total no. of infected eggs laid");
    Output(infectedEggs).help("No. of infected eggs laid");
    Output(uninfectedEggs).help("No. of uninfected eggs laid");
}

void MosquitoFecundity::update() {
    totalEggs = numBloodMeals*eggsPerBloodMeal;
    uninfectedEggs = totalEggs*(1. - propInfectedEggs);
    infectedEggs = totalEggs*propInfectedEggs;
}

} //namespace
