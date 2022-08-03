/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Kayla Sale, University of Arizona [kayla.sale at email.arizona.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "mediator.h"

using namespace base;

namespace pollnet {

PUBLISH(Mediator)

Mediator::Mediator(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(B0).equals(0.5).help("Half-saturation density for feeding on floral rewards.");
    Input(h).equals(1.2).help("Hill coefficient--dictates shape of functional response.");
    Input(connectivityFile).imports("network[connectivityFile]");
    Input(R).help("Biomass of floral rewards of plant population.");
    Input(V).help("Biomass of vegetative plant population.");
    Input(B).help("Biomass of pollinator population.");
    Input(x).help("Metabolic rate of pollinator.");
    Input(y).help("Max. consumption rate");
}

void Mediator::amend() {
    // Find plants and pollinators
    plants = findMany<Box*>("*<Plant>");
    pollinators = findMany<Box*>("*<Pollinator>");
    numPlants = plants.size();
    numPollinators = pollinators.size();

    // Allocate vectors and matrices
    R.resize(numPlants);
    V.resize(numPlants);
    x.resize(numPollinators);
    y.resize(numPollinators);
    B.resize(numPollinators);
    CR.resize(numPollinators, numPlants);
    Gain_Pollinator.resize(numPollinators);
    Loss_Floral.resize(numPlants);
    Poll_Benefit.resize(numPlants);

    // Create and connect outports ports
    setup_Consumption_Ports();
}

void Mediator::setup_Consumption_Ports() {
    // animal i, gains from eating
    for (int i = 0; i < numPollinators; ++i) {
        // Name Ports for Gain from Eating
        QString runningNumber = QString::number(i),
                gain_port_name = "gain"+runningNumber;
        NamedOutput(gain_port_name, Gain_Pollinator[i]);
        QString gain_port_path = fullName() + "[" + gain_port_name + "]";
        pollinators[i]->port("CR")->imports(gain_port_path);
    }

    // plant j, loss from being eaten & competition & gain from pollination
    for (int j = 0; j < numPlants; ++j) {
        // Name Ports for Loss from Being Eaten
        QString runningNumber = QString::number(j),
                loss_port_name = "loss"+runningNumber;
        NamedOutput(loss_port_name, Loss_Floral[j]);
        QString loss_port_path = fullName() + "[" + loss_port_name + "]";
        plants[j]->port("loss_floral")->imports(loss_port_path);

        // Name Ports for Total Vegetative Biomass (Vsum, Effect of Competition)
        QString vsum_port_name = "vsum" + runningNumber;
        NamedOutput(vsum_port_name, Vsum);
        QString vsum_port_path = fullName() + "[" + vsum_port_name + "]";
        plants[j]->port("Vsum")->imports(vsum_port_path);

        // Name Ports for Benefit from Pollination
        QString benefit_port_name = "benefit"+runningNumber;
        NamedOutput(benefit_port_name, Poll_Benefit[j]);
        QString benefit_port_path = fullName() + "[" + benefit_port_name + "]";
        plants[j]->port("reproductive_services")->imports(benefit_port_path);

    }

}

void Mediator::initialize() {
    // Read connectivity matrix
    preference.read(connectivityFile, Table::NoLabelling);
    if (preference.numRow() != numPollinators)
        ThrowException("Mismatch of row number and pollinator number").
                value(preference.numRow()).
                value2(numPollinators).
                context(this);
    if (preference.numCol() != numPlants)
        ThrowException("Mismatch of column number and plant number").
                value(preference.numCol()).
                value2(numPlants).
                context(this);
}

void Mediator::reset() {
    B0_h = pow(B0,h);
}

void Mediator::update() {
    // Compute consumption rate matrix CR
    for (int i = 0; i < numPollinators; ++i) {
        double Byx = B[i] * y[i] * x[i];
        for (int j = 0; j < numPlants; ++j) {
            double F = compute_F(i,j);
            CR(i,j) = F * Byx;
        }
    }

    // Compute Gain_Pollinator (sum consumption for across all plants for each pollinator)
    for (int i = 0; i < numPollinators; ++i) {
        double gainTemp = 0;
        for (int j = 0; j < numPlants; ++j) {
            gainTemp = gainTemp + CR(i,j);
        }
        Gain_Pollinator[i] = gainTemp;
    }

    // Compute Loss_Floral (sum consumption across all pollinators for each plant)
    for (int i = 0; i < numPlants; ++i) {
        double lossTemp = 0;
        for (int j = 0; j < numPollinators; ++j) {
            lossTemp = lossTemp + CR(j,i);
        }
        Loss_Floral[i] = lossTemp;
    }

    // Vsum (for competition calculation)
    Vsum = compute_Vsum();

    // Compute Quality Matrix
    base::Matrix<double> Quality;
    Quality.resize(numPollinators, numPlants);
    for (int i = 0; i < numPollinators; ++i) {
        for (int j = 0; j < numPlants; ++j) {
            if (Gain_Pollinator[i] > 0) {
                Quality(i,j) = CR(i,j)/Gain_Pollinator[i];
            }
            else {
                Quality(i,j) = 0;
            }

        }
    }

    // Compute Poll_Benefit (sum...)
   for (int j = 0; j < numPlants; ++j) {
       double benefitTemp = 0;
       for (int i = 0; i < numPollinators; ++i) {
           benefitTemp = benefitTemp + CR(i,j)*Quality(i,j);
       }
       Poll_Benefit[j] = benefitTemp;
   }
}

double Mediator::compute_F(int i, int j) {
    if (preference(i,j) == 0) return 0.;

    double F_numerator = preference(i,j) * pow(R[j],h);
    double F_denominator =  B0_h + F_numerator;
    return (R[j] <= 0) ? 0 : F_numerator/F_denominator;
}

double Mediator::compute_Vsum() {
    double Vtemp = 0;
    for (int i = 0; i < numPlants; ++i) {
        Vtemp = Vtemp + V[i];
    }
    return Vtemp;
}


}
