/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef AVERAGE_COVER_H
#define AVERAGE_COVER_H
#include "average_cover_or_screen.h"

namespace vg {

class AverageCover : public AverageCoverOrScreen
{
public:
    AverageCover(QString name, Box *parent);
    void amend();
    void reset();
private:
    // Input
    double groundArea;
    // Data
    QVector<Box*> _faces;
    double _areas[6], _Rfaces[6];
    QVector<const double*> _Uinsulations[6];
    // Methods
    void updateUbottomAdj();
    QStringList collectFacesByMaterial(QString material);
    QString pathToMaterialPorts(QString material, QString portName) const;
};

} //namespace

#endif
