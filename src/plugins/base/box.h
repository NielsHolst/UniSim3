/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
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

#define Input(X)  (*new base::Port(#X, base::PortType::Input , this)).initialize(& X)
#define Output(X) (*new base::Port(#X, base::PortType::Output, this)).initialize(& X)

#define NamedInput(X,Y)  (*new base::Port((X), base::PortType::Input , this)).initialize(& (Y))
#define NamedOutput(X,Y) (*new base::Port((X), base::PortType::Output, this)).initialize(& (Y))

namespace base {

class Port;

class Box : public Node
{
public:
    Box(QString name, Box *parent);
    ~Box();
    void addPort(Port *port);

    Port* peakPort(QString name);
    const Port* peakPort(QString name) const;

    Port* port(QString name, Caller caller=Caller());
    const Port* port(QString name, Caller caller=Caller()) const;

    void help(QString s);
    QString help() const;

    void sideEffects(QString s);
    QString sideEffects() const;

    void additionalOutputs(QString description);
    QString additionalOutputs() const;

    static bool hasRoot();
    static Box* root();
    static Box* root(Box *box);

    template<class T> T findOne(QString path);
    template<class T> T findMaybeOne(QString path);
    template<class T> QVector<T> findMany(QString path);

    virtual void amend() {}
    virtual void initialize() {}
    virtual void reset() {}
    virtual void update() {}
    virtual void cleanup() {}
    virtual void debrief() {}
    virtual void run();

    void amendFamily();
    void initializeFamily();
    void resetFamily();
    void updateFamily();
    void cleanupFamily();
    void debriefFamily();

    bool isInitialized() const;

    const QVector<Port*> &portsInOrder();
    void touchPorts();
    void evaluatePorts();
    void verifyPorts();
    void registerPorts();

    Box* clone(QString name, Box *parent);
    Box* cloneFamily(QString name, Box *parent);
    bool isCloned() const;

    static bool debug();
    static void debug(bool on);

    void createTimer(QString name);
    void startTimer(QString name);
    void stopTimer(QString name);

    QString profileReport();
private:
    // Data
    QString _help, _sideEffects, _additionalOutputs;
    QMap<QString,Port*> _portMap;
    QVector<Port*> _portsInOrder, _trackedPorts;
    bool _amended, _initialized, _cloned;
    Timer _timer;
    static std::unique_ptr<Box> _root;
    static Box *_latest;
    static bool _debugOn;
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

template<class T> inline  QVector<T> Box::findMany(QString path) {
    return Path(path).findMany<T>(this);
}

}
#endif
