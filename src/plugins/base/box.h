/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef BASE_BOX_H
#define BASE_BOX_H
#include <QMap>
#include <QTextStream>
#include <QVector>
#include "caller.h"
#include "node.h"
#include "path.h"
#include "port.h"
#include "timer.h"
#include "base_dllexport.h"

#define Input(X)  (*new base::Port(#X, base::PortType::Input , this)).initialize(& X).parentClassName(className(Namespace::Include))
#define Output(X) (*new base::Port(#X, base::PortType::Output, this)).initialize(& X).parentClassName(className(Namespace::Include))

#define NamedInput(X,Y)  (*new base::Port((X), base::PortType::Input , this)).initialize(& (Y)).parentClassName(className(Namespace::Include))
#define NamedOutput(X,Y) (*new base::Port((X), base::PortType::Output, this)).initialize(& (Y)).parentClassName(className(Namespace::Include))

namespace base {

class Port;

class Box : public Node
{
public:
    UNI_API Box(QString name, Box *parent);
    UNI_API virtual ~Box();
    UNI_API void addPort(Port *port);

    UNI_API Port* peakPort(QString name);
    UNI_API const Port* peakPort(QString name) const;

    UNI_API Port* port(QString name, Caller caller=Caller());
    UNI_API const Port* port(QString name, Caller caller=Caller()) const;

    UNI_API Box& help(QString s);
    UNI_API QString help() const;

    UNI_API void sideEffects(QString s);
    UNI_API QString sideEffects() const;

    UNI_API void additionalOutputs(QString description);
    UNI_API QString additionalOutputs() const;

    UNI_API void ignore(bool doIgnore);
    UNI_API bool ignore() const;

    UNI_API static bool hasRoot();
    UNI_API static Box* root();
    UNI_API static Box* root(Box *box);

    template<class T> T findOne(QString path);
    template<class T> T findMaybeOne(QString path);
    template<class T> QVector<T> findMany(QString path);

    virtual void amend() {}
    virtual void initialize() {}
    virtual void reset() {}
    virtual void update() {}
    virtual void cleanup() {}
    virtual void debrief() {}
    UNI_API virtual void run();

    UNI_API void amendFamily();
    UNI_API void initializeFamily();
    UNI_API void resetFamily();
    UNI_API void updateFamily();
    UNI_API void cleanupFamily();
    UNI_API void debriefFamily();

    UNI_API void updateConstness();
    UNI_API void updateConstnessFamily();

    UNI_API bool isInitialized() const;

    UNI_API const QVector<Port*> &portsInOrder();
    UNI_API void touchPorts();
    UNI_API void evaluatePorts();
    UNI_API void verifyPorts();
    UNI_API void registerPorts();

    UNI_API Box* clone(QString name, Box *parent);
    UNI_API Box* cloneFamily(QString name, Box *parent);
    UNI_API bool isCloned() const;

    UNI_API static bool debug();
    UNI_API static void debug(bool on);
    UNI_API static bool diagnose();
    UNI_API static void diagnose(double minValue, double maxValue);
    UNI_API static void diagnoseOff();

    UNI_API void createTimer(QString name);
    UNI_API void startTimer(QString name);
    UNI_API void stopTimer(QString name);

    UNI_API QString profileReport();
private:
    // Data
    QString _help, _sideEffects, _additionalOutputs;
    QMap<QString,Port*> _portMap;
    QVector<Port*> _portsInOrder, _trackedPorts;
    bool _amended, _initialized, _cloned, _ignore;
    Timer _timer;
    static std::unique_ptr<Box> _root;
    static Box *_latest;
    static bool _debugOn, _diagnoseOn;
    static double _diagnoseMin, _diagnoseMax;
    // Methods
    Box *latestRoot();
    void createTimers();
    void addPort(QMap<QString,Port*> &ports, Port *port);
    void clearPorts();
    Timer::Report collectTimerReport();
};

template<class T> inline T Box::findOne(QString path) {
    return Path(path).findOne<T>(this);
}

template<class T> inline T Box::findMaybeOne(QString path) {
    return Path(path).findMaybeOne<T>(this);
}

template<class T> inline QVector<T> Box::findMany(QString path) {
    return Path(path).findMany<T>(this);
}

}
#endif
