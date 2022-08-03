/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef RANDOMISER_SOBOL_SEQUENCE_H
#define RANDOMISER_SOBOL_SEQUENCE_H
#include <boost/random/variate_generator.hpp>
#include <boost/random/sobol.hpp>
#include "randomiser_base.h"

namespace boxes {

class RandomiserSobolSequence : public RandomiserBase
{
public:
    RandomiserSobolSequence(QString name, Box *parent);
    ~RandomiserSobolSequence();
    int numStrata() const;
private:
    // Data
    typedef boost::variate_generator<boost::random::sobol&, Distribution> Variate;
    boost::random::sobol *sobol;
    Variate *variate;
    // Methods
    void inializeGenerator();
    void checkIterations(int iterations);
    double draw01();
};

}

#endif
