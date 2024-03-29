/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef ACTUATORS_H
#define ACTUATORS_H

#include <base/box.h>
#include <base/box_builder.h>

namespace vg {

class Actuators : public base::Box
{
public:
    Actuators(QString name, Box *parent);
    void amend();
private:
    void addLayer(base::BoxBuilder &builder, int i);
};
} //namespace


#endif
