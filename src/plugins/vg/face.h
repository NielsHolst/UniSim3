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
    void update();
    const LayerParameters* parameters(int index) const;
private:
    // Inputs
    QString cover, screens;
    double area,
        sunElevation, sunAzimuth, sunDiffuseRadiation,
        faceSlope, faceAzimuth;
    // Outputs
    QVector<QString> screenMaterials;
    int numScreens;
    double aoi, swWeight, lwWeight;
    // Data
    const LayerParameters *_cover;
    QVector<const LayerParameters*> _screens;
};

} //namespace
#endif
