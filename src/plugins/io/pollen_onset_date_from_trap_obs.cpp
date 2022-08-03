/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/convert.h>
#include <base/data_grid.h>
#include <base/dialog.h>
#include <base/publish.h>
#include "pollen_onset_date_from_trap_obs.h"

using namespace base;

namespace io {

PUBLISH(PollenOnsetDateFromTrapObs)

PollenOnsetDateFromTrapObs::PollenOnsetDateFromTrapObs(QString name, QObject *parent)
    : PollenOnsetDateBase(name, parent)
{
    Input(fileName).equals("input/pollen-weather-Westerloge.txt").help( "Name of file with pollen trap catches");
}

void PollenOnsetDateFromTrapObs::reset() {
    dialog().information("reset A");
    DataGrid data(fileName, this);
    colDate = findColumn(data, "Date");
    colPollen = findColumn(data, "Pollen");
    updateFractile();

    dialog().information("reset B");
    bool inYear = false;
    for (int row = 0; row < data.rowNumber(); ++row) {
        dialog().information("reset C " + QString::number(row));
        inYear = inYear || data.cell<QDate>(row, colDate).year() == year;
        if (inYear) {
            double pollen = data.cell<double>(row, colPollen);
            if (pollen >= fractile) {
                date = data.cell<QDate>(row, colDate);
                dialog().information("reset Z1");
                return;
            }
        }
    }
    dialog().information("reset Z2");
    QString msg = "Could not find pollen fractile (%1) in calendar year (%2)";
    ThrowException(msg.arg(fractile).arg(year)).context(this);
}

int PollenOnsetDateFromTrapObs::findColumn(const DataGrid &data, QString colName)  {
    const QStringList &colNames(data.columnNames());
    int colIndex = colNames.indexOf( QRegExp(colName, Qt::CaseInsensitive) );
    if (colIndex == -1) {
        QString msg = "Could not find column named '%1' among colums: '%2'";
        ThrowException(msg.arg(colName).arg(colNames.join(','))).context(this);
    }
    return colIndex;
}

} //namespace

