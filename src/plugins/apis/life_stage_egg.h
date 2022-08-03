/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Merijn Moens, Aarhus University [merijnmoens at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef LIFE_STAGE_EGG_H
#define LIFE_STAGE_EGG_H
#include <base/box.h>
#include <base/circular_buffer.h>

namespace apis {

class LifeStageEgg : public base::Box
{
public:
    LifeStageEgg(QString name, QObject *parent);
    void initialize();
    void reset();
    void update();
private:
    typedef QVector<double> Cohorts;
    // input
    int bufferSize;
    double timeStepDays, numberInit, numberNew;
    Cohorts Pmorph;
    // output
    Cohorts number, age;
    double numberSum,
        numberMorphed,
        numberLeaked;
    // data
    base::CircularBuffer<double> _number, _age;
    // methods
    void updateStatistics();
};

}

#endif
