/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef BASE_VECTOR_OP_H
#define BASE_VECTOR_OP_H

#include <QVector>

#include "base_dllexport.h"

namespace base {
    class Node;
}

namespace vector_op {

typedef QVector<double> Vec;
typedef double Scalar;

// Unary
UNI_API void add(Vec &v, const Scalar &x);
UNI_API void add(Vec &v, const Vec &x, base::Node *context=nullptr);
UNI_API void subtract(Vec &v, const Vec &x, base::Node *context=nullptr);
UNI_API void subtract(Vec &v, const Scalar &x);
UNI_API void multiply(Vec &v, const Vec &x, base::Node *context=nullptr);
UNI_API void multiply(Vec &v, const Scalar &x);
UNI_API void inverse(Vec &v, const Vec &x, base::Node *context=nullptr);

// Binary
UNI_API void sum(Vec &v, const Vec &x, const Vec &y, base::Node *context=nullptr);
UNI_API void sum(Vec &v, const Vec &x, const Scalar &y);
UNI_API void difference(Vec &v, const Vec &x, const Vec &y, base::Node *context=nullptr);
UNI_API void difference(Vec &v, const Vec &x, const Scalar &y);
UNI_API void difference(Vec &v, const Scalar &x, const Vec &y);
UNI_API void product(Vec &v, const Vec &x, const Vec &y, base::Node *context=nullptr);
UNI_API void product(Vec &v, const Vec &x, const Scalar &y);
UNI_API bool eq(const Vec &x, const Vec &y, double feps = 1e-6, base::Node *context=nullptr);

// Summary
UNI_API double sum(const Vec &x);
UNI_API double product(const Vec &x);
UNI_API double average(const Vec &x);
UNI_API double max(const Vec &x);
UNI_API double min(const Vec &x);
UNI_API int whichMax(const Vec &x);
UNI_API int whichMin(const Vec &x);
UNI_API double weightedAverage(const Vec &x, const Vec &weights, base::Node *context=nullptr);
UNI_API double sumOfProducts(const Vec &x, const Vec &y, base::Node *context=nullptr);

UNI_API bool any(const QVector<bool> &x);

} //namespace

#endif
