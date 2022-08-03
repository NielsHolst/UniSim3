/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef POLLEN_ONSET_DATE_BASE_H
#define POLLEN_ONSET_DATE_BASE_H
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <QDate>
#include <base/box.h>
#include <base/random_generator.h>

namespace io {

class PollenOnsetDateBase : public base::Box
{
public:
    PollenOnsetDateBase(QString name, QObject *parent);
protected:
    // Input
    int year;
    // Outputs
    double fractile;
    QDate date;
    // Methods
    void updateFractile();
private:
    // Random number generation
    typedef boost::uniform_real<double> Uniform01;
    typedef boost::variate_generator<base::RandomGenerator::Generator&, Uniform01> Variate;
    Uniform01 *uniform01;
    Variate *variate;

};

} //namespace
#endif
