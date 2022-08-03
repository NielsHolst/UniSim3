/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/path.h>
#include <base/port.h>
#include <base/publish.h>
#include "vector_sum.h"

using namespace base;

namespace boxes {

PUBLISH(VectorSum)

VectorSum::VectorSum(QString name, Box *parent)
    : Box(name, parent)
{
    help("computes the vector sum of many vectors");
    Input(input).help("Path to vectors (must have same length)");
    Output(value).help("Sum; a vector of same length as the inputs");
}

void VectorSum::initialize() {
    _inputPtrs.clear();
    QVector<Port*> inputPorts = findMany<Port*>(input);
    for (Port *port : inputPorts)
        _inputPtrs << port->valuePtr< QVector<double> >();
}

void VectorSum::reset() {
    if (_inputPtrs.isEmpty())
        value.fill(0.,_size = 1);
    else {
        _size = _inputPtrs.at(0)->size();
        for (auto inputPtr : _inputPtrs) {
            if (inputPtr->size() != _size) {
                QString s{"%1 != %2"},
                        msg = s.arg(_size).arg(inputPtr->size());
                ThrowException("Input vectors must all have the same size").value(msg);
            }
        }
        value.fill(0.,_size);
        update();
    }
}

namespace {

    void addOne(QVector<double> *receiver, const QVector<double> *vec) {
        Q_ASSERT(receiver->size() == vec->size());
        double *p = receiver->data();
        const double *q = vec->data();
        int i = 0, n = vec->size();
        while (i++ < n)
            *p++ += *q++;
    }

    void add(QVector<double> *receiver, QList<const QVector<double>*> add) {
        for (const QVector<double> *vec : add)
            addOne(receiver, vec);
    }

}

void VectorSum::update() {
    add(&value, _inputPtrs);
}



}
