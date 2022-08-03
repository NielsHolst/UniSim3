/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef PARASITE_H
#define PARASITE_H
#include <QVector>
#include <base/box.h>

namespace boxes {

class Parasite : public base::Box
{
public: 
    Parasite(QString name, Box *parent);
    void setFoodWebImports();
private:
    // Inputs
    double density, demand;
    // Outputs
    double supply, sdRatio;
};

} //namespace
#endif
