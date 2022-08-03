/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QDateTime>
#include <QTime>
#include <base/box_builder.h>
#include <base/mega_factory.h>
#include <base/phys_math.h>
#include <base/publish.h>
#include "sine_wave_temperature.h"

using namespace base;

namespace boxes {

PUBLISH(SineWaveTemperature)

SineWaveTemperature::SineWaveTemperature(QString name, Box *parent)
    : Box(name, parent) {
    help("computes sine-wave interpolation from daily min-max temperatures");
    Input(recordsName).equals("weather").help("Name of Records box holding temperature");
    Input(TminName).equals("Tmin").help("Name of column holding daily min. temperature");
    Input(TmaxName).equals("Tmax").help("Name of column holding daily max. temperature");
    Input(morningOffset).equals(0.5).unit("h").help("Hours after sunrise when temperature is at minimum");
    Input(afternoonOffset).equals(3.).unit("h").help("Hours after noon when temperature is at maximum");
    Input(time).imports("calendar[time]");
    Input(sunrise).imports("calendar[sunrise]");
    Input(solarTime).imports("calendar[trueSolarTime]");
    Input(timeStep).imports("calendar[timeStep]");
    Input(timeUnit).imports("calendar[timeUnit]");
    Output(TminYesterday).help("Yesterday's Tmin");
    Output(TmaxYesterday).help("Yesterday's Tmax");
    Output(TminToday).help("Today's Tmin");
    Output(TmaxToday).help("Today's Tmax");
    Output(TminTomorrow).help("Tomorrow's Tmin");
    Output(TmaxTomorrow).help("Tomorrow's Tmax");
    Output(value).help("Sine-interpolated temperature");
}

void SineWaveTemperature::amend() {
    BoxBuilder builder(this);
    builder.
    box("OffsetDateTime").name("offsetDateTime").
        port("dateTime").imports("calendar[dateTime]").
        port("days").equals(1).
    endbox();

    _weather = findOne<Box*>(recordsName);
    _offsetWeather = _weather->cloneFamily("weather2", this);
    _offsetWeather->port("calendarDateTime")->imports("../offsetDateTime[value]");
    _offsetWeather->amend();
    _Tmin = _offsetWeather->port(TminName)->valuePtr<double>();
    _Tmax = _offsetWeather->port(TmaxName)->valuePtr<double>();
}

void SineWaveTemperature::reset() {
    TminYesterday = TminToday = _weather->port(TminName)->value<double>();
    TmaxYesterday = TmaxToday = _weather->port(TmaxName)->value<double>();;
    TminTomorrow = *_Tmin;
    TmaxTomorrow = *_Tmax;
    _step = 0;
    resetStepsPerDay();
    for (int i=0; i<_stepsPerDay; ++i)
        update();
}

void SineWaveTemperature::resetStepsPerDay() {
    switch (timeUnit.at(0).toLatin1()) {
    case 'h':
        if (24%timeStep > 0)
            ThrowException("Time step in hours must be divisible into 24").value(timeStep).context(this);
        _stepsPerDay = 24/timeStep;
        break;

    case 'm':
        if (1440%timeStep > 0)
            ThrowException("Time step in minutes must be divisible into 24*60=1440").value(timeStep).context(this);
        _stepsPerDay = 1440/timeStep;
        break;

    case 's':
        if (86400%timeStep > 0)
            ThrowException("Time step in minutes must be divisible into 24*60*60=86400").value(timeStep).context(this);
        _stepsPerDay = 86400/timeStep;
        break;
    }
}

void SineWaveTemperature::update() {
    updateTemperatures();
    updateValue();
}

void SineWaveTemperature::updateTemperatures() {
    bool shift =  (_step % _stepsPerDay == _stepsPerDay-1 && _step>_stepsPerDay-1);
    if (shift) {
        TminYesterday = TminToday;
        TminToday     = TminTomorrow;
        TminTomorrow  = *_Tmin;
        TmaxYesterday = TmaxToday;
        TmaxToday     = TmaxTomorrow;
        TmaxTomorrow  = *_Tmax;
    }
    _step++;
}

inline int toSecs(double hours) {
    return static_cast<int>(std::round(3600*hours));
}

void SineWaveTemperature::updateValue() {
    int solarOffsetSecs = solarTime.secsTo(time);
    QTime timeTmin = sunrise.addSecs(toSecs(morningOffset)),
          timeTmax = QTime(12,0,0).addSecs(toSecs(afternoonOffset)+solarOffsetSecs);

    if (time<timeTmin) {
        int secsSinceYesterdayTmax = 24*3600 - time.secsTo(timeTmax),
            secsTotal = 24*3600 - timeTmin.secsTo(timeTmax);
        proportion = double(secsSinceYesterdayTmax)/secsTotal;
        value = phys_math::sineWaveDown(proportion, TmaxYesterday, TminToday);
        phase = 1;
    }
    else if (time<timeTmax) {
        int secsSinceTodayTmin = timeTmin.secsTo(time),
            secsTotal = timeTmin.secsTo(timeTmax);
        proportion = double(secsSinceTodayTmin)/secsTotal;
        value = phys_math::sineWaveUp(proportion, TminToday, TmaxToday);
        phase = 2;
    }
    else {
        int secsToTomorrowTmin = timeTmax.secsTo(time),
            secsTotal = 24*3600 - timeTmin.secsTo(timeTmax);
        proportion = double(secsToTomorrowTmin)/secsTotal;
        value = phys_math::sineWaveDown(proportion, TmaxToday, TminTomorrow);
        phase = 3;
    }
}

} //namespace

