/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "exception.h"
#include "vector_op.h"

#define CHECK_SIZE(func, X) \
    if (X.size() != n) ThrowException(QString("In ") + func + ": Vectors must be of equal size"). \
                       value2(X.size()).value1(n).context(context_)

using namespace base;

namespace vector_op {

//
// Unary
//
void add(Vec &v, const Vec &x, Node *context_) {
    int i = 0, n = v.size();
    CHECK_SIZE("add", x);
    double *receiver = v.data();
    const double *sender = x.data();
    while (i++ < n)
        *receiver++ += *sender++;
}

void add(Vec &v, const Scalar &x) {
    int i = 0, n = v.size();
    double *receiver = v.data();
    while (i++ < n)
        *receiver++ += x;
}

void subtract(Vec &v, const Vec &x, Node *context_) {
    int i = 0, n = v.size();
    CHECK_SIZE("subtract", x);
    double *receiver = v.data();
    const double *sender = x.data();
    while (i++ < n)
        *receiver++ -= *sender++;
}

void subtract(Vec &v, const Scalar &x) {
    int i = 0, n = v.size();
    double *receiver = v.data();
    while (i++ < n)
        *receiver++ -= x;
}

void multiply(Vec &v, const Vec &x, Node *context_) {
    int i = 0, n = v.size();
    CHECK_SIZE("multiply", x);
    double *receiver = v.data();
    const double *sender = x.data();
    while (i++ < n)
        *receiver++ *= *sender++;
}

void multiply(Vec &v, const Scalar &x) {
    int i = 0, n = v.size();
    double *receiver = v.data();
    while (i++ < n)
        *receiver++ *= x;
}

void inverse(Vec &v, const Vec &x, Node *context_) {
    int i = 0, n = v.size();
    CHECK_SIZE("inverse", x);
    double *receiver = v.data();
    const double *sender = x.data();
    while (i++ < n) {
        *receiver++ = 1./(*sender);
        sender++;
    }
}


//
// Binary
//

void sum(Vec &v, const Vec &x, const Vec &y, Node *context_) {
    int i = 0, n = x.size();
    CHECK_SIZE("sum", y);
    v.resize(n);
    double *receiver = v.data();
    const double *sender1 = x.data();
    const double *sender2 = y.data();
    while (i++ < n)
        *receiver++ = *sender1++ + *sender2++;
}

void sum(Vec &v, const Vec &x, const Scalar &y) {
    int i = 0, n = x.size();
    v.resize(n);
    double *receiver = v.data();
    const double *sender = x.data();
    while (i++ < n)
        *receiver++ = *sender++ + y;
}

void difference(Vec &v, const Vec &x, const Vec &y, Node *context_) {
    int i = 0, n = x.size();
    CHECK_SIZE("difference", y);
    v.resize(n);
    double *receiver = v.data();
    const double *sender1 = x.data();
    const double *sender2 = y.data();
    while (i++ < n)
        *receiver++ = *sender1++ - *sender2++;
}

void difference(Vec &v, const Vec &x, const Scalar &y) {
    int i = 0, n = x.size();
    v.resize(n);
    double *receiver = v.data();
    const double *sender = x.data();
    while (i++ < n)
        *receiver++ = *sender++ - y;
}

void difference(Vec &v, const Scalar &x, const Vec &y) {
    int i = 0, n = y.size();
    v.resize(n);
    double *receiver = v.data();
    const double *sender = y.data();
    while (i++ < n)
        *receiver++ = x - *sender++;
}

void product(Vec &v, const Vec &x, const Vec &y, Node *context_) {
    int i = 0, n = x.size();
    CHECK_SIZE("product", y);
    v.resize(n);
    double *receiver = v.data();
    const double *sender1 = x.data();
    const double *sender2 = y.data();
    while (i++ < n)
        *receiver++ = *sender1++ * *sender2++;
}

void product(Vec &v, const Vec &x, const Scalar &y) {
    int i = 0, n = x.size();
    v.resize(n);
    double *receiver = v.data();
    const double *sender = x.data();
    while (i++ < n)
        *receiver++ = *sender++ * y;
}

//
// Summary
//

double sum(const Vec &x) {
    int i = 0, n = x.size();
    double receiver = 0.;
    const double *sender = x.data();
    while (i++ < n)
        receiver += *sender++;
    return receiver;
}

double product(const Vec &x) {
    int i = 0, n = x.size();
    if (n==0) return 0.;
    double receiver = 1.;
    const double *sender = x.data();
    while (i++ < n)
        receiver *= *sender++;
    return receiver;
}

double average(const Vec &x) {
    int n = x.size();
    return (n==0) ? 0 : sum(x)/n;
}

double max(const Vec &x) {
    int i = 1, n = x.size();
    const double *sender = x.data();
    double receiver = *sender;
    ++sender;
    while (i++ < n) {
        double v = *sender++;
        if (v > receiver) receiver = v;
    }
    return receiver;
}

double min(const Vec &x) {
    int i = 1, n = x.size();
    const double *sender = x.data();
    double receiver = *sender;
    ++sender;
    while (i++ < n) {
        double v = *sender++;
        if (v < receiver) receiver = v;
    }
    return receiver;
}

int whichMax(const Vec &x) {
    int i = 1, which = 0, n = x.size();
    const double *sender = x.data();
    double receiver = *sender;
    ++sender;
    while (i++ < n) {
        double v = *sender++;
        if (v > receiver) {
            receiver = v;
            which = i;
        }
    }
    return which;
}

int whichMin(const Vec &x) {
    int i = 1, which = 0, n = x.size();
    const double *sender = x.data();
    double receiver = *sender;
    ++sender;
    while (i++ < n) {
        double v = *sender++;
        if (v < receiver) {
            receiver = v;
            which = i;
        }
    }
    return which;
}


double weightedAverage(const Vec &x, const Vec &weights, Node *context_) {
    int i = 0, n = x.size();
    CHECK_SIZE("weightedAverage", weights);
    double weightedSum = 0, sumOfWeights = 0;
    const double *sender1 = x.data();
    const double *sender2 = weights.data();
    while (i++ < n) {
        sumOfWeights += *sender2;
        weightedSum += *sender1++ * *sender2++;
    }
    return (sumOfWeights == 0.) ? 0. : weightedSum/sumOfWeights;
}

double sumOfProducts(const Vec &x, const Vec &y, Node *context_) {
    int i = 0, n = x.size();
    CHECK_SIZE("sumOfProducts",y);
    double receiver = 0;
    const double *sender1 = x.data();
    const double *sender2 = y.data();
    while (i++ < n)
        receiver += *sender1++ * *sender2++;
    return receiver;
}

} // namespace
