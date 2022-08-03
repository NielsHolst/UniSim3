/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Christian Nansen, University of California [chrnansen at ucdavis.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include <base/test_num.h>
#include <base/vector_op.h>
#include "reproduction.h"

using namespace base;

namespace resist {

PUBLISH(Reproduction)

Reproduction::Reproduction(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(N).help("Population size vector (ss, sr, rr)");
    Input(R).help("Net reproductive rate vector(ss, sr, rr) (per capita per generation)");
    Output(ss).help("Reproduction of ss genotype (per generation)");
    Output(sr).help("Reproduction of sr genotype (per generation)");
    Output(rr).help("Reproduction of rr genotype (per generation)");
    Output(values).help("Reproduction vector (ss, sr, rr)");
    values.resize(3);
}

void Reproduction::update() {
    QVector<double> m, p;
    // Compute offspring produced by each genotype
    vector_op::product(m, R, N, this);

    // Compute genotype proportions
    double Ntotal = vector_op::sum(N);
    if (Ntotal > 0)
        vector_op::product(p, N, 1./Ntotal);
    else
        p.fill(0., 3);

    // Compute offspring entering each genotype
    values[0] = ss = m[0]*(p[0] + p[1]/2) + m[1]*(p[0]/2 + p[1]/4);
    values[1] = sr = m[0]*(p[1]/2 + p[2]) + m[1]*(p[0]/2 + p[1]/2 + p[2]/2) + m[2]*(p[0] + p[1]/2);
    values[2] = rr = m[2]*(p[2] + p[1]/2) + m[1]*(p[2]/2 + p[1]/4);

    double sumIn  = vector_op::sum(m),
           sumOut = vector_op::sum(values);
    if (TestNum::ne(sumIn, sumOut))
        ThrowException("Reproduction error").value(sumIn).value2(sumOut).context(this);
//    offspring = c(
//      m[1]*(p[1] + p[2]/2) + m[2]*(p[1]/2 + p[2]/4),
//      m[1]*(p[2]/2 + p[3]) + m[2]*(p[1]/2 + p[2]/2 + p[3]/2) + m[3]*(p[1] + p[2]/2),
//      m[3]*(p[3] + p[2]/2) + m[2]*(p[3]/2 + p[2]/4)
//    )

}

}
