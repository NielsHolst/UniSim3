/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_VECTOR_OP_H
#define BASE_VECTOR_OP_H
#include <QVector>

namespace base {
    class Node;
}

namespace vector_op {

typedef QVector<double> Vec;
typedef double Scalar;

// Unary
void add(Vec &v, const Scalar &x);
void add(Vec &v, const Vec &x, base::Node *context=nullptr);
void subtract(Vec &v, const Vec &x, base::Node *context=nullptr);
void subtract(Vec &v, const Scalar &x);
void multiply(Vec &v, const Vec &x, base::Node *context=nullptr);
void multiply(Vec &v, const Scalar &x);
void inverse(Vec &v, const Vec &x, base::Node *context=nullptr);

// Binary
void sum(Vec &v, const Vec &x, const Vec &y, base::Node *context=nullptr);
void sum(Vec &v, const Vec &x, const Scalar &y);
void difference(Vec &v, const Vec &x, const Vec &y, base::Node *context=nullptr);
void difference(Vec &v, const Vec &x, const Scalar &y);
void difference(Vec &v, const Scalar &x, const Vec &y);
void product(Vec &v, const Vec &x, const Vec &y, base::Node *context=nullptr);
void product(Vec &v, const Vec &x, const Scalar &y);

// Summary
double sum(const Vec &x);
double product(const Vec &x);
double average(const Vec &x);
double max(const Vec &x);
double min(const Vec &x);
int whichMax(const Vec &x);
int whichMin(const Vec &x);
double weightedAverage(const Vec &x, const Vec &weights, base::Node *context=nullptr);
double sumOfProducts(const Vec &x, const Vec &y, base::Node *context=nullptr);

} //namespace

#endif
