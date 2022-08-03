/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef LEAF_PHOTO_DEGRADATION_H
#define LEAF_PHOTO_DEGRADATION_H

#include "loss_rate.h"

namespace base {
    class DataGrid;
}

namespace PestTox {

class LeafPhotoDegradation : public LossRate
{
public:
    LeafPhotoDegradation(QString name, Box *parent);
    void initialize();

private:
    // Input
    QString fileName;
    double dayOfYear, latitude, kOH;

    // Output
    double OHconcentration;

    // Data
    base::DataGrid *_freeRadicalsTable;

    // Methods
    double computeInstantaneous();
};

} //namespace


#endif
