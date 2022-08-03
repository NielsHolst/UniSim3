/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_OPERATE_H
#define BASE_OPERATE_H
#include "value.h"

// Define operations as functions
namespace operate {
       base::Value add          (const base::Value &a, const base::Value &b);
//inline base::Value add          (const base::Value &a, const base::Value *b) { return add( a, *b); }
//inline base::Value add          (const base::Value *a, const base::Value *b) { return add(*a, *b); }
//inline base::Value add          (const base::Value *a, const base::Value &b) { return add(*a,  b); }

       base::Value subtract     (const base::Value &a, const base::Value &b);
//inline base::Value subtract     (const base::Value *a, const base::Value &b) { return subtract(*a,  b); }
//inline base::Value subtract     (const base::Value &a, const base::Value *b) { return subtract( a, *b); }
//inline base::Value subtract     (const base::Value *a, const base::Value *b) { return subtract(*a, *b); }

       base::Value multiply     (const base::Value &a, const base::Value &b);
//inline base::Value multiply     (const base::Value *a, const base::Value &b) { return multiply(*a,  b); }
//inline base::Value multiply     (const base::Value &a, const base::Value *b) { return multiply( a, *b); }
//inline base::Value multiply     (const base::Value *a, const base::Value *b) { return multiply(*a, *b); }

       base::Value divide       (const base::Value &a, const base::Value &b);
//inline base::Value divide       (const base::Value *a, const base::Value &b) { return divide(*a,  b); }
//inline base::Value divide       (const base::Value &a, const base::Value *b) { return divide( a, *b); }
//inline base::Value divide       (const base::Value *a, const base::Value *b) { return divide(*a, *b); }

       base::Value exponentiate (const base::Value &a, const base::Value &b);
//inline base::Value exponentiate (const base::Value *a, const base::Value &b) { return exponentiate(*a,  b); }
//inline base::Value exponentiate (const base::Value &a, const base::Value *b) { return exponentiate( a, *b); }
//inline base::Value exponentiate (const base::Value *a, const base::Value *b) { return exponentiate(*a, *b); }

       base::Value larger       (const base::Value &a, const base::Value &b);
//inline base::Value larger       (const base::Value *a, const base::Value &b) { return larger(*a,  b); }
//inline base::Value larger       (const base::Value &a, const base::Value *b) { return larger( a, *b); }
//inline base::Value larger       (const base::Value *a, const base::Value *b) { return larger(*a, *b); }

       base::Value largerOrEqual(const base::Value &a, const base::Value &b);
//inline base::Value largerOrEqual(const base::Value *a, const base::Value &b) { return largerOrEqual(*a,  b); }
//inline base::Value largerOrEqual(const base::Value &a, const base::Value *b) { return largerOrEqual( a, *b); }
//inline base::Value largerOrEqual(const base::Value *a, const base::Value *b) { return largerOrEqual(*a, *b); }

       base::Value less         (const base::Value &a, const base::Value &b);
//inline base::Value less         (const base::Value *a, const base::Value &b) { return less(*a,  b); }
//inline base::Value less         (const base::Value &a, const base::Value *b) { return less( a, *b); }
//inline base::Value less         (const base::Value *a, const base::Value *b) { return less(*a, *b); }

       base::Value lessOrEqual  (const base::Value &a, const base::Value &b);
//inline base::Value lessOrEqual  (const base::Value *a, const base::Value &b) { return lessOrEqual(*a,  b); }
//inline base::Value lessOrEqual  (const base::Value &a, const base::Value *b) { return lessOrEqual( a, *b); }
//inline base::Value lessOrEqual  (const base::Value *a, const base::Value *b) { return lessOrEqual(*a, *b); }

       base::Value equal        (const base::Value &a, const base::Value &b);
//inline base::Value equal        (const base::Value *a, const base::Value &b) { return equal(*a,  b); }
//inline base::Value equal        (const base::Value &a, const base::Value *b) { return equal( a, *b); }
//inline base::Value equal        (const base::Value *a, const base::Value *b) { return equal(*a, *b); }

       base::Value notEqual     (const base::Value &a, const base::Value &b);
//inline base::Value notEqual     (const base::Value *a, const base::Value &b) { return notEqual(*a,  b); }
//inline base::Value notEqual     (const base::Value &a, const base::Value *b) { return notEqual( a, *b); }
//inline base::Value notEqual     (const base::Value *a, const base::Value *b) { return notEqual(*a, *b); }

       base::Value and_         (const base::Value &a, const base::Value &b);
//inline base::Value and_         (const base::Value *a, const base::Value &b) { return and_(*a,  b); }
//inline base::Value and_         (const base::Value &a, const base::Value *b) { return and_( a, *b); }
//inline base::Value and_         (const base::Value *a, const base::Value *b) { return and_(*a, *b); }

       base::Value or_          (const base::Value &a, const base::Value &b);
//inline base::Value or_          (const base::Value *a, const base::Value &b) { return or_(*a,  b); }
//inline base::Value or_          (const base::Value &a, const base::Value *b) { return or_( a, *b); }
//inline base::Value or_          (const base::Value *a, const base::Value *b) { return or_(*a, *b); }

       base::Value not_         (const base::Value &a);
//inline base::Value not_         (const base::Value *a) { return not_(*a); }

       base::Value negate       (const base::Value &a);
//inline base::Value negate       (const base::Value *a) { return negate(*a); }
}

// Define operations as operators
inline base::Value operator+(const base::Value &a, const base::Value &b) {
    return operate::add(a, b);
}

inline base::Value operator-(const base::Value &a, const base::Value &b) {
    return operate::add(a, b);
}

inline base::Value operator*(const base::Value &a, const base::Value &b) {
    return operate::multiply(a, b);
}

inline base::Value operator/(const base::Value &a, const base::Value &b) {
    return operate::divide(a, b);
}

inline base::Value operator&&(const base::Value &a, const base::Value &b) {
    return operate::and_(a, b);
}

inline base::Value operator||(const base::Value &a, const base::Value &b) {
    return operate::or_(a, b);
}

inline base::Value operator!(const base::Value &a) {
    return operate::not_(a);
}

inline base::Value operator-(const base::Value &a) {
    return operate::negate(a);
}


#endif

