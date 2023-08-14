/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <iostream>
#include <QtAlgorithms>
#include <QDir>
#include <QRegularExpression>
#include <QTime>
#include <base/bare_date.h>
#include <base/convert.h>
#include <base/exception.h>
#include <base/port.h>
#include <base/publish.h>
#include "records.h"

using namespace base;

namespace boxes {

PUBLISH(Records)

Records::Records(QString name, Box *parent)
    : Box(name, parent)
{
    help("reads inputs from records in a text file");
    Input(fileName).help("Name of file with records; columns separated by white space").equals("records.txt");
    Input(dateColumnName).help("Name of column with date").equals("Date");
    Input(timeColumnName).help("Name of column with time").equals("Time");
    Input(cycle).equals(false).help("Cycle back to start at end of file? Forces ignoreYear=true");
    Input(ignoreYear).equals(false).help("Ignore year when synchronising with the calendar");
    Input(calendarDateTime).imports("calendar[dateTime]");
    Output(firstDateTime).noClear().help("Date-time stamp of the first line in the file");
    Output(lastDateTime).noClear().help("Date-time stamp of the last line in the file");
    Output(currentDateTime).help("Date-time stamp of the current outputs");
    Output(nextDateTime).help("Date-time stamp of the next outputs");
    Output(currentDate).help("Date stamp of the current outputs");
    Output(nextDate).help("Date stamp of the next outputs");
    Output(currentTime).help("Time stamp of the current outputs");
    Output(nextTime).help("Time stamp of the next outputs");
    additionalOutputs("One output port is created for each column in the input file (`fileName`), except for the date and time columns");
    _currentColumnValues = new QVector<double>;
    _nextColumnValues = new QVector<double>;
}

void Records::amend() {
    // Allow imports defined in amend step
    evaluatePorts();
    // Inspect input file
    openFile();
    // Create outputs
    readColumnNames();
    createColumnOutputs();
    // Read first date time
    advanceFirstLine();
    // Finish file inspection
    _file.close();
}

Records::~Records() {
    delete _currentColumnValues;
    delete _nextColumnValues;
}

void Records::readColumnNames() {
    openFile();
    readLineItems();
    if (_pastLastLine)
        ThrowException("Records file is empty").value(fileNamePath()).context(this);

    _dateColumn = -1;
    _timeColumn = -1;
    _columnNames.clear();
    for (int i = 0; i < _lineItems.size(); ++i) {
        QString id = _lineItems[i];
        _columnNames.append(id);
        if (id == dateColumnName)
            _dateColumn = i;
        else if (id == timeColumnName)
            _timeColumn = i;
    }
}

void Records::openFile() {
    if (_file.isOpen())
        _file.close();
    _file.setFileName(fileNamePath());
    bool fileOk = _file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!fileOk)
        ThrowException("Cannot open records file").value(fileNamePath()).context(this);
    _pastLastLine = false;
}

QString Records::fileNamePath() {
    return environment().inputFileNamePath(fileName);
}

void Records::readLineItems() {
    QString line;
    while (!_file.atEnd() && line.isEmpty()) {
        line = QString(_file.readLine().simplified());
    }
    #if QT_VERSION >= 0x050E00
      _lineItems = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    #else
      _lineItems = line.split(QRegularExpression("\\s+"), QString::SkipEmptyParts);
    #endif
    _pastLastLine = _lineItems.isEmpty();
}

void Records::createColumnOutputs() {
    int n = _columnNames.size();
    _values.fill(0., n);
    _currentColumnValues->fill(0., n);
    _nextColumnValues->fill(0., n);
    // A clone just set the pointer for each port, the ports already exist!
    for (int i = 0; i < n; ++i) {
        QString name = _columnNames[i];
        if (i != _dateColumn && i != _timeColumn) {
            if (isCloned())
                port(name)->equals(&_values[i]);
            else
                NamedOutput(name, _values[i]);
        }
    }
}

void Records::initialize() {
    // Set lastDateTime
    bool saveCycle = cycle;
    cycle = false;
    openFile();
    readLineItems(); // skip labels
    advanceFirstLine();
    while (!_pastLastLine)
        advanceLine();
    lastDateTime = currentDateTime;
    _file.close();
    cycle = saveCycle;
    _foundYearDifference = false;
    _yearChanged = false;
}

void Records::reset() {
    initialize();
    readToFirstLine();

    auto calendar = findMaybeOne<Box*>("boxes::Calendar::*");
    if (calendar) {
        calendar->evaluatePorts();
        calendar->reset();
        evaluatePorts();
    }

    // Cycling also means ignoring year
    if (cycle)
        ignoreYear = true;

    // Update to reach first reading
    update();
}

void Records::readToFirstLine() {
    currentDateTime = firstDateTime;
    currentDate = nextDate = firstDateTime.date();
    currentTime = nextTime = firstDateTime.time();
    nextDateTime = currentDateTime;

    openFile();
    readLineItems(); // skip labels
    advanceFirstLine();
}

void Records::advanceFirstLine() {
    readLineItems();
    if (_pastLastLine)
        ThrowException("Records file is empty").value(fileNamePath()).context(this);
    extractValues();
    advanceTime();
    for (int i = 0; i < _nextColumnValues->size(); ++i)
        (*_currentColumnValues)[i] = _nextColumnValues->at(i);
    firstDateTime = currentDateTime;
}

void Records::advanceTime() {
    currentDate = nextDate;
    currentTime = nextTime;
    currentDateTime = nextDateTime;
}

void Records::extractValues() {
    if (!_pastLastLine && _lineItems.size() != _columnNames.size()) {
        QString s = "Number of items in records file (%1) does not match number of column names";
        ThrowException(s.arg(fileName)).value(_columnNames.join(";")).value2(_lineItems.join(";")).context(this);
    }
    try {
        for (int i = 0; i < _lineItems.size(); ++i) {
            if (i == _dateColumn)
                nextDate = convert<QDate>(_lineItems[_dateColumn]);
            else if (i == _timeColumn)
                nextTime = convert<QTime>(_lineItems[_timeColumn]);
            else
                (*_nextColumnValues)[i] = convert<double>(_lineItems[i]);
        }
        nextDateTime = QDateTime(nextDate, nextTime, Qt::UTC);
    }
    catch (Exception &ex) {
        ThrowException(ex.what()).context(this);
    }
}

void Records::extrapolateValues() {
    for (int i = 0; i < _nextColumnValues->size(); ++i)
        (*_nextColumnValues)[i] = _currentColumnValues->at(i);
}

void Records::update() {
    if (!_foundYearDifference) {
        if (ignoreYear) {
            while (!_pastLastLine) {
                BareDate nextDate     = BareDate(nextDateTime.date()),
                         calendarDate = BareDate(calendarDateTime.date());
                if (nextDate >= calendarDate)
                    break;
                advanceLine();
            }
            _yearDifference = calendarDateTime.date().year() - nextDateTime.date().year();
        }
        else {
            _yearDifference = 0;
        }
        _foundYearDifference = true;
    }

    while (!_pastLastLine && nextDateTime < alignYear(calendarDateTime, -_yearDifference)) {
        advanceLine();
    }

    for (int i = 0; i < _values.size(); ++i) {
        if (i == _dateColumn || i == _timeColumn) continue;
        auto currentAligned = alignYear(currentDateTime, _yearDifference - int(_yearChanged)),
             nextAligned    = alignYear(   nextDateTime, _yearDifference);
        double dx = static_cast<double>(currentAligned.secsTo(nextAligned)),
               dy = _nextColumnValues->at(i) - _currentColumnValues->at(i),
               x = static_cast<double>(currentAligned.secsTo(calendarDateTime));
        _values[i] = ((dx > 0) ? x*dy/dx : 0.) + _currentColumnValues->at(i);
    }
    _yearChanged = false;
}

QDateTime Records::alignYear(QDateTime dt, int yearsToAdd) const {
    return QDateTime(
                QDate(dt.date().year() + yearsToAdd, dt.date().month(), dt.date().day()),
                dt.time(),
                Qt::UTC);
}


void Records::advanceLine() {
    advanceTime();
    qSwap(_currentColumnValues, _nextColumnValues);
    readLineItems();

    if (_pastLastLine) {
        if (cycle) {
            _file.close();
            openFile();
            readLineItems();
            readLineItems();
            extractValues();
            ++_yearDifference;
            _yearChanged = true;
        }
        else {
            extrapolateValues();
        }
    }
    else {
        extractValues();
    }
}

void Records::cleanup() {
    _file.close();
}


} //namespace
