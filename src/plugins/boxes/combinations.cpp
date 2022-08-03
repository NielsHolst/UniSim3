/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/dialog.h>
#include <base/exception.h>
#include <base/port.h>
#include <base/publish.h>
#include "combinations.h"

using namespace base;

namespace boxes {

PUBLISH(Combinations)

Combinations::Combinations(QString name, Box *parent)
    : Box(name, parent) {
    help("produces all combinations of child values");
    Output(iterations).noClear().help("Total number of combinations");
}

void Combinations::amend() {
    // Find child boxes with values port
    QVector<Box*> candidates = findMany<Box*>("./*");
    for (Box *candidate : candidates) {
        Port *port =  candidate->peakPort("values");
        if (port)
            children << candidate;
    }
    // Create a counter port for each child
    counters.resize(children.size());
    n.resize(children.size());
    int *countersPtr = counters.data();
    for (Box *child : children)
        (new Port(counterName(child), PortType::Output, this))->initialize(countersPtr++).noClear();
}

void Combinations::initialize() {
    // Fill counters
    iterations = 1;
    int i = 0;
    for (Box *child : children) {
        QVector<double> v = child->port("values")->value<QVector<double>>();
        counters[i] = 0;
        n[i] = v.size();
        iterations *= v.size();
        ++i;
    }
//    // Loop through all combinations
//    for (int j=0; j<N; ++j) {
//        for (int k=0; k<nSeq; ++k) {
//            if (i[k] < n[k]-1) {
//                ++(i[k]);
//                break;
//            }
//            else {
//                i[k] = 0;
//            }
//        }
//        QString s = QString::number(j) + " ";
//        for (int k=0; k<nSeq; ++k)
//            s += " " + QString::number(i[k]);
//        dialog().information(s);
//    }
}

QString Combinations::counterName(const Box *box) {
    return "counter_" + box->name();
}

void Combinations::cleanup() {
    // Tick counters one ahead
    for (int i=0; i < counters.size(); ++i) {
        if (counters.at(i) < n.at(i)-1) {
            ++(counters[i]);
            break;
        }
        else {
            counters[i] = 0;
        }
    }
}

} //namespace

