/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Merijn Moens, Aarhus University [merijnmoens at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef NECTAR_FLOW_H
#define NECTAR_FLOW_H
#include <QDate>
#include <QTime>
#include <base/box.h>

namespace apis {

class NectarFlow : public base::Box
{
public:
    NectarFlow(QString name, QObject *parent);
    void reset();
    void update();
private:
    // input
    double minFlow, maxFlow;
    QDate beginDate, endDate;
    QTime beginTime, endTime;
    QDateTime currentDateTime;
    double timeStepSecs;
    // output
    double value, rate;
    // methods
    void updateValue();
};

}

#endif
