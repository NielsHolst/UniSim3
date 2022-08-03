/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/convert.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "pollen_onset_date_from_maize_obs.h"

using namespace base;

namespace io {

PUBLISH(PollenOnsetDateFromMaizeObs)

PollenOnsetDateFromMaizeObs::PollenOnsetDateFromMaizeObs(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(obsFileName).help(
        "Name of file with maize phenology observations, "
        "a column-oriented text file with two colums, of which the first is ignored "
        "and the second contains the date when start-of-flowering was observed.");
}

void PollenOnsetDateFromMaizeObs::amend() {
    data = new DataGrid (environment().inputFileNamePath(obsFileName));
    for (int i = 0; i < data->rowNumber(); ++i) {
        QDate date = convert<QDate>(data->row(i).at(1));
        int year = date.year();
        if (!yearsFirstLine.contains(year)) {
            yearsFirstLine[year] = i;
        }
    }
    QList<int> years = yearsFirstLine.keys();
    lastYear = years.last();
}

void PollenOnsetDateFromMaizeObs::reset() {
    int curYear = currentYear();
    int ixMin = yearsFirstLine.value(curYear);
    int ixMax = (curYear == lastYear) ?
                (data->rowNumber() - 1) :
                (yearsFirstLine.value(curYear+1) - 1);
    if (ixMin >= ixMax)
        ThrowException("Calendar year not found in file with maize phenology data").value(curYear).context(this);
    int index = sampleInterval(ixMin, ixMax);
    Q_ASSERT(index>=0 && index<data->rowNumber());
    date = stringToValue<QDate>(data->row(index).at(1));
    dayOfYear = date.dayOfYear();
    year = date.year();
    Q_ASSERT(date.year() == curYear);
}

int PollenOnsetDateFromMaizeObs::sampleInterval(int from, int to) {
    typedef boost::random::uniform_int_distribution<> Distribution;
    typedef boost::variate_generator<UniSim::Random::Generator&, Distribution> Variate;
    Distribution distribution(from, to);
    Variate variate(*randomGenerator(), distribution);
    return variate();
}


} //namespace

