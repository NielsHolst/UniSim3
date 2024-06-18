/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_XML_NODE_H
#define BASE_XML_NODE_H

#include <QMap>
#include <QMultiMap>
#include <QString>
#include <QVector>
#include "exception.h"

namespace base {

class XmlNode
{
public:
    enum class NoMatch{Accept, Error} ;
    XmlNode(QString name, XmlNode *parent);
    ~XmlNode();
    XmlNode *cutRoot();
    void addAttribute(QString name, QString value);
    void setValue(QString value);
    QString name() const;
    QString fullName() const;

    QString value() const;
    bool toBool() const;
    int toInt() const;
    double toDouble() const;

    bool hasAttribute(QString name) const;
    QString getAttributeString(QString name) const;
    int getAttributeInt(QString name) const;
    double getAttributeDouble(QString name) const;

    XmlNode* peak(QString path);
    XmlNode* find(QString path);
    XmlNode* parent();
    QMultiMap<QString, XmlNode *> &children(QString childName="", NoMatch noMatch=NoMatch::Error);
private:
    // Data
    QString _name, _value;
    XmlNode* _parent;
    QMultiMap<QString, XmlNode*> _children, _subset;
    QMap<QString, QString> _attributes;
    // Methods
    void addChild(XmlNode *child);
};

}

#endif
