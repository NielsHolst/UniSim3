/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/data_frame.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/phys_math.h>
#include <base/publish.h>
#include <base/vector_op.h>
#include "calibration.h"

using namespace base;
using namespace phys_math;

namespace saccharina {

PUBLISH(Calibration)

Calibration::Calibration(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates deviation from observations");
    Input(obsFileName).help("Name of file with observations");
    Input(date).imports("calendar[date]");
    Input(area).imports("area[value]");
    Input(yield).imports("biomass[wetWeightYield]");
    Input(pctN).imports("biomass[nitrogenPct]");
    Input(pctC).imports("biomass[carbonPct]");
    NamedOutput("areaSumSq",  output.area.sumSq).help("Sum of squares of model residuals");
    NamedOutput("yieldSumSq", output.yield.sumSq).help("Sum of squares of model residuals");
    NamedOutput("pctNSumSq",  output.pctN.sumSq).help("Sum of squares of model residuals");
    NamedOutput("pctCSumSq",  output.pctC.sumSq).help("Sum of squares of model residuals");
}

void Calibration::reset() {
    // Check for observations
    noObservations = obsFileName.isEmpty();
    if (noObservations)
        return;
    // Clear buffers
    output.area.clear();
    output.yield.clear();
    output.pctN.clear();
    output.pctC.clear();
    // Read obs
    DataFrame obs;
    QString fileNamePath = environment().inputFileNamePath(obsFileName);
    obs.read(fileNamePath, Table::ColumnLabelled);
    // Extract columns
    dates = obs.col<QDate>("date");
    output.area.obs  = obs.col<double>("area");
    output.yield.obs = obs.col<double>("yield");
    output.pctN.obs  = obs.col<double>("nitrogenPct");
    output.pctC.obs  = obs.col<double>("carbonPct");
    // Set to first row
    row = 0;
}

void Calibration::update() {
    // Collect predictions by obs date
    if (!noObservations && row < dates.size() && date == dates.at(row)) {
        output.area.update(area);
        output.yield.update(yield);
        output.pctN .update(pctN);
        output.pctC .update(pctC);
        ++row;
    }
}

void Calibration::Output::clear() {
    obs.clear();
    pred.clear();
    sumSq = 0.;
}

void Calibration::Output::update(double predicted) {
    pred << predicted;
    sumSq = 0;
    int n = pred.size();
    for (int i=0; i<n; ++i) {
        // Skip missing values
        if (obs.at(i) >= 0.)
            sumSq += sqr(obs.at(i) - pred.at(i));
    }
    sumSq = log10(sumSq+0.01) + 2.;
}

}
