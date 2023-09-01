/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "box.h"
#include "boxscript_parser.h"
#include "computation.h"
#include "path.h"
#include "port.h"
#include "resolved_references.h"

#include "dialog.h"

namespace base {

Port::Port(QString name, PortType type, Node *parent)
    : Node(name, parent),
      _type(type),
      _status(PortStatus::TypeDefault),
      _acceptNull(false),
      _isConstant(type == PortType::Input),
      _unparsedExpression(""),
      _expression(this)
{
    setClassName("base", "Port");
    Box *boxParent = dynamic_cast<Box*>(parent);
    if (boxParent)
        boxParent->addPort(this);
    _clearAtReset = (type == PortType::Output);
}

Port& Port::doClear() {
    _clearAtReset = true;
    return *this;
}

Port& Port::noClear() {
    _clearAtReset = false;
    return *this;
}

Port& Port::acceptNull() {
    _acceptNull = true;
    return *this;
}

Port& Port::unit(QString value) {
    if (value.toLower() != "bool") // obsolete unit
        _unit = value;
    return *this;
}

Port& Port::help(QString value) {
    _help = value;
    return *this;
}

Port& Port::status(PortStatus value) {
    _status = value;
    return *this;
}

void Port::outputName(QString name) {
    _outputName = name.replace("/", ".");
}

//
// Define port
//

Port& Port::initialize(Value value) {
    _value = value; // value may be undefined but is used to give _value the right type
    _value.clear(); // _value is set to T()
    return *this;
}

void Port::define() {
    // Constness must be considered
    _isConstant = false;

    // Get computation step
    auto step = Computation::currentStep();

    // In scratch mode don't define ports
    if (step == Computation::Step::Scratch)
        return;

    // Prepare to count expression evaluations to Null
    _nullCount = 0;

    // Check parent
    auto *box = parent<Box*>();
    if (!box)
        ThrowException("Port is missing a parent box").context(this);

    // Check that (re-) definition is legal in this step;
    // From Initialization and onwards, only re-definition to a constant expression is valid
    if (step >= Computation::Step::Initialize && !_expression.isConstant())
        ThrowException("Change of port definition only allowed until amend step").
                value(Computation::toString(step)).context(this);

    // Register definition
    switch (step) {
    case Computation::Step::Construct:
        _status = PortStatus::ConstructionDefault;
        break;
    case Computation::Step::Modify:
        _status = PortStatus::UserDefined;
        break;
    default:
        break;
    }

    // Set me as parent of the expression and of any paths in the expression
    _expression.setParent(this);

    // Vector type ports accept null values
    _acceptNull = _value.isVector();

    // Port holds a path
    if (_value.type() == Value::Type::Path) {
        // The expression musy hold exactly one element
        if (_expression.size() != 1)
            ThrowException("Port to a Path variable can only be defined by a path").
                value(_expression.originalAsString()).context(this);

        // Retrieve path value
        Path path;
        const auto &el = _expression.at(0);
        // If the element is a path then assign _value to that
        if (Expression::type(el) == Expression::Type::Path) {
            path = std::get<Path>(el);
        }
        // If the element is a value, it must contain either a string or a path value
        else if (Expression::type(el) == Expression::Type::Value) {
            Value value = std::get<Value>(el);
            switch (value.type()) {
                case Value::Type::String:
                path = Path(value.as<QString>(), this);
                break;
            case Value::Type::Path:
                path = value.as<Path>();
                break;
            default:
                ThrowException("Path or string value expected").
                    value1(Expression::toString(el)).value2(Expression::typeName(el)).
                    context(this);
            }
        }
        path.setParent(this);
        _value.changeValue(path);

        // Path outputs are not cleared at reset
        if (_type == PortType::Output)
            noClear();
    }
    // Evaluation may not complete but try to evaluate value, except Path ports needs no evaluation
    if (_value.type() != Value::Type::Path)
        touch();
}

void Port::setDefaultStatus() {
//    switch (_status) {
//    case PortStatus::Unknown:
//        _status = PortStatus::TypeDefault;
//        _isConstant = (_type==PortType::Input);
//        break;
//    case PortStatus::Redefined:
//        _status = PortStatus::ConstructionDefault;
//        break;
//    case PortStatus::TypeDefault:
//    case PortStatus::ConstructionDefault:
//        ThrowException("Invalid status prior to setting default status").value(convert<QString>(_status)).context(this);
//    }
}

Port& Port::equals(const Value &value) {
    _expression.clear();

    bool portTypeHasBeenSet = (_value.type() != Value::Type::Uninitialized);
    if (portTypeHasBeenSet) {
        // Keep known type of _value
        _value = value;
        _expression.push(_value);
    }
    else {
        // Let _expression deduce type from value
        _expression.push(value);
    }

    _unparsedExpression = _expression.originalAsString();
    define();
    return *this;
}

Port& Port::equals(const Expression &expression) {
    _expression = expression;
    _unparsedExpression = expression.originalAsString();
    define();
    return *this;
}

Port& Port::imports(QString pathToPort, Caller caller) {
    _expression.clear();
    _expression.push(Path(pathToPort, this));
    _unparsedExpression = pathToPort;
    _importCaller = caller;
    define();
    return *this;
}

Port& Port::computes(QString expression) {
    Expression expr = boxscript::parser::parseExpression(this, expression);
    return equals(expr);
}

void Port::setConstness(bool isConstant) {
    _isConstant = isConstant;
}

void Port::updateConstness() {
    // If a port is const or it's an output, there is nothing to do ,
    // an output will keep its constness (which is nearly always false)
//    if (!_isConstant && _type!=PortType::Output)
//        _isConstant = _expression.isConstant();

//    if (_isConstant || _type==PortType::Output)
//        return;
//    // An unresolved expression is not constant
//    if (!_expression.isResolved()) {
//        _isConstant = false;
//        return;
//    }
//    // If any imported port is non-const, neither is this one
//    auto imports = importPortsLeaves();
//    for (auto imported : imports) {
//        if (!imported->isConstant())
//            return;
//    }
//    // All imports were constant; upgrade this to be constant
//    _isConstant = true;
}

//
// Change value
//

void Port::clear() {
    if (_clearAtReset)
        _value.clear();
}

void Port::touch() {
    // Tentative evaluation. Imports of yet-undefined ports in _expression
    // will be replaced ny null values; hence _expression is kept
    auto keep = _expression;
    evaluate();
    _expression = keep;
}

void Port::evaluate() {
    // A constant port, a port with an empty expression, or a port to path needs no evaluation
    if (_isConstant || _expression.isEmpty() || _value.type() == Value::Type::Path)
        return;

    if (_type == PortType::Auxiliary &&
        (Computation::currentStep() <= Computation::Step::Reset ||
         _value.type() == Value::Type::Uninitialized ||
         !ResolvedReferences::fixed()))
        // Overwrite value's type, as an auxillary port may change type:  Its expression
        // is evaluated again and again, and additional references (with other types) may have been resolved
        _value.overwrite(_expression.evaluate());
    else {
        // Update value, keeping its type, which for inputs and outputs were defined
        // in the C++ code; the expression's type will be converted to the value's types
        // For aux ports, all references have now been resolved and the value's type thereby fixed        
        Value evaluation = _expression.evaluate();
        if  (evaluation.isNull() && !_acceptNull)
            ++_nullCount;
        bool vectorError = evaluation.isVector() && !_value.isVector(),
             nullError   = (evaluation.isNull() && !_acceptNull && ResolvedReferences::fixed()) || (_nullCount > 10);

        if (vectorError || nullError) {
            QString msg = vectorError ? "Cannot assign vector to scalar" : "Cannot assign to null. Reference not found";
            ThrowException(msg).
                    value(_expression.originalAsString()).
                    hint("Ports used by the expression:\n" + _expression.importPortNames().join("\n")).
                    context(this);
        }
        _value = evaluation;
    }

    // Register if the value will remain constant after reset
    // Ensure outputs are never constant; otherwise Output(x).equals(1) would mean x remains constant forever
    if (Computation::currentStep() <= Computation::Step::Reset && _type != PortType::Output)
        _isConstant = _expression.isConstant();
}

//
// Query
//

Box *Port::boxParent() const {
    Box *par = dynamic_cast<Box*>(parent());
    if (!par)
        ThrowException("Application error: Port has no Box parent").context(this);
    return par;
}

PortType Port::type() const {
    return _type;
}

QString Port::unit() const {
    return _unit;
}

QString Port::help() const {
    return _help;
}

PortStatus Port::status() const {
    return _status;
}

QString Port::importPath() const {
    if (_expression.size() != 1) {
        ThrowException("Port expression must have only on token").
            value(_expression.originalAsString()).context(this);
    }

    const Expression::Element &element(_expression.original().at(0));
    if (Expression::type(element) != Expression::Type::Path) {
        ThrowException("Port expression must be a path").
            value(_expression.originalAsString()).context(this);
    }

    const Path &path(std::get<Path>(element));
    return path.toString();
}

QString Port::unparsedExpression() const {
    return _unparsedExpression;
}

int Port::size() const {
    return _value.size();
}

bool Port::isConstant() const {
    return _isConstant;
}

QString Port::outputName() const {
    return _outputName;
}

QVector<const Port*> Port::importPortsAll() const {
    return collectImports(Subset::All);
}

QVector<const Port*> Port::importPortsLeaves() const {
    return collectImports(Subset::Leaves);
}

QVector<const Port*> Port::collectImports(Subset subset) const {
    // Collect all imports in a set
    QSet<const Port*> imported;
    collectImports(QSet<const Port*>(), imported, subset);

    // Exclude self from set
    imported.remove(this);

    // Turn into a vector and sort by order
    QVector<const Port*> result(imported.begin(), imported.end());
    std::sort(result.begin(), result.end(),
              [](const Port *a, const Port *b) { return a->order() < b->order(); });
    return result;
}

void Port::collectImports(
        const QSet<const Port *> &alreadyVisited,
        QSet<const Port*> &currentCollection,
        Port::Subset subset) const
{
    QVector<Port*> imported = importPorts();
    bool isLeaf = imported.isEmpty();
//    int id = order();
    if (subset==Subset::All || isLeaf)
        currentCollection << this;
    for (Port *port : imported) {
        if (!alreadyVisited.contains(this)) {
            QSet<const Port *> alreadyVisitedPlus = alreadyVisited;
            alreadyVisitedPlus += this;
            port->collectImports(
                alreadyVisitedPlus,
                currentCollection,
                subset);
        }
    }
}

QVector<Port*> Port::importPorts() const {
    if (_value.type() == Value::Type::Path) {
        auto matches = _value.as<Path>().matches(boxParent());
        QVector<Port*> result;
        for (auto match : matches)
            result << dynamic_cast<Port*>(match);
        return result;
    }
    else {
        return _expression.importPorts();
    }
}

QVector<Port*> Port::exportPorts() const {
    return _exportPorts;
}

const Value& Port::value() const {
    return _value;
}

void Port::verifyValue() const {
    if (_value.type() == Value::Type::Double &&
        std::fpclassify(_value.as<double>()) == FP_NAN) {
            ThrowException("Value is not a number").context(this);
    }
}

const Expression& Port::expression() const {
    return _expression;
}

QString Port::format() const {
    using Type = Value::Type;
    switch (_value.baseType()) {
        case Type::String:
        case Type::BareDate:
            return "character";
            break;
        case Type::Date:
            return "ymd";
            break;
        case Type::Time:
            return "HMS";
            break;
        case Type::DateTime:
            return "ymdHMS";
            break;
        default:
            return "NA";
    }
}

//
// Housekeeping
//

void Port::registerExports() {
    for (auto port : _expression.importPorts())
        port->registerExport(this);
}

void Port::registerExport(Port *importer) {
    if (!_exportPorts.contains(importer))
        _exportPorts << importer;
}

// Access

void Port::toText(QTextStream &text, int indentation) {
    static QMap<PortType,QString> prefix =
        {{PortType::Input, "."}, {PortType::Output, ">"}, {PortType::Auxiliary, "&"}};
    QString fill;
    fill.fill(' ', indentation);
    text << fill
         << prefix.value(_type) << objectName()
         << " = " << _expression.originalAsString()
         << " // #" << order()
         << "\n";
}

template <> Port& Port::initialize(Path *path) {
    path->setParent(this);
    _value = Value(path);
    return *this;
}

}

