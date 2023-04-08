/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_PORT_H
#define BASE_PORT_H
#include <boost/variant/get.hpp>
#include <QMap>
#include <QSet>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QVector>
#include "caller.h"
#include "convert.h"
#include "exception.h"
#include "expression.h"
#include "node.h"
#include "path.h"
#include "port_type.h"
#include "port_status.h"
#include "value.h"
#include "success.h"

namespace base {

class Box;

class Port : public Node {
private:
    PortType _type;         // Purpose of the port
    PortStatus _status;     // Status of port value
    bool
        _clearAtReset,      // Is value set to T() at reset()? Defaults to true for output and aux ports
        _acceptNull,        // Accept evaluation yielding Null?
        _isConstant;        // Does the value remain fixed following its first evaluation?
    QString
        _unparsedExpression,// Original, unparsed expression
        _unit,              // Unit text
        _help;              // Help text
    Value
        _value;             // Holds the current value
    Expression
        _expression;        // Any path operands have been replaced with port pointers, when the expression was closed
    QString
        _outputName;        // Unique name labelling of the column for this port in the output text file;
                            // empty if not included in output
    QVector<Port *>
        _exportPorts;       // Ports which imports this port
    Caller _importCaller;   // Latest object calling for an import of this port (for error message)

public:
    // Configure
    Port(QString name, PortType type, Node *parent);
    Port& doClear();
    Port& noClear();
    Port& acceptNull();
    Port& unit(QString value);
    Port& help(QString value);
    Port& status(PortStatus value);
    void outputName(QString name);

    // Define port
    template <class T> Port& initialize(T *variable);
    template <class T> Port& initialize(T  variable);
    Port& initialize(Value value);
    void setDefaultStatus();
    template <class T> Port& equals(T fixedValue);
    Port& equals(const Value &value);
    Port& equals(const Expression &expression);
    Port& imports(QString pathToPort, Caller caller=Caller());
    Port& computes(QString expression);
    void define();
    void setConstness(bool isConstant);
    void updateConstness();

    // Change value
    void clear();
    void touch();
    void evaluate();

    // Query
    Box *boxParent() const;
    PortType type() const;
    QString unit() const;
    QString help() const;
    PortStatus status() const;
    QString importPath() const;
    QString unparsedExpression() const;
    int size() const;
    bool isConstant() const;
    QString outputName() const;
    QVector<const Port*> importPortsAll() const;
    QVector<const Port*> importPortsLeaves() const;
    QVector<Port*> importPorts() const;
    QVector<Port*> exportPorts() const;

    // Get value
    template <class T> T value() const;
    template <class T> const T* valuePtr() const;
    const Value& value() const;
    void verifyValue() const;
    const Expression& expression() const;
    QString format() const;

    // Housekeeping
    void registerExports();

    // Output
    void toText(QTextStream &text, int indentation = 0);
private:
    enum class Subset{All, Leaves};
    void registerExport(Port *importer);
    QVector<const Port*> collectImports(Subset subset) const;
    void collectImports(
        const QSet<const Port *> &alreadyVisited,
        QSet<const Port*> &currentCollection,
        Port::Subset subset) const;
};

template <class T> Port& Port::initialize(T *variable) {
    _value.initialize(variable);
    _value.clear();
    return *this;
}

template <> Port& Port::initialize(Path *variable);

template <class T> Port& Port::initialize(T fixedValue) {
    _value.initialize(fixedValue);
    equals(fixedValue);
    return *this;
}

template <class T> Port& Port::equals(T fixedValue)
{
    return equals(Value(fixedValue));
}

template <class T> T Port::value() const
{
    return _value.as<T>();
}

template <class T> const T* Port::valuePtr() const
{
    return _value.constPtr<T>();
}

template <> inline const Value* Port::valuePtr() const
{
    return &_value;
}

inline uint qHash(const Port* port) {
    return port->order();
}

}
#endif
