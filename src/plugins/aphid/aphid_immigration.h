/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Stéphanie Saussure, Norwegian Institute of Bioeconomy Research [stephanie.saussure at nibio.no].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef APHID_IMMIGRATION_H
#define APHID_IMMIGRATION_H
#include <base/box.h>

namespace aphid {

class AphidImmigration : public base::Box
{
public:
    AphidImmigration(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double cropGrowthStage, fromCropGrowthStage, toCropGrowthStage,
           immigrationRate, propExposedImmigrants;
    int k;
    // Outputs
    double total, susceptible;
    QVector<double> exposed;
};

}

#endif
