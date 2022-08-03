/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Merijn Moens, Aarhus University [merijnmoens at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/any_year.h>
#include <base/publish.h>
#include "nectar_flow.h"

using namespace base;

namespace apis {

PUBLISH(NectarFlow)

NectarFlow::NectarFlow(QString name, QObject *parent)
    : Box(name, parent)
{
    help("yields current nectar flow available from flowers");
    Input(minFlow).help("Minimum nectar flow (ml/h)");
    Input(maxFlow).help("Maximum nectar flow (ml/h)");
    Input(beginDate).equals(QDate(anyYear(), 1, 1));
    Input(endDate).equals(QDate(anyYear(), 12, 31));
    Input(beginTime).equals(QTime(0,0));
    Input(endTime).equals(QTime(0,0));
    Input(currentDateTime).imports("calendar[dateTime]");
    Input(timeStepSecs).imports("calendar[timeStepSecs]");
    Output(value).help("Nectar flow within this time steo (ml   )");
    Output(rate).help("Nectar flow rate (ml/h)");
}

void NectarFlow::reset() {
    rate = minFlow;
    updateValue();
}

void NectarFlow::update() {
    bool dateIsRight = isAfterOrEquals(currentDateTime.date(), beginDate) &&
                       isBeforeOrEquals(currentDateTime.date(), endDate),
         timeIsRight = currentDateTime.time() >= beginTime &&
                       (currentDateTime.time() <= endTime || endTime == QTime(0,0));
    rate = dateIsRight && timeIsRight ? maxFlow : minFlow;
    updateValue();
}

void NectarFlow::updateValue() {
    value = rate*timeStepSecs*3600.;
}

}
