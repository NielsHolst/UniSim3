/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "xml_node.h"

namespace base {

XmlNode::XmlNode(QString name, XmlNode *parent)
    : _name(name), _parent(parent)
{
    if (parent)
        parent->addChild(this);
}

XmlNode::~XmlNode() {
    auto child = _children.begin();
    while (child != _children.end()) {
        delete child.value();
        ++child;
    }
}

XmlNode* XmlNode::detachChild() {
    if (_children.size() != 1)
        ThrowException("Only one child expexted").value(_name);
    XmlNode *first = _children.values()[0];
    _children.clear();
    return first;
}

void XmlNode::addAttribute(QString name, QString value) {
    _attributes[name] = value;
}

void XmlNode::setValue(QString value) {
    _value = value;
}

QString XmlNode::name() const {
    return _name;
}

QString XmlNode::value() const {
    return _value;
}

bool XmlNode::toBool() const {
    return (toInt() != 0);
}

int XmlNode::toInt() const {
    bool ok;
    int value = _value.toInt(&ok);
    if (!ok)
        ThrowException("Integer value expected").value(_name);
    return value;
}

double XmlNode::toDouble() const {
    bool ok;
    double value = _value.toDouble(&ok);
    if (!ok)
        ThrowException("Number value expected").value(_name);
    return value;
}

QString XmlNode::getAttributeString(QString name) const {
    return _attributes.value(name, "");
}

int XmlNode::getAttributeInt(QString name) const {
    QString s = getAttributeString(name);
    bool ok;
    int value = s.toInt(&ok);
    if (!ok)
        ThrowException("Attribute integer value expected").value("Node: " + _name).value2("Attribute: " + name);
    return value;
}

double XmlNode::getAttributeDouble(QString name) const {
    QString s = getAttributeString(name);
    bool ok;
    double value = s.toDouble(&ok);
    if (!ok)
        ThrowException("Attribute numerical expected").value("Node: " + _name).value2("Attribute: " + name);
    return value;
}

XmlNode* XmlNode::parent() {
    return _parent;
}

QMultiMap<QString, XmlNode*> & XmlNode::children(QString childName) {
    if (childName.isEmpty())
        return _children;
    else if (!_children.contains(childName))
        ThrowException("XML node has no child with that name").value(_name).value2(childName);

    _subset.clear();
    for (auto child = _children.lowerBound(childName); child != _children.upperBound(childName); ++child)
        _subset.insert(child.key(), child.value());
    return _subset;
}

XmlNode* XmlNode::peak(QString path) {
    QStringList names = path.split(("/"));
    XmlNode *node = this;
    for (auto &name : names) {
        int n = node->_children.values(name).size();
        if (n > 1)
            ThrowException("XML path is not unique").value(path).value2(n);
        auto child = node->_children.find(name);
        if (child != node->_children.end())
            node = *child;
        else
            return nullptr;
    }
    return node;
}

XmlNode* XmlNode::find(QString path) {
    XmlNode *node = peak(path);
    if (!node)
        ThrowException("Cannot find child XML element of that name").value(path);
    return node;
}

void XmlNode::addChild(XmlNode *child) {
    _children.insert(child->_name, child);
}

} // namespace
