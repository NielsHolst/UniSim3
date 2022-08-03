/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box.h>
#include <base/exception.h>
#include <base/publish.h>
#include <base/test_num.h>
#include <base/vector_op.h>
#include "distributed_delay.h"

using namespace base;

namespace boxes {

DistributedDelay::DistributedDelay(const Parameters &p_, Box *parent)
    : DistributedDelayBase(parent), p(p_)
//! Create distributed delay from parameters
{
    resize(p.k);
}

DistributedDelay::DistributedDelay(const DistributedDelay &dd)
    : DistributedDelayBase(dd._parent), p(dd.p), s(dd.s)
//! Create distributed delay as a copy of existing distributed delay
{
    _x = dd._x;
    _xSum = dd._xSum;
}

void DistributedDelay::resize(int k) {
    p.k = k;
    _x.resize(p.k);
}

void DistributedDelay::update(double inflow, double dt, double fgr) {
//! Update distributed delay
/*! Add inflow and update by time step (dt). Apply finite growth rate () by 'attrition' mechanism.
*/
    // To compute net change
    double totalBefore = _xSum + inflow;

    // To collect outflow
    s.outflowRate = 0;

    // When time is stopped just add the inflow
    if (dt==0.) {
        _x[0] += inflow;
    }
    else {
        // Set del and attrition according to Vansickle
        double  del = p.L*pow(fgr, -1./p.k),
                atr = p.k*(1./p.L - 1./del);		// atr <= 0 always

        // Calculate attrition factor
        double b = 1. + atr*del/p.k;

        // Divide time step to increase precision
        int idt = (int) floor(1.5 + 2.*b*dt*p.k/del);
        if (idt < p.minIter) idt = p.minIter;

//         Correct inflow for divided time step
//        double dividedInflow = inflow/idt;

        // Calculate flow coefficient
        double a;
        do {
            a = p.k/del*dt/idt;
            if (a<=1. || idt > 1000) break;
            idt *= 2;
        } while (true);
        if (!(0.<a && a<=1.)) {
            QString msg = "Illegal value for flow coefficient in DistributedDelay (a==%1). "
                    "Should be within ]0;1]. Other parameters: k=%2, del=%3, dt=%4, idt=%5, fgr=%6, L=%7";
            ThrowException(msg.arg(a).arg(p.k).arg(del).arg(dt).arg(idt).arg(fgr).arg(p.L)).context(_parent);
        }

        // Integrate
        for (int j = 0; j < idt; j++){
            // Collect outflow
            s.outflowRate += a*_x.at(p.k-1);
            // Step backwards through age classes
            for (int i = p.k-1; i > 0; i--)
                _x[i] += a*(_x.at(i-1) - b*_x.at(i));
            // Finish with first age class; enter inflow into that
//            x[0] += dividedInflow - a*b*x.at(0);
            _x[0] -= a*b*_x.at(0);
            if (j==0)
                _x[0] += inflow;
        }
    }
    _xSum = vector_op::sum(_x);
    s.growthRate = _xSum + s.outflowRate - totalBefore;
}

DistributedDelay::State DistributedDelay::state() const {
    return s;
}

} // namespace
