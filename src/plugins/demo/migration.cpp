#include <base/dialog.h>
#include <base/publish.h>
#include <base/vector_op.h>
#include "migration.h"

using namespace base;

namespace demo {

PUBLISH(Migration)

Migration::Migration(QString name, QObject *parent)
    : Box(name, parent)
{
}

void Migration::amend() {
    QString popPath = "islands/*/" + name();
    populations = findMany<Box*>(popPath);
    n = populations.size();

    distances.resize(n, n);
    emRates.resize(n, n);
    outcomes.resize(n);

    setupOutcomePorts();
}

void Migration::setupOutcomePorts() {
    // For each population
    int i = 0;
    for (Box *pop : populations) {
        // Create one output for emigration and one for immigration
        QString runningNumber = QString::number(i),
                emPortName = "em"+runningNumber,
                imPortName = "im"+runningNumber;
        NamedOutput(emPortName, outcomes[i].em);
        NamedOutput(imPortName, outcomes[i].im);
        ++i;
        // Construct paths to just created ports
        QString emPortPath = fullName() + "["+emPortName+"]",
                imPortPath = fullName() + "["+imPortName+"]";
        // Hook emigration and immigration outputs to population stage inputs
        Box *popStage = pop->findOne<Box*>("./*<Stage>");
        popStage->port("phaseOutflowProportion")->imports(emPortPath);
        popStage->port("phaseInflow")->imports(imPortPath);
    }
}

void Migration::reset() {
    setupDistances();
    dialog().information("distances=\n" + distances.toString());
    updateEmigration();
    dialog().information("rates=\n" + emRates.toString());
    updateImmigration();
}

void Migration::setupDistances() {
    for (int i=0; i<n; ++i) {
        for (int j=0; j<n; ++j) {
            Box *island1 = populations[i]->findOne<Box*>(".."),
                *island2 = populations[j]->findOne<Box*>("..");
            double dx = island1->port("longitude")->value<double>() -
                        island2->port("longitude")->value<double>();
            double dy = island1->port("latitude")->value<double>() -
                        island2->port("latitude")->value<double>();
            distances(i,j) = sqrt(dx*dx + dy*dy);
        }
    }
}

void Migration::update() {
    updateEmigration();
    updateImmigration();
}

void Migration::updateEmigration() {
    for (int sender=0; sender<n; ++sender) {
        double emigSlope = populations[sender]->
                           port("emigrationSlope")->
                           value<double>(),
               emigSum = 0;
        for (int receiver=0; receiver<n; ++receiver) {
            double emig = (sender==receiver) ?
                0 : computeEmigration( distances(sender,receiver), emigSlope );
            emRates(sender,receiver) = emig;
            emigSum += emig;
        }
        if (emigSum > 1) {
            ThrowException("Emigration rates from population exceeds one")
                    .value(populations[sender]->name())
                    .value2(emigSum)
                    .context(this);
        }
        outcomes[sender].em = emigSum;
    }
}

double Migration::computeEmigration(double distance, double slope) const {
    return 1./(1. + slope*distance);
}

void Migration::updateImmigration() {
    // Immigrants are exchanged as cohorts
    typedef QVector<double> Cohorts;
    // For each sender of emigrants
    for (int sender=0; sender<n; ++sender) {
        Box *popStage = populations[sender]->findOne<Box*>("./*<Stage>");
        Cohorts emig = popStage->port("phaseOutflow")->value<Cohorts>();
        // Split emigrants among receiving populations
        for (int receiver=0; receiver<n; ++receiver) {
            double proportionReceived = (outcomes[sender].em > 0.) ?
                emRates(sender,receiver)/outcomes[sender].em : 0.;
            Cohorts emigReceived;
            vector_op::product(emigReceived, emig, proportionReceived);
            if (sender == 0)
                outcomes[receiver].im = emigReceived;
            else
                vector_op::add(outcomes[receiver].im, emigReceived, this);
        }
    }
}


}
