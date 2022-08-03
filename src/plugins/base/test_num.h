/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_TEST_NUM_H
#define BASE_TEST_NUM_H

#include <float.h>
#include <cmath>

namespace base {
    class Node;
}

namespace TestNum {

// Fuzzy equality of one number with zero

inline bool eqZero(double n, double feps=1e-6) {
    return -feps<n && n<feps;
}

// Fuzzy comparison of two double numbers

inline bool eq(double n, double d, double feps = 1e-6) {
    if (n==0.)
        return eqZero(d, feps);
    if (d==0.)
        return eqZero(n, feps);
    if (fabs(d)<1 && fabs(n)>fabs(d)*DBL_MAX) // overflow
		return false;
    if (fabs(d)>1 && fabs(n)<fabs(d)*DBL_MIN) // underflow
        return false;
    return (1.-feps<n/d && n/d<1+feps) || fabs(n-d)<feps;
}

inline bool ne(double n, double d, double feps = 1e-6) {
    return !eq(n,d,feps);
}

inline bool lt(double n, double d, double feps = 1e-6) {
    return n<d && !eq(n,d,feps);
}

inline bool le(double n, double d, double feps = 1e-6) {
    return n<d || eq(n,d,feps);
}

inline bool gt(double n, double d, double feps = 1e-6) {
    return n>d && !eq(n,d,feps);
}

inline bool ge(double n, double d, double feps = 1e-6) {
    return n>d || eq(n,d,feps);
}

// Fuzzy comparison of one number with zero

inline bool neZero(double n, double feps = 1e-6) {
    return !eqZero(n,feps);
}

inline bool ltZero(double n, double feps = 1e-6) {
    return n<0. && !eqZero(n,feps);
}

inline bool leZero(double n, double feps = 1e-6) {
    return n<0. || eqZero(n,feps);
}

inline bool gtZero(double n, double feps = 1e-6) {
    return n>0. && !eqZero(n,feps);
}

inline bool geZero(double n, double feps = 1e-6) {
    return n>0. || eqZero(n,feps);
}

// Fuzzy rounding in zero neighbourhood

inline void snapToZero(double &n, double feps = 1e-6) {
    if (eqZero(n, feps))
        n = 0.;
}

void assureEqZero(double &n, const char *varName, base::Node *context, double feps = 1e-6);

void assureGeZero(double &n, const char *varName, base::Node *context, double feps = 1e-6);

void assureLeZero(double &n, const char *varName, base::Node *context, double feps = 1e-6);

// Fuzzy rounding in neighbourhood of constant

inline void snapTo(double &n, double d, double feps = 1e-6) {
    if (eq(n, d, feps))
        n = d;
}

void assureEq(double &n, double d, const char *varName, base::Node *context, double feps = 1e-6);

void assureGe(double &n, double d, const char *varName, base::Node *context, double feps = 1e-6);

void assureLe(double &n, double d, const char *varName, base::Node *context, double feps = 1e-6);

} // namespace

#endif

