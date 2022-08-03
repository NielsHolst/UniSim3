/* Copyright 2012-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Clement N. Mweya, National Institute for Medical Research, Tukuyu, Tanzania [cmweya at nimr.or.tz].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef MOSQUITO_FECUNDITY_H
#define MOSQUITO_FECUNDITY_H
#include <base/box.h>

namespace rvf {

class MosquitoFecundity : public base::Box
{
public:
    MosquitoFecundity(QString name, QObject *parent);
    void update();

private:
    // Inputs
    double numBloodMeals, eggsPerBloodMeal, propInfectedEggs;

    // Outputs
    double totalEggs, uninfectedEggs, infectedEggs;
};

} //namespace


#endif
