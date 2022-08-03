/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include <base/test_num.h>
#include <base/vector_op.h>
#include "split.h"

#include <base/dialog.h>
using namespace base;
using namespace TestNum;
using namespace vector_op;

namespace boxes {

PUBLISH(Split)

const int N = 5;

Split::Split(QString name, Box *parent)
    : Box(name, parent) {
    help("splits a number into proportional pieces (max. 5)");
    Input(whole).help("The number to be split");
    Input(proportions).help("Vector of proportions; sum<=1");
    Output(pieces).help("Vector holding proportions of whole; same length as proportions");
}

void Split::amend() {
    pieces.resize(N);
    for (int i=0; i<N; ++i) {
        QString number = QString::number(i),
                name = "piece_" + number;
        NamedOutput(name, pieces[i]).help("Holds values of piece " + number);
    }
}

void Split::initialize() {
    n = proportions.size();
    if (n>N)
        ThrowException("Max. 5 proportions allowed");
}

void Split::reset() {
    double s = sum(proportions);
    if (gt(s, 1.))
        ThrowException("Sum of proportions exceeds 1").value(s).context(this);
}

void Split::update() {
    for (int i=0; i<n; ++i)
        pieces[i] = whole*proportions.at(i);
}

} //namespace

