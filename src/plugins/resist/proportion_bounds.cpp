/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Christian Nansen, University of California [chrnansen at ucdavis.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "proportion_bounds.h"
#include <QMessageBox>

using namespace base;

namespace resist {

PUBLISH(ProportionBounds)

ProportionBounds::ProportionBounds(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes bounds on a log scale for a proportion");
    Input(logP).help("Log10 of the average proportion");
    Input(logWidth).help("Width of log10 interval setting the bounds (>0)");
    Output(Pmin).help("Lower bound of interval");
    Output(Pmax).help("Upper bound of interval");
}

void ProportionBounds::initialize() {
    update();
//    QMessageBox::information(0, "ProportionBounds::initialize() " + objectName() , QString::number(Pmin) + " " + QString::number(Pmax));
}

void ProportionBounds::reset() {
    update();
//    QMessageBox::information(0, "ProportionBounds::reset() " + objectName() , QString::number(Pmin) + " " + QString::number(Pmax));
}

void ProportionBounds::update() {
//    Pmin = 0.01;
//    Pmax = 0.1;
    Pmin = pow(10., logP - logWidth/2.);
    Pmax = pow(10., logP + logWidth/2.);
    if (Pmin >= 1)
        Exception("Pmin must be less than 1").value(Pmin).context(this);
    if (Pmax > 1) Pmax = 1.;
}

}
