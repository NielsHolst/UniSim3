/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <boost/numeric/conversion/cast.hpp>
#include <QFile>
#include <QTextStream>
#include <base/computation.h>
#include <base/convert.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/path.h>
#include <base/port.h>
#include <base/publish.h>
#include <base/resolved_references.h>
#include <base/timer.h>
#include "simulation.h"

#include <base/dialog.h>
using namespace base;

namespace boxes {

PUBLISH(Simulation)

Simulation::Simulation(QString name, Box *parent)
    : Box(name, parent)
{
    help("runs a simulation");
    Input(iterations).equals(1).help("Number of iterations to run");
    Input(steps).computes("if exists(Calendar::*[steps]) then Calendar::*[steps] else 1");
    Input(stopIteration).equals(false).help("Stop running after this iteration?");
    Input(stopStep).equals(false).help("Stop running after this step?");
    Input(silent).equals(false).help("Run without scrolling progress messages?");
    Input(unattended).equals(false).help("An unattended run will finish without accessing clipboard");
    Output(iteration).noClear().help("Iteration number (1,2,...)");
    Output(step).noClear().help("Step number (0,1,2,...)");
    Output(finalStep).help("Last completed step in simulation");
    Output(executionTime).help("Duration of simulation run (ms)");
    Output(hasRun).help("Has this simulation been run?");
    Output(hasError).help("Did an error occur during simulation run?");
    Output(errorMsg).help("Error message");
}

void Simulation::initialize() {
    if (debug())
        dialog().information("Note: Debug is ON. Program will be slow");
    environment().isUnattended(unattended);
    iteration = 1;
    hasRun = false;
    // Increment file counter;
    // output files needed for this run must be open at this time
    environment().incrementFileCounter();
}

void Simulation::reset() {
    // Simulation is the last object reset
    // Setting step = 1 here means it will be 1 for the first update of descendants
    step = 1;
}

void Simulation::run() {
    dialog().resetProgress();
    environment().isSilent(silent);
    if (silent)
        dialog().information("Running silently...");
    hasError = false;
    QElapsedTimer time;
    try {
        time.start();
        ResolvedReferences::clear();
        Computation::changeStep(Computation::Step::Initialize);
        initializeFamily();
        ResolvedReferences::check();

        if (iterations < 1)
            ThrowException("Nothing to do; iterations < 1").value(iterations).context(this);

        iteration = 0;
        while (!stopIteration && iteration < iterations) {
            ++iteration;
            Computation::changeStep(Computation::Step::Reset);
            resetFamily();
            ResolvedReferences::check();
            updateConstnessFamily();
            Computation::changeStep(Computation::Step::Update);

            step = 0;
            while (!stopStep && step < steps) {
                ++step;
                if (Box::debug())
                    dialog().information("iteration: " + QString::number(iteration) +
                                         "step     : " + QString::number(step));
                show(time);
                updateFamily();
                ResolvedReferences::check();
            }
            Computation::changeStep(Computation::Step::Cleanup);
            cleanupFamily();
        }
        Computation::changeStep(Computation::Step::Debrief);
        debriefFamily();
    }
    catch (Exception &ex) {
        hasError = true;
        errorMsg = ex.what();
    }
    dialog().finishProgress();
    executionTime = boost::numeric_cast<int>(time.elapsed());
    hasRun = true;
    Computation::changeStep(Computation::Step::Ready);
}

void Simulation::show(QElapsedTimer time) {
    DialogBase::ProgressInfo info;
    info.time = time;
    info.step = step;
    info.steps = steps;
    info.iteration = iteration;
    info.iterations = iterations;
    dialog().updateProgress(info);
}

void Simulation::cleanup() {
    finalStep = step;
    step = 0;
}

void Simulation::debrief() {
    if (unattended)
        dialog().information("The simulation has finished in unattended mode. Write 'clip' at the prompt before pasting the outputs to R.");
    dialog().finishProgress();
}


}
