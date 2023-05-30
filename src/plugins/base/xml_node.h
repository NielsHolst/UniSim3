/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_XML_NODE_H
#define BASE_XML_NODE_H

#include <QMap>
#include <QString>
#include <QVector>

namespace base {

class XmlNode
{
public:
    XmlNode(QString name, XmlNode *parent);
    ~XmlNode();
    XmlNode* detachChild();
    void addAttribute(QString name, QString value);
    void setValue(QString value);
    QString value() const;
    XmlNode* find(QString path);
    XmlNode* parent();
    QMap<QString, XmlNode*> & children();
private:
    // Data
    QString _name, _value;
    XmlNode* _parent;
    QMap<QString, XmlNode*> _children;
    QMap<QString, QString> _attributes;
    // Methods
    void addChild(XmlNode *child);
};

}

#endif
