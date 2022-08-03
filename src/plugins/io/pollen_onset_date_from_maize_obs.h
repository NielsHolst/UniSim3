/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef POLLEN_ONSET_DATE_FROM_MAIZE_OBS_H
#define POLLEN_ONSET_DATE_FROM_MAIZE_OBS_H
#include <QDate>
#include <QMap>
#include <base/box.h>
#include <base/data_grid.h>

namespace io {

class PollenOnsetDateFromMaizeObs : public base::Box
{
public:
    PollenOnsetDateFromMaizeObs(QString name, QObject *parent);
    void amend();
    void reset();
private:
    // Inputs
    QString obsFileName;
    // Date
    base::DataGrid *data;
    QMap<int, int> yearsFirstLine;
    int lastYear;
    // Methods
    int sampleInterval(int from, int to);
};

} //namespace
#endif
