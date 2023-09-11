/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef AVERAGE_SCREEN_H
#define AVERAGE_SCREEN_H
#include "average_all_materials_in_layer.h"

namespace base {
 class BoxBuilder;
}

namespace vg {

class AverageScreen : public AverageAllMaterialsInLayer
{
public: 
    AverageScreen(QString name, Box *parent);
    void amend();
    void reset();
    void update();
private:
    // Output
    double effectiveArea;
    // Data
    QVector<Box*> _faces;
    QVector<double> _areas;
    QVector<const double *> _states;
    // Methods
    void updateUbottomAdj();
    QStringList collectFacesByMaterial(QString material);
    QString pathToMaterialPorts(QString material, QString portName) const;

    int getNumberFromName() const;
    void buildAverageMaterialNone(base::BoxBuilder &builder);
};

} //namespace
#endif
