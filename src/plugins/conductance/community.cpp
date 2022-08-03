/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/minimise.h>
#include <base/test_num.h>
#include <base/track.h>
#include <base/publish.h>
#include "community.h"
#include "plant.h"

using namespace base;

namespace conductance {

PUBLISH(Community)

Community::Community(QString name, QObject *parent)
    : Box(name, parent)
{
    help("models a community of one or two plants in the Conductance Model");
    Input(n).imports("./*[n]");
    Input(LA).imports("./*[LAPlant]");
    Output(time).help("Simulation progress (days)");
    Output(ZoneAreaTotal).help("Total crown zone area of the plant populations "
                        "(m^2 ground area owned per m^2 ground area available)");
}

void Community::initialize() {
    plants = findMany<Plant*>("./*");
	if (plants.isEmpty())
        ThrowException("Community has no plants");
	else if (plants.size() > 2)
        ThrowException("Max. 2 plants are allowed in community").value(plants.size());
}

void Community::reset() {
    phase = Unlimited;
    smaller = larger = 0;
    _nextTimeStep = 1.;
    time = 0.;

    ZoneAreaTotal = sumZoneArea();
    if (ZoneAreaTotal > 1)
        ThrowException("Total crown zone area must be < 1 m2 on day 0").value(ZoneAreaTotal);
}

struct g
{
  QObject *_context;

  g(QObject *context) : _context(context) {}

  double operator()(double x)
  {
      return (x-5)*(x-10);
  }
};


struct h {
    Community *_community;

    h(Community *community) : _community(community) {}

    double operator()(double timeStep) {
        double zoneAreaTotal = 0;
        for (Plant *plant : _community->plants) {
            plant->controlledUpdate(timeStep);
            zoneAreaTotal += plant->port("ZoneAreaTotal")->value<double>();
            plant->undoUpdate();
        }
        return fabs(zoneAreaTotal - 1.);
    }
};


void Community::update() {
    double timeStep = _nextTimeStep;
    updatePlants(timeStep);

    if (phase == Unlimited && ZoneAreaTotal < 1. && sumZoneArea() > 1) {
        undoUpdatePlants();

        QPair<double, double> minimum = base::minimise(h(this), 0., _nextTimeStep, 1e-6, 100, this);
        timeStep = minimum.first;

        updatePlants(timeStep);

        if (TestNum::ne(sumZoneArea(), 1.))
            ThrowException("Unexpected: ZoneAreaTotal != 1").value(ZoneAreaTotal).context(this);

        phaseUnlimitedChanged();
        _nextTimeStep = 1. - timeStep;
    }
    else if (phase == UnderCompression && plants.size() == 2) {
        bool plantsAreEven = smaller->port("LAI")->value<double>() >=
                             larger->port("LAI")->value<double>();
        if (plantsAreEven)
        {
            smaller->changePhase(Proportional);
            larger->changePhase(Proportional);
            phase = Proportional;
        }
        _nextTimeStep = 1.;
    }
    else if (phase == Proportional) {
        Q_ASSERT(plants.size() == 2);
        double LASum = LA[0]*n[0] + LA[1]*n[1];
        if (LASum > 0) {
            plants[0]->updateByRelativeLA(LA[0]/LASum);
            plants[1]->updateByRelativeLA(LA[1]/LASum);
        }
        _nextTimeStep = 1.;
    }

    time += timeStep;
    updateImports();
    ZoneAreaTotal = sumZoneArea();
}

void Community::updatePlants(double timeStep) {
    for (Plant *plant : plants)
        plant->controlledUpdate(timeStep);
}

void Community::undoUpdatePlants() {
    for (Plant *plant : plants)
        plant->undoUpdate();
}

double Community::sumZoneArea() {
    double sum = 0.;
    for (Plant *plant : plants)
        sum += plant->port("ZoneAreaTotal")->value<double>();
    return sum;
}

void Community::phaseUnlimitedChanged() {
        sortPlants();
        smaller->changePhase(UnderCompression);
        phase = UnderCompression;
}

void Community::sortPlants() {
    bool onlyOnePlant = plants.size() == 1;
    if (onlyOnePlant) {
        smaller = plants[0];
        larger = 0;
        return;
    }

    bool firstIsSmaller = plants[0]->port("LAI")->value<double>() <
                          plants[1]->port("LAI")->value<double>();
    if (firstIsSmaller) {
        smaller = plants[0];
        larger = plants[1];
        // Swap so that larger gets updated first
        plants[0] = larger;
        plants[1] = smaller;
    }
    else {
        smaller = plants[1];
        larger = plants[0];
    }
}

bool Community::phaseUnderCompressionChanged() {
    return false;
}


} //namespace

