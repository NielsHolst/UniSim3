/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "box.h"
#include "exception.h"
#include "node.h"
#include "port.h"

#include <iostream>

namespace base {

const Node *Node::_current = nullptr;

Node::Node(QString name, Node *parent)
    : _objectName(name), _parent(parent), _order(-1)
{
    _current = this;
    if (parent)
        parent->addChild(this);
}

Node::~Node() {
    _current = nullptr;
    for (auto child : _children)
        delete child;
    _children.clear();
}

void Node::addChild(Node *child) {
    _children << child;
}

void Node::removeChild(Node *child) {
    bool found = false;
    for (auto ch = _children.begin(); ch!=_children.end(); ++ch) {
        if (*ch == child) {
            _children.erase(ch);
            found = true;
            break;
        }
    }
    if (!found)
        ThrowException("Child not found").value(child->fullName()).context(this);
}

void Node::setParent(Node *parent) {
    if (_parent)
        ThrowException("Parent already set").value(parent->fullName()).context(this);
    _parent = parent;
    if (parent)
        parent->addChild(this);
}

void Node::setClassName(QString namespaceName, QString className) {
    const auto type = Type{namespaceName, className};
    if (!_pedigree.contains(type))
        _pedigree.append(type);
}

void Node::setObjectName(QString name) {
    _objectName = name;
}

void Node::setOrder(int order) {
    _order = order;
}

void Node::addAttribute(QString name, QString value) {
    _attributes[name] = value;
}

void Node::enumerate() {
    Box *root = Box::root();
    if (!root)
      ThrowException("Missing root");
    int i = 0;
    root->enumerate(i);
}

void Node::enumerate(int &i) {
    for (auto *box : children<Box*>())
        box->enumerate(i);
    for (auto *port : children<Port*>())
        port->enumerate(i);
    _order = i++;
}

QString Node::fullName(const Node *node) {
    QStringList names;
    QString portName("");
    const Node *p = node;
    if (dynamic_cast<const Port*>(p)) {
        portName = "[" + p->objectName() + "]";
        p = p->parent();
    }
    while (p) {
        QString name = p->objectName();
        if (name.isEmpty())
            name = p->className() + "::*";
        names.prepend(name);
        p = p->parent();
    }
    return names.join("/") + portName;
}

QString Node::className(Namespace ns) const {
    return _pedigree.isEmpty() ?
            ( (ns == Namespace::Include) ? "base::Node" : "Node" ) :
            _pedigree.last().toText(ns);
}

QString Node::Type::toText(Namespace ns) const {
    return (ns == Namespace::Include) ? (namespaceName + "::" + className) : className;
}

QString Node::pedigreeAsString(Namespace ns) const {
    QStringList list;
    for (auto type : _pedigree)
        list << type.toText(ns);
    return list.join(" -> ");
}

bool Node::isType(QString name) const {
    QStringList names = name.split("::");
    bool found = false;
    auto it = _pedigree.begin();
    const auto end = _pedigree.end();
    if (names.size() == 1) {
        while (!found && it!=end) {
            found = (it->className == name);
            ++it;
        }
    }
    else if (names.size() == 2) {
        while (!found && it!=end) {
            found = (it->namespaceName == names.at(0) && it->className == names.at(1));
            ++it;
        }
    }
    return found;
}

template <> QVector<Box*> Node::children() {
    QVector<Box*> result;
    for (auto child : _children) {
        Box* res = dynamic_cast<Box*>(child);
        if (res) result << res;
    }
    return result;
}

}
