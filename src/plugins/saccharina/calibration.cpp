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
using namespace vector_op;

namespace saccharina {

PUBLISH(Calibration)

Calibration::Calibration(QString name, Box *parent)
    : Box(name, parent)
{
    help("calculates deviation from observations");
    Input(obsFileName).help("Name of file with observations");
    Input(date).imports("calendar[date]");
    Input(biomass).imports("biomass/whole[dw]");
    Input(concC).imports("biomass/whole[concC]");
    Input(concN).imports("biomass/whole[concN]");
    Input(concP).imports("biomass/whole[concP]");
    Output(value).help("Sum of squares");
}

void Calibration::initialize() {
    // Read obs
    DataFrame obs;
    QString fileNamePath = environment().inputFileNamePath(obsFileName);
    obs.read(fileNamePath, Table::ColumnLabelled);
    // Extract obs
    obsDates = obs.col<QDate>("Date");
    variable.biomass.obs = obs.col<double>("Wtotal");
    variable.concC.obs   = obs.col<double>("Cconc");
    variable.concN.obs   = obs.col<double>("Nconc");
    variable.concP.obs   = obs.col<double>("Pconc");
    // Standardise obs
    variable.biomass.standardizeObs();
    variable.concC.standardizeObs();
    variable.concN.standardizeObs();
    variable.concP.standardizeObs();
    // Allocate prediction data
    auto n = obsDates.size();
    variable.biomass.pred.resize(n);
    variable.concC.pred.resize(n);
    variable.concN.pred.resize(n);
    variable.concP.pred.resize(n);
}

double variance(const Vec &x) {
    int i = 0, n = x.size();
    double sum = 0., sumSq = 0.;
    const double *sender = x.data();
    while (i++ < n) {
        sumSq += (*sender)*(*sender);
        sum += *sender++;
    }
    return (sumSq - sum*sum/n)/(n-1);
}

double sd(const Vec &x) {
    return sqrt(variance(x));
}

void standardize(Vec &v, double icept, double scale) {
    int i = 0, n = v.size();
    double *receiver = v.data();
    while (i++ < n) {
        *receiver   -= icept;
        *receiver++ /= scale;
    }
}

void standardize(Vec &v) {
    return standardize(v, average(v), sd(v));
}

void Calibration::Data::standardizeObs() {
    obsAvg = average(obs);
    obsSd  = sd(obs);
    standardize(obs, obsAvg, obsSd);
}

void Calibration::Data::enterPrediction(int row, double value) {
    pred[row] = (value - obsAvg)/obsSd;
}

double Calibration::Data::sqDev(int row) const {
    double x = obs[row] - pred[row];
    return x*x;
}

void Calibration::reset() {
    currentRow = 0;
    value = 0.;
}

void Calibration::update() {
    // Collect predictions by obs date
    if (currentRow < obsDates.size() && date == obsDates.at(currentRow)) {
        variable.biomass.enterPrediction(currentRow, biomass);
        variable.concC.enterPrediction(  currentRow, concC);
        variable.concN.enterPrediction(  currentRow, concN);
        variable.concP.enterPrediction(  currentRow, concP);
        value += variable.biomass.sqDev(currentRow) +
                 variable.concC.sqDev(  currentRow) +
                 variable.concN.sqDev(  currentRow) +
                 variable.concP.sqDev(  currentRow);
        ++currentRow;
    }
}

}
