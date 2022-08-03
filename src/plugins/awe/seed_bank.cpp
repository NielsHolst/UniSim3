/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <cmath>
#include <base/exception.h>
#include <base/interpolate.h>
#include <base/publish.h>
#include "seed_bank.h"

using namespace base;

namespace awe {

PUBLISH(SeedBank)

SeedBank::SeedBank(QString name, QObject *parent)
    : Box(name,parent)
{ 
    QVector<double> cal;
    cal << 0 << 0 << 5 << 10 << 20 << 40 << 10 << 5 << 0 << 0 << 0 << 0;
    Input(initial).equals(1000).help("Initial density of the seeds in the seed bank (seeds per m @Sup {2}");
    Input(emergenceByMonth).equals(cal);
            /*
                 "The calendar shows the relative emergence of seedlings per month in a scenario where no shade is "
                 "causing a reduction in emergence. The scale is relative and needs not "
                 "add up to a total of, e.g., 1 or 100. A daily calendar with values for @F dailyEmergenceRatioPotential "
                 "is constructed from this calendar and the @F {yearlyEmergenceRate} */
    Input(yearlyEmergence).equals(0.20); /*
          "The proportion [0..1] of the seed bank that would potentially emerge if no crop reduced emergence "
          "by shading */
    Input(yearlyMortality).equals(0.10); /*
          "The mortality [0..1] of the seed bank per year */
    Input(cropLaiExp).equals(0.04282); /*
          "Exponent @I a in the equation to calculate @F cropEffectOnEmergence, "
          "@Math{ y = exp(-ax sup{5 over 2}) }, "
          "where @I x is the leaf area index Ã‚Â½of the current @F {Crop} */
    Input(dormantInflow); /*
          "Use this to set the daily inflow of dormant seeds into the seed bank (seeds per m @Sup 2 per day). "
          "You cannot put non-dormant seeds into the seed bank */
    Input(instantMortality); /*
                 "The mortality (%) will be applied once in the next time step, before new seeds of @F dormantInflow "
                 "are added. Dormant and non-dormant seeds will be affected alike */
    Input(dayOfYear).imports("calendar[dayOfYear]");
    Input(cropLai).imports("field[lai]");

    Output(total); /*
                     "Total number of seeds (dormant + non-dormant) in the soil (seeds per m @Sup {2}) */
    Output(dormant); /*
                     "Number of dormant seeds in the soil (seeds per m @Sup {2}) */
    Output(nonDormant); /*
                     "Number of non-dormant seeds in the soil (seeds per m @Sup {2}) */
    Output(dailyEmergenceRatio); /*
                     "Ratio [0..1] of seedlings emerging in this time step (per day) */
    Output(totalEmergenceRatio); /*
                     "Accumulated ratio [0..1] of seedlings that have emerged since 1 January */
    Output(dailyEmergenceDensity); /*
                     "Density of seedlings emerging in this time step (seedlings per m @Sup 2 per day) */
    Output(totalEmergenceDensity); /*
                     "Accumulated density of seedlings emerged since 1 January (seedlings per m @Sup {2}) */
    Output(cropEffectOnEmergence); /*
                     "The effect [0..1] is a scaling factor applied to the @F dailyEmergenceRatioPotential to "
                     "achieve the realised @F {dailyEmergenceRatio} */
    Output(dailyEmergenceRatioPotential); /*
                     "Potential ratio [0..1] of seedlings emerging in this time step (per day). "
                     "The realised ratio @F dailyEmergenceRatio depends on the shading effect of the crop */
}

void SeedBank::initialize()
{
    dailySurvivalRate = pow(1. - yearlyMortality, 1./365.);
    initEmergenceCalendar();
    fitEmergence();
}

void SeedBank::reset()
{
    nonDormant = initial;
    dailyEmergenceRatio = totalEmergenceRatio =
    dailyEmergenceDensity = totalEmergenceDensity = 0;
    dormant = dormantInflow = 0.;
    instantMortality = 0.;
}

void SeedBank::update()
{
    applyInstantMortality();
    addInflow();
    dailyEmergenceRatioPotential = lookupEmergence(dayOfYear);
    cropEffectOnEmergence = calcCropEffectOnEmergence();
    dailyEmergenceRatio = dailyEmergenceRatioPotential*cropEffectOnEmergence;
    dailyEmergenceDensity = dailyEmergenceRatio*nonDormant;
    nonDormant -= dailyEmergenceDensity;
    nonDormant *= dailySurvivalRate;
    dormant *= dailySurvivalRate;

    if (dayOfYear == 1) {
        totalEmergenceRatio = dailyEmergenceRatio;
        totalEmergenceDensity = dailyEmergenceDensity;
    }
    else {
        totalEmergenceRatio += dailyEmergenceRatio;
        totalEmergenceDensity += dailyEmergenceDensity;
    }
    total = nonDormant + dormant;
}

void SeedBank::applyInstantMortality() {
    double survival = 1. - instantMortality/100.;
    if (survival < 1.) {
        nonDormant *= survival;
        dormant *= survival;
    }
}

void SeedBank::addInflow() {
    dormant += dormantInflow;
    if (dayOfYear == 1) {
        nonDormant += dormant;
        dormant = 0.;
    }
}

double SeedBank::calcCropEffectOnEmergence() const {
    return (cropLai==0) ? 1 : exp(-cropLaiExp*pow(cropLai, 2.5));
}

void SeedBank::initEmergenceCalendar() {
    const int m[14] = {-16,15,45,74,105,135,166,196,227,258,288,319,349,380};
    for (int i = 0; i < 12; ++i)
        emergenceCalendar[m[i+1]] = emergenceByMonth.at(i);
    emergenceCalendar[m[0]] = emergenceCalendar[m[12]];
    emergenceCalendar[m[13]] = emergenceCalendar[m[1]];
}

double SeedBank::lookupEmergence(int dayOfYear) const {
    double emergenceAtDay = interpolate(emergenceCalendar, dayOfYear);
    return emergenceScaling*emergenceAtDay;
}

void SeedBank::fitEmergence() {
    double sum = 0.;
    for (QMap<int,double>::iterator em = emergenceCalendar.begin();
                                    em != emergenceCalendar.end(); ++em) {
        sum += em.value();
    }
    if (sum == 0.) return;

    emergenceScaling = 1.;
    scaleEmergence(1./sum);

    double minScale = 1., maxScale = 1.;

    do {
        minScale /= 2.;
        emergenceScaling = minScale;
    } while (calcTotalEmergenceRatio() > yearlyEmergence);
    do {
        maxScale *= 2.;
        emergenceScaling = maxScale;
    } while (calcTotalEmergenceRatio() < yearlyEmergence);

    int it = 0;

    /*QFile file("C:/data/qdev/UniSim_0_2_2/applications/UniSim_sim/temp/fittest.txt");
    bool fileOk = file.open(QIODevice::WriteOnly | QIODevice::Text);
    Q_ASSERT(fileOk);*/

    double seekValue = yearlyEmergence;
    const double EPS = 1e-6;
    do {
        double midScale = (minScale + maxScale) / 2.;
        emergenceScaling = midScale;
        double midValue = calcTotalEmergenceRatio();
        if (fabs(midValue - seekValue) < EPS) break;

        /*_emergenceScaling = minScale;
        double minValue = totalEmergenceRatio();

        _emergenceScaling = maxScale;
        double maxValue = totalEmergenceRatio();

        QString s;
        QTextStream text(&s);
        text << "(" << minValue << " " << midValue << " " << maxValue << ")  "
             << "(" << minScale << " " << midScale << " " << maxScale<< ") \n";*/

        if (seekValue < midValue)
            maxScale = midScale;
        else
            minScale = midScale;

        // file.write(qPrintable(s));
    } while (it++ < 20);
}

void SeedBank::scaleEmergence(double factor) {
    for (QMap<int,double>::iterator em = emergenceCalendar.begin();
                                    em != emergenceCalendar.end(); ++em) {
        em.value() *= factor;
    }
}

double SeedBank::calcTotalEmergenceRatio() const {
    double density= 1, total = 0;
    for (int day = 1; day <= 365; ++day) {
        double dailyEmergenceRatio = lookupEmergence(day);
        total += dailyEmergenceRatio;
        density -= dailyEmergenceRatio;
        density *= dailySurvivalRate;
    }
    return total;
}


}
