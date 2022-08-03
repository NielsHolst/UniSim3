/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef POLLEN_H
#define POLLEN_H
#include <QDate>
#include <base/box.h>

namespace io {

class Pollen : public base::Box
{
public: 
    Pollen(QString name, QObject *parent);
    void initialize();
	void reset();
	void update();

private:
    // Inputs
    double distance, distanceExp, distanceMin, Npeak, lossRate;
    int duration;
    QDate onsetDate, currentDate;
    // Outputs
    double depositionRate, depositionTotal, density;
    // Data
    double scaleToPeakAtOne;
    // Methods
    void setScaleToPeakAtOne();
    int daysIntoShedding();
    double parabola(int x);
    double distanceEffect();
};

} //namespace
#endif
