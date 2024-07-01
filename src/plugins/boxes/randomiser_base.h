/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef RANDOMISER_BASE_H
#define RANDOMISER_BASE_H
#include <boost/random/uniform_01.hpp>
#include <boost/random/variate_generator.hpp>
#include <QVector>
#include <base/box.h>
#include <base/matrix.h>

namespace boxes {

class RandomBase;
class RandomiserBase : public base::Box
{
public:
    RandomiserBase(QString name, Box *parent);
    void initialize() final;
    void reset() final;
    void debrief() final;
    virtual int numStrata() const = 0;
    virtual double draw01() = 0;
private:
    // Inputs
    int iteration, seed, bootstrapSize;
    bool
        drawAtInitialize,
        drawAtReset,
        drawAtUpdate;
    // Data
    QVector<RandomBase*> randomVariables;
    base::Matrix<double> A, B, C;
    enum class Phase {A, B, C};
    Phase phase;
    // Methods
    void setBaseSampleSize();
    void fillMatrices();
    virtual void inializeGenerator() = 0;
    virtual void checkIterations(int iterations) = 0;
protected:
    // Inputs
    int iterations;
    bool doSensitivityAnalysis;
    // Outputs
    int numVariables;
    // Data
    typedef boost::uniform_01<double> Distribution;
    Distribution distribution;
    int N;
    // Methods
    void checkIterationsSimple(int iterations);
};

}

#endif
