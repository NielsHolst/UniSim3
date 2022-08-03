/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef POPULATION_H
#define POPULATION_H
#include <base/box.h>
#include <base/circular_buffer.h>
#include <base/port.h>

namespace boxes {

class Population : public base::Box
{
public:
    Population(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double initial, ageIncrement;
    int bufferSize;
    double firstCohortGain;
    QVector<double> cohortsGain, cohortsLoss;
    // Outputs
    double lastCohortSpill;
    QVector<double> cohorts, age;
    // Data
    base::CircularBuffer<double> _cohorts, _age;
    QVector<base::Port*> _gains, _losses, _newBorns;
};

}

#endif
