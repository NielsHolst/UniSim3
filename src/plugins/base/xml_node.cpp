/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "exception.h"
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
    while (child != _children.end())
        delete child.value();
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

QString XmlNode::value() const {
    return _value;
}

XmlNode* XmlNode::parent() {
    return _parent;
}

QMap<QString, XmlNode*> & XmlNode::children() {
    return _children;
}

XmlNode* XmlNode::find(QString path) {
    QStringList names = path.split(("/"));
    XmlNode *node = this;
    for (auto &name : names) {
        auto child = node->_children.find(name);
        if (child != node->_children.end())
            node = *child;
        else
            ThrowException("Cannot find child XML element of that name").value("Path: "+path).value2("Child: "+name);
    }
    return node;
}

void XmlNode::addChild(XmlNode *child) {
    _children[child->_name] = child;
}

} // namespace
