/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef FOOD_WEB_H
#define FOOD_WEB_H
#include <base/box.h>

namespace boxes {

class FoodWeb : public base::Box
{
public: 
    FoodWeb(QString name, Box *parent);
    void amend();
private:
    // Inputs
    double timeStep;
    // Methods
    void setFoodWebImports();
};

} //namespace
#endif
