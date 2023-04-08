/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SIMULATION_H
#define SIMULATION_H
#include <QElapsedTimer>
#include <QList>
#include <QTime>
#include <base/box.h>

namespace base {
    class Port;
}

namespace boxes {

class Simulation : public base::Box
{
public:
    Simulation(QString name, Box *parent);
    void initialize();
    void reset();
    void cleanup();
    void debrief();
    void run();
private:
    // Inputs
    int iterations, steps;
    bool stopIteration, stopStep,
        silent, unattended;
    // Outputs
    int iteration, step, finalStep, executionTime;
    bool hasRun, hasError;
    QString errorMsg;
    // Methods
    void show(QElapsedTimer time);
    void updateConstness();
};

}

#endif
