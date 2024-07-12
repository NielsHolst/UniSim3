/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef FACE_H
#define FACE_H
#include <base/box.h>
#include "layer_parameters.h"

namespace vg {

class Face : public base::Box
{
public: 
    Face(QString name, Box *parent);
    void amend();
    void reset();
    const LayerParametersPtrs& coverParameters() const;
    const LayerParametersPtrs& screenParameters(int index) const;
    const LayerParametersPtrs& parameters(int index) const;
private:
    // Inputs
    QString cover, screens;
    double area, weight;
    // Outputs
    QVector<QString> screenMaterials;
    int numScreens;
    // Data
    LayerParametersPtrs _cover;
    QVector<LayerParametersPtrs> _screens;
};

} //namespace
#endif
