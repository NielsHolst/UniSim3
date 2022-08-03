/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "box.h"
#include "exception.h"
#include "timer.h"

namespace base {

inline auto now() {
    return std::chrono::steady_clock::now();
}

inline auto zero() {
    return std::chrono::steady_clock::time_point::duration::zero();
}

Timer::Timer(Box *parent)
    : _parent(parent)
{
}

void Timer::addProfile(QString name) {
    if (_lookup.contains(name))
        ThrowException("Timer already present").value(name).context(_parent);
    _lookup[name] = Watch{TimePoint(), zero()};
}

void Timer::reset() {
    for (auto it = _lookup.begin(); it != _lookup.end(); ++it) {
        Watch &watch(*it);
        watch.total = zero();
    }
}

void Timer::start(QString name) {
    if (!_lookup.contains(name))
        ThrowException("No timer with that name").value(name)
                .value2(QStringList(_lookup.keys()).join(", ")).context(_parent);
    _lookup[name].startedAt = now();
}

void Timer::stop(QString name) {
    using namespace std::chrono;
    auto stoppedAt = steady_clock::now();
    Watch &watch(_lookup[name]);
    auto timeSpan = stoppedAt - watch.startedAt;
    watch.total += timeSpan;
}

double Timer::duration(QString name) const {
    // Duration in milliseconds
    const Watch &watch(_lookup[name]);
    return std::chrono::duration_cast<std::chrono::nanoseconds>(watch.total).count()*1e-6;
}

Timer::Report Timer::report() const {
    Timer::Report result;
    auto it = _lookup.constBegin();
    while (it != _lookup.constEnd()) {
        result << ReportItem{_parent, it.key(), duration(it.key()) };
        ++it;
    }
    return result;
}


}
