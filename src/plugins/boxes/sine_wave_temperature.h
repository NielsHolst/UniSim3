/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SINE_WAVE_TEMPERATURE
#define SINE_WAVE_TEMPERATURE
#include <base/box.h>

namespace boxes {

class SineWaveTemperature : public base::Box
{
public: 
    SineWaveTemperature(QString name, Box *parent);
    void amend();
    void reset();
    void update();
private:
    // Inputs
    QString recordsName, TminName, TmaxName;
    QTime time, sunrise, solarTime;
    double morningOffset, afternoonOffset;
    int timeStep;
    QString timeUnit;
    // Outputs
    double TminYesterday, TmaxYesterday,
        TminToday, TmaxToday,
        TminTomorrow, TmaxTomorrow,
        value;
    // Debugging outputs
    double proportion;
    int phase;
    // Data
    int _step, _stepsPerDay;
    base::Box *_weather, *_offsetWeather;
    const double *_Tmin, *_Tmax;
    // Methods
    void resetStepsPerDay();
    void updateTemperatures();
    void updateValue();
};

} //namespace
#endif
