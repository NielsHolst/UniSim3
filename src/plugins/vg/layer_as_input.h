/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef LAYER_AS_INPUT_H
#define LAYER_AS_INPUT_H
#include "layer.h"

namespace vg {

class LayerAsInput : public vg::Layer
{
public:
    LayerAsInput(QString name, base::Box *parent);
};

} //namespace
#endif
