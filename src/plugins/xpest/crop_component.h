/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef CROP_H
#define CROP_H

#include <base/box.h>

namespace xpest {

class CropComponent : public base::Box
{
public:
    CropComponent(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double assimilate, assimilateLoss, belowGroundPartition,
        sla, initMass, senescence,
        remobilizationIn, remobilizationOut,
        partitioningCoefficient;
    bool isBelowGround;
    // Outputs
    double netPartition, mass, lai;
    // Methods
    void updateFromMass();
};

} //namespace


#endif
