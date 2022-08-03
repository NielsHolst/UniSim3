/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "exception.h"
#include "node.h"
#include "test_num.h"

using namespace base;

namespace TestNum {

void assureEqZero(double &n, const char *varName, Node *context, double feps) {
    if (eqZero(n, feps))
        n = 0.;
    if (n != 0.)
        ThrowException("Cannot round number to zero: " + QString(varName) + " = " + QString::number(n)).context(context);
}

void assureGeZero(double &n, const char *varName, Node *context, double feps) {
    if (geZero(n, feps) && n < 0.)
        n = 0.;
    if (n < 0)
        ThrowException("Cannot round number to zero: " + QString(varName) + " = " + QString::number(n)).context(context);
}

void assureLeZero(double &n, const char *varName, Node *context, double feps) {
    if (leZero(n, feps) && n > 0.)
        n = 0.;
    if (n > 0)
        ThrowException("Cannot round number to zero: " + QString(varName) + " = " + QString::number(n)).context(context);
}

void assureEq(double &n, double d, const char *varName, Node *context, double feps) {
    if (eq(n, d, feps))
        n = d;
    if (n != d)
        ThrowException("Cannot round number to constant: " + QString(varName) +
                                " = " + QString::number(n) +
                                " != " + QString::number(d)).context(context);
}

void assureGe(double &n, double d, const char *varName, Node *context, double feps) {
    if (ge(n, d, feps) && n < d)
        n = d;
    if (n < d)
        ThrowException("Cannot round number to constant: " + QString(varName) +
                                " = " + QString::number(n) +
                                " != " + QString::number(d)).context(context);
}

void assureLe(double &n, double d, const char *varName, Node *context, double feps) {
    if (le(n, d, feps) && n > d)
        n = d;
    if (n > d)
        ThrowException("Cannot round number to constant: " + QString(varName) +
                                " = " + QString::number(n) +
                                " != " + QString::number(d)).context(context);
}

} // namespace

