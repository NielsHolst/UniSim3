/* Copyright 2016-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include "leaf_photo_degradation.h"
#include <cmath>
#include <base/data_grid.h>
#include <base/environment.h>
#include <base/interpolate.h>
#include <base/publish.h>

using namespace base;

namespace PestTox {

PUBLISH (LeafPhotoDegradation)
	
LeafPhotoDegradation::LeafPhotoDegradation(QString name, Box *parent)
    : LossRate(name, parent), _freeRadicalsTable(nullptr)
{
    help("manages fate of pesticide on crop or leaf surface due to photo-degradation");
    Input(dayOfYear).imports("calendar[dayOfYear]");
    Input(latitude).imports("calendar[latitude]");
    Input(fileName).help("File with table of (Julian day,latitude)-indexed free radicals concentration");
    Input(kOH).unit("cm3/molecules/h").help("Degradation rate");

    Output(OHconcentration).unit("molecules/cm3").help("Free OH radical concentration");
}

void LeafPhotoDegradation::initialize() {
    delete _freeRadicalsTable;
    QString filePath = environment().inputFileNamePath(fileName);
    _freeRadicalsTable = new DataGrid(filePath, this);

}

double LeafPhotoDegradation::computeInstantaneous() {
    OHconcentration = interpolate(*_freeRadicalsTable, latitude, dayOfYear)*1e5; // molecules/cm3
    return kOH*OHconcentration/3600.; // s-1 = cm3/molecules/h * molecules/cm3 / (s/h)
}

} //namespace

