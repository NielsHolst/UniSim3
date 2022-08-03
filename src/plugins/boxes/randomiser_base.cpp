/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/environment.h>
#include <base/random_generator.h>
#include <base/random_order.h>
#include "output_r.h"
#include "randomiser_base.h"
#include "random_base.h"

#include <base/dialog.h>

using namespace base;

namespace boxes {

RandomiserBase::RandomiserBase(QString name, Box *parent)
    : Box(name, parent)
{
    setClassName("boxes", "RandomiserBase");
    Input(iteration).imports("/.[iteration]");
    Input(iterations).imports("/.[iterations]");
    Input(doSensitivityAnalysis).equals(false).help("Carry out a sensitivity analysis?");
    Input(bootstrapSize).equals(1000).help("Size of bootstrap sample (cheap in computation time); only used in sensitivity analysis");
    Input(seed).help("Seed for random numbers; if this is zero a random seed value will be used");
    Input(drawAtInitialize).equals(false).help("Draw a value when a box is initialized?");
    Input(drawAtReset).equals(true).help("Draw a value when a box is reset?");
    Input(drawAtUpdate).equals(false).help("Draw a value when a box is updated?");
    Output(numVariables).noClear().help("Number of variables which are randomised").unit("[0;inf)");
}

void RandomiserBase::initialize() {
    // Set random seed in global generator
    int theSeed = (seed==0) ? static_cast<int>(std::time(nullptr)) : seed;
    base::seedRandomGenerator(theSeed);

    // Find random variables among children
    randomVariables = findMany<RandomBase*>("../RandomBase::*");
    numVariables = randomVariables.size();

    // Initialize generator and check proper no. of iterations according to my type
    inializeGenerator();
    checkIterations(iterations);

    // Set up SA
    if (doSensitivityAnalysis) {
        setBaseSampleSize();
        fillMatrices();
    }
}

namespace {
    void copyColumn(const Matrix<double> &a, Matrix<double> &b, int col) {
        for (int row=0; row<a.numRow(); ++row)
            b(row,col) = a.at(row,col);
    }
}

void RandomiserBase::fillMatrices() {
    // Shuffle the order of numbers picked from Sobol' Sequence
    RandomOrder order(2*N);

    // The A and B matrices will remain fixed
    // C=B except one column that will be replaced, in turn, from A for each iteration
    // N is the sample size; the number of iterations is (2+k)*N
    int &k(numVariables);
    A.resize(N, k);
    B.resize(N, k);
    C.resize(N, k);

    // We must draw 2*N random numbers for each of the k variables.
    for (int i=0; i<2*N; ++i) {

        // Pick the the row number from the shuffled order
        const int &row(order.at(i));

        // The first N rows go to A, the following to B=C
        const bool useA = (row < N);

        // Draw k numbers, representing a point in k-dimensional parameter space
        // col: 0..k-1
        int col = 0;
        for (RandomBase *variable : randomVariables) {
            variable->drawValue();
            if (useA)
                A(row,col) = variable->getValue();
            else
                B(row-N,col) = C(row-N,col) = variable->getValue();
            ++col;
        }
    }
}

void RandomiserBase::reset() {
    if (!doSensitivityAnalysis) return;
    int &k(numVariables);

    // The phase determines which matrix to use as input
    // We will do N iterations in phase A
    // and        N iterations in phase B
    // and      k*N iterations in phase C
    phase = (iteration <= N)   ? Phase::A :
            (iteration <= 2*N) ? Phase::B : Phase::C;

    // The row in the matrix is determined by the current iteration
    // iteration: 1..N
    // row      : 0..(N-1)
    // col      : 0..(k-1)
    int row = (iteration-1) % N, // iteration counts from 1
        col = 0;

    // Depending on phase:
    switch (phase) {
    // Copy values from the A row into the variables
    case Phase::A:
        for (RandomBase *variable : randomVariables)
             variable->setValue(A.at(row, col++));
        break;
    // Copy values from the B row into the variables
    case Phase::B:
        for (RandomBase *variable : randomVariables)
            variable->setValue(B.at(row, col++));
        break;
    // Copy values from the C row into the variables
    case Phase::C:
        // We are at the start of an additional k iterations through C
        if (row==0) {
            // The column to replace: 0..(k-1)
            int colReplaced = (iteration - 1)/N - 2;
            Q_ASSERT(colReplaced>=0 && colReplaced < k);
            // Copy the earlier replaced column back to C from B
            // Then C is back to its original contents
            if (colReplaced > 0)
                copyColumn(B,C,colReplaced-1);
            // Now replace the column with the one from A
            copyColumn(A,C,colReplaced);
        }
        // We are ready to copy values from the C row into the variables
        for (RandomBase *variable : randomVariables)
            variable->setValue(C.at(row, col++));
    }
}

void RandomiserBase::setBaseSampleSize() {
    int &k(numVariables);
    N = iterations/(k+2);
    if (iterations%(k+2) > 0)
        ThrowException("Unexpected: Illegal number of iterations").value(iterations).context(this);
}

void RandomiserBase::checkIterationsSimple(int iterations) {
    int N = static_cast<int>(floor(iterations/double(2+numVariables)));
    if (N<4)
        N = 4;
    int N1 = N*(2+numVariables),
        N2 = (N+1)*(2+numVariables);
    QString msg("Simulation iterations must equal (2+k)*N (k=%1) to run a sensitivity analysis"),
            hint("Use instead, for example, %1 or %2 iterations");
    if (iterations!=N1 && iterations!=N2) {
        ThrowException(msg.arg(numVariables)).
                value(iterations).
                hint(hint.arg(N1).arg(N2)).
                context(this);
    }
}

void RandomiserBase::debrief() {
    OutputR *outputR = findMaybeOne<OutputR*>("*");
    if (outputR &&doSensitivityAnalysis) {
        outputR->addRCode("sobol_k = " + QString::number(numVariables) +
                          "; sobol_N = " + QString::number(N) +
                          "; sobol_B = " + QString::number(bootstrapSize) +"\n" +
                          "source(\"" + environment().inputFileNamePath("scripts/begin-sobol.R") + "\")"
                          );
    }
}

}
