/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box_builder.h>
#include <base/publish.h>
#include "budget_solver.h"

using namespace base;

namespace vg {

PUBLISH(BudgetSolver)

BudgetSolver::BudgetSolver(QString name, Box *parent)
    : Box(name, parent)
{
    help("solves energy and water budgets");
}

void BudgetSolver::amend() {
    BoxBuilder builder(this);
    builder.
    box("Layer").name("cover").
    endbox();
}
void BudgetSolver::initialize() {
    findShelterFaces();
    findCoverTypes();

    cover = findOne<Box*>("./cover");
    sw.layers
        << Budget::Layer(
               "cover",
               cover->port("swAbsorptivityTop")->valuePtr<double>(),
               cover->port("swReflectivityTop")->valuePtr<double>(),
               cover->port("swTransmissivityTop")->valuePtr<double>(),
               cover->port("swAbsorptivityBottom")->valuePtr<double>(),
               cover->port("swReflectivityBottom")->valuePtr<double>(),
               cover->port("swTransmissivityBottom")->valuePtr<double>(),
               cover->port("heatCapacity")->valuePtr<double>()
               );
    lw.layers
        << Budget::Layer(
               "cover",
               cover->port("lwAbsorptivityTop")->valuePtr<double>(),
               cover->port("lwReflectivityTop")->valuePtr<double>(),
               cover->port("lwTransmissivityTop")->valuePtr<double>(),
               cover->port("lwAbsorptivityBottom")->valuePtr<double>(),
               cover->port("lwReflectivityBottom")->valuePtr<double>(),
               cover->port("lwTransmissivityBottom")->valuePtr<double>(),
               cover->port("heatCapacity")->valuePtr<double>()
               );
}

void BudgetSolver::findShelterFaces() {
    roof1 = findOne<Box*>("roof1");
    roof2 = findOne<Box*>("roof2");
    side1 = findOne<Box*>("side1");
    side2 = findOne<Box*>("side2");
    end1  = findOne<Box*>("end1");
    end2  = findOne<Box*>("end2");
    geometry  = findOne<Box*>("construction/geometry");
    roof12Area = geometry->port("roofArea")->value<double>()/2;
    side12Area = geometry->port("sideArea")->value<double>()/2;
    end12Area  = geometry->port("endArea")->value<double>()/2;
    coverArea = 2*(roof12Area + side12Area + end12Area);
}

void BudgetSolver::findCoverTypes() {
    QVector<Box*> types = findMany<Box*>("shelter/covers/*");
    for (Box *type : types)
        coverTypes[type->objectName()] = type;
}

void BudgetSolver::reset() {
    updateCoverPort("swReflectivityTop").setConstness(true);
    updateCoverPort("swReflectivityBottom").setConstness(true);
    updateCoverPort("swTransmissivityTop").setConstness(true);
    updateCoverPort("swTransmissivityBottom").setConstness(true);
    updateCoverPort("lwReflectivityTop").setConstness(true);
    updateCoverPort("lwReflectivityBottom").setConstness(true);
    updateCoverPort("lwTransmissivityTop").setConstness(true);
    updateCoverPort("lwTransmissivityBottom").setConstness(true);
    updateCoverPort("Ubottom").setConstness(true);
    updateCoverPort("heatCapacity").setConstness(true);
    update();
}

void BudgetSolver::update() {
    updateCoverPort("Utop");
}

Port& BudgetSolver::updateCoverPort(QString portName) {
    Port *port = cover->port(portName);
    return port->equals(weightedCoverValue(portName)/coverArea);
}

double BudgetSolver::weightedCoverValue(QString portName) const {
    return
        roof12Area*(coverTypes.value("roof1")->port(portName)->value<double>() +
                    coverTypes.value("roof2")->port(portName)->value<double>()) +
        side12Area*(coverTypes.value("side1")->port(portName)->value<double>() +
                    coverTypes.value("side2")->port(portName)->value<double>()) +
        end12Area *(coverTypes.value("end1") ->port(portName)->value<double>() +
                    coverTypes.value("end2") ->port(portName)->value<double>());
}

} //namespace

