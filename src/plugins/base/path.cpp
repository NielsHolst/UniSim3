/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QMap>
#include "box.h"
#include "boxscript_parser.h"
#include "expression.h"
#include "path.h"
#include "port.h"

using namespace std;

namespace base {

// Lookup tables

static QMap<QString, Path::Directive> nodeDirFromString =
{
    {"any",         Path::Directive::Any},
    {"children",    Path::Directive::Children},
    {"descendants", Path::Directive::Descendants},
    {"ancestors",   Path::Directive::Ancestors},
    {"siblings",    Path::Directive::Siblings},
    {"preceding",   Path::Directive::Preceding},
    {"following",   Path::Directive::Following}
};

static QMap<Path::Directive, QString> nodeDirToString =
{
    {Path::Directive::Any        , "any"        },
    {Path::Directive::Children   , "children"   },
    {Path::Directive::Descendants, "descendants"},
    {Path::Directive::Ancestors  , "ancestors"  },
    {Path::Directive::Siblings   , "siblings"   },
    {Path::Directive::Preceding  , "preceding"  },
    {Path::Directive::Following  , "following"  }
};

inline bool isNodeDirective(std::string s) {
    return nodeDirFromString.contains(QString::fromStdString(s));
}

inline bool isNodeDirective(QString s) {
    return nodeDirFromString.contains(s);
}

inline QString qstr(std::string s) {
    return QString::fromStdString(s);
}

// Node

Path::Node::Node(QString directive, QString className, QString objectName)
    : _className(className), _objectName(objectName)
{
    if (directive.isEmpty())
        _directive =  Directive::Any;
    else if (isNodeDirective(directive))
        _directive = nodeDirFromString.value(directive);
    else
        ThrowException("Unknown node directive").value(directive);
}

Path::Node::Node(std::vector<std::string> names) {
    // Must have at least one name
    if (names.empty())
        ThrowException("Empty name in path");

    // Directive, if present, must be first name
    bool hasDirective = isNodeDirective(names.at(0));

    // If no directive, the node refers to Any by default
    _directive = hasDirective ?
         nodeDirFromString.value(qstr(names.at(0))) :
         base::Path::Directive::Any;

    // Set class and object name
    bool tooLong = false;
    switch (names.size()) {
    case 1:
        // Object name only
        _objectName = qstr(names.at(0));
        break;
    case 2:
        if (hasDirective)
            // Directive + object name
            _objectName = qstr(names.at(1));
        else {
            // Class name, + object name
            _className  = qstr(names.at(0));
            _objectName = qstr(names.at(1));
        }
        break;
    case 3:
        if (hasDirective) {
            // Directive + class name + object name
            _className  = qstr(names.at(1));
            _objectName = qstr(names.at(2));
        }
        else {
            // (Namespace + class name) + object name
            _className  = qstr(names.at(0)) + "::" + qstr(names.at(1));
            _objectName = qstr(names.at(2));
        }
        break;
    case 4:
        if (hasDirective) {
            // Directive + (namespace + class name) + object name
            _className  = qstr(names.at(1)) + "::" + qstr(names.at(2));
            _objectName = qstr(names.at(3));
        }
        else
            ThrowException("Unknown path directive").value(qstr(names.at(0)));
        break;
    default:
        tooLong = true;
    }
    if (tooLong) {
        QStringList list;
        for (auto s : names)
            list << qstr(s);
        ThrowException("Too many tokens (>4) in path").value(list.join("::")).value2(names.size());
    }
}

QString Path::Node::toString() const {
    QString result = (_directive == Directive::Any) ? "" : (nodeDirToString.value(_directive) + "::");
    if (!_className.isEmpty())
        result += _className + "::";
    return result + _objectName;
}

// Port

Path::Port::Port(QString directive, QString name)
    : _name(name)
{
    if (!directive.isEmpty()) {
        if (isPortType(directive))
            _directive = portTypeFromString(directive);
        else
            ThrowException("Unknown port directive").value(directive);
    }
}

Path::Port::Port(std::vector<std::string> names) {
    // Must have at least one name
    if (names.empty())
        ThrowException("Empty port name");

    // Directive, if present, must be first name
    bool hasDirective = isPortType(qstr(names.at(0)));
    if (hasDirective)
        _directive = portTypeFromString(qstr(names.at(0)));

    // Set name
    bool tooLong = false;
    switch (names.size()) {
    case 1:
        // Only name
        if (hasDirective)
            ThrowException("Directive cannot be used alone").value(qstr(names.at(0)));
        _name = qstr(names.at(0));
        break;
    case 2:
        if (hasDirective)
            // Directive :: name
            _name = qstr(names.at(1));
        else {
            ThrowException("Unknown path directive").value(qstr(names.at(0)));
        }
        break;
    default:
        tooLong = true;
    }
    if (tooLong) {
        QStringList list;
        for (auto s : names)
            list << qstr(s);
        ThrowException("Too many tokens (>2) in port").value(list.join("::")).value2(names.size());
    }

}

bool Path::Port::matches(const base::Port *port) {
    bool directiveOk = (!_directive.has_value() || *_directive == port->type()),
         nameOk      = (_name == "*"            ||  _name      == port->name());
    return directiveOk && nameOk;
}

QString Path::Port::toString() const {
    QString result = _directive.has_value() ? (portTypeToString(*_directive) + "::") : "";
    return "[" + result + _name + "]";
}

// Alternative

Path::Alternative::Alternative() {
}

void Path::Alternative::setRoot(bool hasRoot) {
    _hasRoot = hasRoot;
}

void Path::Alternative::addNode(Node node) {
    // An empty directive means Any for the first node and, Children for the following nodes
    if (!_nodes.isEmpty() && node._directive==Directive::Any)
        node._directive = Directive::Children;
    _nodes << node;
}

void Path::Alternative::setPort(const Port &port) {
    _port = port;
}

bool Path::Alternative::hasRoot() const {
    return _hasRoot;
}

const QVector<Path::Node> &Path::Alternative::nodes() const {
    return _nodes;
}

const std::optional<Path::Port> &Path::Alternative::port() const {
    return _port;
}

QString Path::Alternative::toString() const {
    QStringList list;
    for (auto node : _nodes)
        list <<node.toString();

    QString result = _hasRoot ? "/" : "";
    result += list.join("/");
    if (_port.has_value())
        result += _port->toString();
    return result;
}

const Path::Objects& Path::Alternative::matches(base::Box *parent) {
    auto next = _nodes.begin();
    if (next) {
        initiateMatches(*next, parent);
        ++next;
    }
    for(;next != _nodes.end(); ++next)
        filterMatches(*next);

    if (_port.has_value()) {
        Objects ports;
        for (auto candidate : _matches) {
            // If the candidate is a port, test if it matches _port
            auto *basePort = dynamic_cast<base::Port*>(candidate);
            if (basePort) {
                if (_port->matches(basePort))
                    ports << candidate;
            }
            // Otherwise, if the candidate is a box then
            // loop through all its ports and test if they match _port
            else {
                auto *parentBox = dynamic_cast<base::Box*>(candidate);
                if (parentBox) {
                    for (auto basePort : parentBox->children<base::Port*>()) {
                        if (_port->matches(basePort))
                            ports << dynamic_cast<base::Node*>(basePort);
                    }
                }
            }
            // Other sub-classes of Node objects are ignored
        }
        _matches.clear();
        _matches = ports;
    }
    return _matches;
}

void Path::Alternative::initiateMatches(const Node &node, base::Node *parentNode) {
    // Find root
    Object *root = Box::root();

    // Check that root or parent exists, if need
    enum {NeedsRoot, NeedsParent} need;
    need = (node._objectName.contains(".") || node._directive!=Directive::Any) ? NeedsParent : NeedsRoot;

    // Check root
    if (need==NeedsRoot && !root)
        ThrowException("No root found for path").value(toString()).context(parentNode);

    // Check parent
    if (need==NeedsParent && !parentNode)
        ThrowException("No parent found for path").value(toString()).context(parentNode);

    // Set up initial nodes for search
    Object *match;
    _matches.clear();
    if (_hasRoot) {
        if (node._objectName == ".")
            // Search from the root==node
            _matches << root;
        else
            // Search from the root's children
            _matches << children(root, node._className, node._objectName);
    }
    else if (node._objectName == ".")
        // Search from parent node
        _matches << parentNode;
    else if (node._objectName == "..") {
        // Search from grandparent node
        match = parent(parentNode, "", "*");
        if (match) _matches << match;
    }
    else if (node._objectName == "...")
        // Search from nearest nodes
        _matches << parentNode << ancestors(parentNode, "", "*");
    else {
        switch(node._directive) {
        case Directive::Any:
            // Search from any node matching class and object name
            _matches << any(root, node._className, node._objectName);
            break;
        case Directive::Children:
            // Search from all parent's children
            _matches << children(parentNode, node._className, node._objectName);
            break;
        case Directive::Descendants:
            // Search from parent's descendants
            _matches << descendants(parentNode, node._className, node._objectName);
            break;
        case Directive::Ancestors:
            // Search from parent's ancestors
            _matches << ancestors(parentNode, node._className, node._objectName);
            break;
        case Directive::Siblings:
            // Search from parent's siblings
            _matches << siblings( parentNode, node._className, node._objectName);
            break;
        case Directive::Preceding:
            // Search from parent's preceding sibling
            match = preceding(parentNode, node._className, node._objectName);
            if (match) _matches << match;
            break;
        case Directive::Following:
            // Replace each match with its following sibling
            match = following(parentNode, node._className, node._objectName);
            if (match) _matches << match;
            break;
        }
    }
}

void Path::Alternative::filterMatches(const Node &node) {
    if (node._objectName == ".") {
        // Nothing to do
        ;
    }
    else if (node._objectName == "..") {
        // Replace matches with their parents
        _matches = parents(_matches, "", "*");
    }
    else if (node._objectName == "...") {
        // Add ancestors to matches
        _matches << ancestors(_matches, "", "*");
    }
    else {
        switch(node._directive) {
        case Directive::Any:
            ThrowException("Any direction is invalid here").value(toString());
        case Directive::Children:
            // Replace matches with their children
            _matches = children(_matches, node._className, node._objectName);
            break;
        case Directive::Descendants:
            // Replace matches with their descendants
            _matches = descendants(_matches, node._className, node._objectName);
            break;
        case Directive::Ancestors:
            // Replace matches with their ancestors
            _matches = ancestors(  _matches, node._className, node._objectName);
            break;
        case Directive::Siblings:
            // Replace matches with their siblings
            _matches = siblings(   _matches, node._className, node._objectName);
            break;
        case Directive::Preceding:
            // Replace each match with its preceding sibling
            _matches = preceding(  _matches, node._className, node._objectName);
            break;
        case Directive::Following:
            // Replace each match with its following sibling
            _matches = following(  _matches, node._className, node._objectName);
            break;
        }
    }
}

bool match(Path::Object *p, QString className, QString objectName) {
    return (className.isEmpty() || p->isType(className)) &&
           (objectName == "*"   || p->name() == objectName);
}

Path::Objects Path::Alternative::any(Object *p, QString className, QString objectName) {
    auto desc = p->descendants<Object*>();
    Objects all, result;
    all << p << Objects(desc.begin(), desc.end());
    for (auto candidate : all) {
        if (match(candidate, className, objectName))
            result << candidate;
    }
    return result;
}

Path::Objects Path::Alternative::descendants(Object *p, QString className, QString objectName) {
    Objects result;
    auto descendants = p->descendants<Object*>();
    for (auto descendant : descendants) {
        if (match(descendant, className, objectName))
            result << descendant;
    }
    return result;
}

Path::Objects Path::Alternative::descendants(Objects candidates, QString className, QString objectName) {
    Objects result;
    for (auto candidate : candidates)
        result << descendants(candidate, className, objectName);
    return result;
}

Path::Object* Path::Alternative::parent(Object *p, QString className, QString objectName) {
    auto parent = p->parent<Box*>();
    return (parent && match(parent, className, objectName)) ? parent : nullptr;
}

Path::Objects Path::Alternative::parents(Objects candidates, QString className, QString objectName) {
    Objects result;
    for (auto candidate : candidates) {
        auto p = parent(candidate, className, objectName);
        if (p)
            result << p;
    }
    return result;
}

Path::Objects Path::Alternative::ancestors(Object *p, QString className, QString objectName) {
    Objects result;
    auto q = dynamic_cast<Box*>(p->parent());
    if (q) {
        if (match(q, className, objectName))
            result << q;
        result << ancestors(q, className, objectName);
    }
    return result;
}

Path::Objects Path::Alternative::ancestors(Objects candidates, QString className, QString objectName) {
    Objects result;
    for (auto candidate : candidates)
        result << ancestors(candidate, className, objectName);
    return result;
}

Path::Objects Path::Alternative::children(Object *p, QString className, QString objectName) {
    Objects result;
    auto children = p->children<Box*>();
    for (auto child : children) {
        if (match(child, className, objectName))
            result << child;
    }
    return result;
}

Path::Objects Path::Alternative::children(Objects candidates, QString className, QString objectName) {
    Objects result;
    for (auto candidate : candidates)
        result << children(candidate, className, objectName);
    return result;
}

Path::Objects Path::Alternative::siblings(Object *p, QString className, QString objectName) {
    Objects result;
    auto children = p->children<Box*>();
    for (auto child : children) {
        if (child != p && match(child, className, objectName))
            result << child;
    }
    return result;
}

Path::Objects Path::Alternative::siblings(Objects candidates, QString className, QString objectName) {
    Objects result;
    for (auto candidate : candidates)
        result << siblings(candidate, className, objectName);
    return result;
}

Path::Object* Path::Alternative::preceding(Object *p, QString className, QString objectName) {
    auto children = p->children<Box*>();
    int i=0;
    while (children.at(i) != p) ++i;
    return (i>0 && match(children.at(i), className, objectName)) ? children[i] : nullptr;
}

Path::Objects Path::Alternative::preceding(Objects candidates, QString className, QString objectName) {
    Objects result;
    for (auto candidate : candidates) {
        auto p = preceding(candidate, className, objectName);
        if (p)
            result << p;
    }
    return result;
}

Path::Object* Path::Alternative::following(Object *p, QString className, QString objectName) {
    auto children = p->children<Box*>();
    int i=0,
        n=children.size();
    while (children.at(i) != p) ++i;
    return (i<n-1 && match(children.at(i+1), className, objectName)) ? children[i+1] : nullptr;
}

Path::Objects Path::Alternative::following(Objects candidates, QString className, QString objectName) {
    Objects result;
    for (auto candidate : candidates) {
        auto p = following(candidate, className, objectName);
        if (p)
            result << p;
    }
    return result;
}

// Path

Path::Path(base::Port *parent)
    : _parent(parent)
{
}

Path::Path(QString path, base::Port *parent)
    : _parent(parent)
{
    // The parser needs a port on every alternative: Add missing ports as blanks
    QStringList alternatives = path.split("|"), corrected;
    for (auto alternative : alternatives)
        corrected << (path.contains("[") ? alternative : alternative.trimmed() + "[]");

    // Parse path as an expression
    auto expression = boxscript::parser::parseExpression(nullptr, corrected.join("|"));
    if (expression.size() != 1 || Expression::type(expression.at(0)) != Expression::Type::Path)
        ThrowException("Invalid path").value(path).context(parent);
    initialize(expression);
}

Path::Path(const Expression *expression) {
    Q_ASSERT(expression);
    initialize(*expression);
}

void Path::initialize(Expression expression) {
    if (expression.size() != 1 || Expression::type(expression.at(0)) != Expression::Type::Path)
        ThrowException("Invalid path").value(expression.originalAsString()).context(_parent);

    Path parsedPath = std::get<Path>(expression.at(0));
    _alternatives = parsedPath._alternatives;
    _matches = parsedPath._matches;
    setParent(expression.parent());
}

void Path::setParent(base::Port *parent)
{
    _parent = parent;
}

base::Port *Path::parent() {
    return _parent;
}

void Path::add(const Alternative &alternative) {
    _alternatives << alternative;
}

void Path::add(const Path &path) {
    for (const auto &alternative : path.alternatives())
        add(alternative);
}

const Path::Objects &Path::matches(base::Node *anchor) const {
    // Find matches from scratch
    _matches.clear();

    // Nothing to find
    if (_alternatives.isEmpty())
        return _matches;

    // Path must have a parent or an anchor
    base::Node *nodeAnchor = anchor ? anchor : dynamic_cast<base::Node*>(_parent);
    if (!nodeAnchor)
        ThrowException("Path must have a parent or an anchor").value(toString())
                .value2(_parent->fullName());

    // The anchor must be a Box; if not use its parent
    base::Box *boxAnchor = dynamic_cast<Box*>(nodeAnchor);
    if (!boxAnchor)
        boxAnchor = nodeAnchor->parent<Box*>();
    if (!boxAnchor)
        ThrowException("Path must be anchored to a box").value(toString())
                                                        .value2(_parent->fullName());

    // Collect matches
    for (auto &alternative : _alternatives) {
        for (auto match : alternative.matches(boxAnchor)) {
            // Avoid duplicates
            if (!_matches.contains(match))
                _matches << match;
        }
    }
    return _matches;
}

QString Path::toString() const {
    QStringList list;
    for (auto alt : _alternatives)
        list <<alt.toString();
    return list.join("|");
}

const QVector<Path::Alternative> &Path::alternatives() const {
    return _alternatives;
}

bool Path::isValid(QString path) {
    auto expression = boxscript::parser::parseExpression(nullptr, path);
    return (expression.size() == 1 &&
            Expression::type(expression.at(0)) == Expression::Type::Path);

}

template<> QVector<base::Port*> Path::findMany(base::Node *anchor) const {
    QVector<base::Port*> result;
    for (auto candidate : matches(anchor)) {
        base::Port *port = dynamic_cast<base::Port*>(candidate);
        if (port) {
            // If port found in this path itself holds a path
            // then look for further ports in that path
            if (port->value().type() == Value::Type::Path) {
                const Path &pathReferredTo = port->value<Path>();
                // Accept but don't follow cyclic references
                auto portsReferredTo = pathReferredTo.findMany<base::Port*>(port);
                for (auto port : portsReferredTo) {
                    if (!result.contains(port))
                        result << port;
                }
            }
            // Else just add the port found in this path
            else
                result << port;
        }
    }
    std::sort(result.begin(), result.end(),
              [](base::Port *a, base::Port *b) { return a->order() < b->order(); });

    QStringList names;
    for (auto r : result)
        names << r->fullName();
//    dialog().information("Found:\n" + names.join("\n"));
    return result;
}

QSet<base::Node *> Path::nodesOnPath() const {
    QSet<base::Node*> result;
    auto nodes = findMany<base::Node*>();
    for (auto node : nodes) {
        base::Port *port = dynamic_cast<base::Port*>(node);
        if (port) {
            auto ports = port->importPorts();
            QSet<base::Node*> set = QSet<base::Node*>(ports.begin(), ports.end());
            result += set;
        }
    }
    return result;
}

} //namespace
