/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef POLLEN_ONSET_DATE_FROM_TRAP_OBS
#define POLLEN_ONSET_DATE_FROM_TRAP_OBS
#include "pollen_onset_date_base.h"

namespace base {
    class DataGrid;
}

namespace io {

class PollenOnsetDateFromTrapObs : public PollenOnsetDateBase
{
public:
    PollenOnsetDateFromTrapObs(QString name, QObject *parent);
    // standard methods
    void reset();
private:
    // Inputs
    QString fileName;
    // data
    int colDate, colPollen;
    // methods
    int findColumn(const base::DataGrid &data, QString colName);
};


} //namespace
#endif
