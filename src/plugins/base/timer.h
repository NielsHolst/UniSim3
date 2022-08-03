/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_TIMER_H
#define BASE_TIMER_H
#include <chrono>
#include <QMap>
#include <QString>
#include <QStringList>

namespace base {

class Box;

class Timer
{
public:
    Timer(Box *parent);
    void addProfile(QString name);
    void reset();
    void start(QString name);
    void stop(QString name);
    double duration(QString name) const;

    struct ReportItem {
        Box *parent;
        QString name;
        double duration;
    };
    using Report = QVector<ReportItem>;
    Report report() const;
private:
    Box *_parent;
    using TimePoint = std::chrono::steady_clock::time_point;
    struct Watch {
        TimePoint startedAt;
        std::chrono::steady_clock::time_point::duration total;
    };
    QMap<QString, Watch> _lookup;
};

}

#endif
