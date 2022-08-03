/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Kayla Sale, University of Arizona [kayla.sale at email.arizona.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/matrix.h>
#include <base/mega_factory.h>
#include <base/publish.h>
#include "big_bang.h"

using namespace base;

namespace pollnet {

PUBLISH(BigBang)

BigBang::BigBang(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(connectivityFile).help("Name of file with connectivity matrix");
}

void BigBang::amend() {
    // Read connectivity matrix
    Matrix<double> C;
    C.read(connectivityFile, Table::NoLabelling);

    // Create pollinator objects
    for (int i=0; i<C.numRow(); ++i) {
        QString name = "pollinator" + QString::number(i);
        pollinators << MegaFactory::create<>("pollnet::Pollinator", name, this);
    }

    // Create plant objects
    for (int j=0; j<C.numCol(); ++j) {
        QString name = "plant" + QString::number(j);
        plants << MegaFactory::create<>("pollnet::Plant", name, this);
    }
}


}
