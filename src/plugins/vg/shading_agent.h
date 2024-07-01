/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef SHADING_AGENT_H
#define SHADING_AGENT_H
#include <base/box.h>


namespace vg {

class ShadingAgent : public base::Box
{
public: 
    ShadingAgent(QString name, Box *parent);
private:
    // Inputs
    double swReflectivity, lwReflectivity, state;
};

} //namespace
#endif
