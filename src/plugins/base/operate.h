/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef BASE_OPERATE_H
#define BASE_OPERATE_H

#include "value.h"
#include "base_dllexport.h"

// Define operations as functions
namespace operate {
       UNI_API base::Value add          (const base::Value &a, const base::Value &b);
//inline base::Value add          (const base::Value &a, const base::Value *b) { return add( a, *b); }
//inline base::Value add          (const base::Value *a, const base::Value *b) { return add(*a, *b); }
//inline base::Value add          (const base::Value *a, const base::Value &b) { return add(*a,  b); }

       UNI_API base::Value subtract     (const base::Value &a, const base::Value &b);
//inline base::Value subtract     (const base::Value *a, const base::Value &b) { return subtract(*a,  b); }
//inline base::Value subtract     (const base::Value &a, const base::Value *b) { return subtract( a, *b); }
//inline base::Value subtract     (const base::Value *a, const base::Value *b) { return subtract(*a, *b); }

       UNI_API base::Value multiply     (const base::Value &a, const base::Value &b);
//inline base::Value multiply     (const base::Value *a, const base::Value &b) { return multiply(*a,  b); }
//inline base::Value multiply     (const base::Value &a, const base::Value *b) { return multiply( a, *b); }
//inline base::Value multiply     (const base::Value *a, const base::Value *b) { return multiply(*a, *b); }

       UNI_API base::Value divide       (const base::Value &a, const base::Value &b);
//inline base::Value divide       (const base::Value *a, const base::Value &b) { return divide(*a,  b); }
//inline base::Value divide       (const base::Value &a, const base::Value *b) { return divide( a, *b); }
//inline base::Value divide       (const base::Value *a, const base::Value *b) { return divide(*a, *b); }

       UNI_API base::Value exponentiate (const base::Value &a, const base::Value &b);
//inline base::Value exponentiate (const base::Value *a, const base::Value &b) { return exponentiate(*a,  b); }
//inline base::Value exponentiate (const base::Value &a, const base::Value *b) { return exponentiate( a, *b); }
//inline base::Value exponentiate (const base::Value *a, const base::Value *b) { return exponentiate(*a, *b); }

       UNI_API base::Value larger       (const base::Value &a, const base::Value &b);
//inline base::Value larger       (const base::Value *a, const base::Value &b) { return larger(*a,  b); }
//inline base::Value larger       (const base::Value &a, const base::Value *b) { return larger( a, *b); }
//inline base::Value larger       (const base::Value *a, const base::Value *b) { return larger(*a, *b); }

       UNI_API base::Value largerOrEqual(const base::Value &a, const base::Value &b);
//inline base::Value largerOrEqual(const base::Value *a, const base::Value &b) { return largerOrEqual(*a,  b); }
//inline base::Value largerOrEqual(const base::Value &a, const base::Value *b) { return largerOrEqual( a, *b); }
//inline base::Value largerOrEqual(const base::Value *a, const base::Value *b) { return largerOrEqual(*a, *b); }

       UNI_API base::Value less         (const base::Value &a, const base::Value &b);
//inline base::Value less         (const base::Value *a, const base::Value &b) { return less(*a,  b); }
//inline base::Value less         (const base::Value &a, const base::Value *b) { return less( a, *b); }
//inline base::Value less         (const base::Value *a, const base::Value *b) { return less(*a, *b); }

       UNI_API base::Value lessOrEqual  (const base::Value &a, const base::Value &b);
//inline base::Value lessOrEqual  (const base::Value *a, const base::Value &b) { return lessOrEqual(*a,  b); }
//inline base::Value lessOrEqual  (const base::Value &a, const base::Value *b) { return lessOrEqual( a, *b); }
//inline base::Value lessOrEqual  (const base::Value *a, const base::Value *b) { return lessOrEqual(*a, *b); }

       UNI_API base::Value equal        (const base::Value &a, const base::Value &b);
//inline base::Value equal        (const base::Value *a, const base::Value &b) { return equal(*a,  b); }
//inline base::Value equal        (const base::Value &a, const base::Value *b) { return equal( a, *b); }
//inline base::Value equal        (const base::Value *a, const base::Value *b) { return equal(*a, *b); }

       UNI_API base::Value notEqual     (const base::Value &a, const base::Value &b);
//inline base::Value notEqual     (const base::Value *a, const base::Value &b) { return notEqual(*a,  b); }
//inline base::Value notEqual     (const base::Value &a, const base::Value *b) { return notEqual( a, *b); }
//inline base::Value notEqual     (const base::Value *a, const base::Value *b) { return notEqual(*a, *b); }

       UNI_API base::Value and_         (const base::Value &a, const base::Value &b);
//inline base::Value and_         (const base::Value *a, const base::Value &b) { return and_(*a,  b); }
//inline base::Value and_         (const base::Value &a, const base::Value *b) { return and_( a, *b); }
//inline base::Value and_         (const base::Value *a, const base::Value *b) { return and_(*a, *b); }

       UNI_API base::Value or_          (const base::Value &a, const base::Value &b);
//inline base::Value or_          (const base::Value *a, const base::Value &b) { return or_(*a,  b); }
//inline base::Value or_          (const base::Value &a, const base::Value *b) { return or_( a, *b); }
//inline base::Value or_          (const base::Value *a, const base::Value *b) { return or_(*a, *b); }

       UNI_API base::Value not_         (const base::Value &a);
//inline base::Value not_         (const base::Value *a) { return not_(*a); }

       UNI_API base::Value negate       (const base::Value &a);
//inline base::Value negate       (const base::Value *a) { return negate(*a); }
}

// Define operations as operators
UNI_API inline base::Value operator+(const base::Value &a, const base::Value &b) {
    return operate::add(a, b);
}

UNI_API inline base::Value operator-(const base::Value &a, const base::Value &b) {
    return operate::add(a, b);
}

UNI_API inline base::Value operator*(const base::Value &a, const base::Value &b) {
    return operate::multiply(a, b);
}

UNI_API inline base::Value operator/(const base::Value &a, const base::Value &b) {
    return operate::divide(a, b);
}

UNI_API inline base::Value operator&&(const base::Value &a, const base::Value &b) {
    return operate::and_(a, b);
}

UNI_API inline base::Value operator||(const base::Value &a, const base::Value &b) {
    return operate::or_(a, b);
}

UNI_API inline base::Value operator!(const base::Value &a) {
    return operate::not_(a);
}

UNI_API inline base::Value operator-(const base::Value &a) {
    return operate::negate(a);
}


#endif

