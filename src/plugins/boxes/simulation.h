/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef SIMULATION_H
#define SIMULATION_H

#include <QElapsedTimer>
#include <QList>
#include <QTime>

#include <base/box.h>
#include <base/base_dllexport.h>

namespace base {
    class Port;
}

namespace boxes {

class UNI_API Simulation : public base::Box
{
public:
    Simulation(QString name, Box *parent);
    void initialize() override;
    void reset() override;
    void cleanup() override;
    void debrief() override;
    void run() override;
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
