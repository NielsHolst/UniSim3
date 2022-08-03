/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef RANDOMISER_MONTE_CARLO_H
#define RANDOMISER_MONTE_CARLO_H
#include <boost/random/variate_generator.hpp>
#include <base/random_generator.h>
#include "randomiser_base.h"

namespace boxes {

class RandomiserMonteCarlo : public RandomiserBase
{
public:
    RandomiserMonteCarlo(QString name, Box *parent);
    ~RandomiserMonteCarlo();
    int numStrata() const;
private:
    // Data
    typedef boost::variate_generator<base::RandomGenerator::Generator&, Distribution> Variate;
    Variate *variate;
    // Methods
    void inializeGenerator();
    void checkIterations(int iterations);
    double draw01();
};

}

#endif
