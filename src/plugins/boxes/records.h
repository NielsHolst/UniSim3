/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef RECORDS_H
#define RECORDS_H

#include <QDate>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QTime>
#include <QVector>
#include <base/box.h>
#include <base/environment.h>

namespace boxes {

class Records : public base::Box
{
public:
    Records(QString name, Box *parent);
    ~Records();
    // standard methods
    void amend();
    void initialize();
    void reset();
    void update();
    void cleanup();

private:
    // inputs
    QString fileName, dateColumnName, timeColumnName;
    QDateTime calendarDateTime;
    bool cycle, ignoreYear;

    // outputs
    QDateTime currentDateTime, nextDateTime, firstDateTime, lastDateTime;
    QDate currentDate, nextDate;
    QTime currentTime, nextTime;
    QVector<double> _values;

    // methods
    QString fileNamePath();
    void openFile();
    void readLineItems();
    void readColumnNames();
    void createColumnOutputs();
    void readToFirstLine();
    void advanceFirstLine();
    void advanceLine();
    void extractValues();
    void extrapolateValues();
    void advanceTime();
    QDateTime alignYear(QDateTime dt, int yearsToAdd) const;

    // data
    QFile _file;
    QStringList _lineItems, _columnNames;
    bool _pastLastLine, _foundYearDifference, _yearChanged;
    int _dateColumn, _timeColumn,
        _yearDifference;
    QVector<double> *_currentColumnValues, *_nextColumnValues;
};

}
#endif
