/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef RUNNING_AVERAGE_TIMED_H
#define RUNNING_AVERAGE_TIMED_H
#include <QVector>
#include <base/box.h>

namespace boxes {

class RunningAverageTimed : public base::Box
{
public: 
    RunningAverageTimed(QString name, Box *parent);
    void reset();
    void update();
private:
    // Input
    double input, initial;
    double timeWindow, timeStepSecs;
    QString timeUnit;

    // Output
    double value;
    int count;
    // Data
    QVector<double> buffer;
    double sum;
    int pos;
};

} //namespace
#endif
