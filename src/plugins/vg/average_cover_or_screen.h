/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef AVERAGE_COVER_OR_SCREEN_H
#define AVERAGE_COVER_OR_SCREEN_H
#include "layer_adjusted.h"

namespace base {
    class BoxBuilder;
}

namespace vg {

class AverageCoverOrScreen : public LayerAdjusted
{
public:
    AverageCoverOrScreen(QString name, Box *parent);
    void reset() final;
    void update() final ;
private:
    // Inputs
    QVector<double>
        swAbsorptivityTops,   swAbsorptivityBottoms,
        swReflectivityTops,   swReflectivityBottoms,
        swTransmissivityTops, swTransmissivityBottoms,
        lwAbsorptivityTops,   lwAbsorptivityBottoms,
        lwReflectivityTops,   lwReflectivityBottoms,
        lwTransmissivityTops, lwTransmissivityBottoms,
        Utops, Ubottoms,
        heatCapacitys;
    virtual QStringList collectFacesByMaterial(QString material) = 0;
    virtual QString pathToMaterialPorts(QString material, QString portName) const = 0;
protected:
    // Methods
    QString pathToFaces(QStringList faces, QString port) const;
    void buildAverageMaterial(base::BoxBuilder &builder, QString material);
};

} //namespace

#endif
