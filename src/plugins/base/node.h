/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef BASE_NODE_H
#define BASE_NODE_H

#include <QMap>
#include <QString>
#include <QVector>

#include "base_dllexport.h"

#define Doc(X) setDoc(X); setSourceCodeFilePathName(__FILE__)

namespace base {

class Box;

class Node
{
public:
    enum class Namespace{Include, Exclude};

    struct Type {
        QString namespaceName, className;
        QString toText(Namespace ns = Namespace::Exclude) const;
    };
    using Pedigree = QVector<Type>;

    UNI_API Node(QString name, Node *parent);
    UNI_API virtual ~Node();
    UNI_API void setParent(Node *parent);
    UNI_API void setPluginName(QString pluginName);
    UNI_API void setClassName(QString nameSpaceName, QString className);
    UNI_API void setObjectName(QString name);
    UNI_API void setDoc(QString doc);
    UNI_API void setSourceCodeFilePathName(QString name);

    UNI_API void setOrder(int order);
    UNI_API void addAttribute(QString name, QString value);
    UNI_API void removeChild(Node *child);

    UNI_API static void enumerate();

    template <class T=Node*> T parent() const;
    template <class T=Node*> QVector<T> children();
    template <class T=Node*> QVector<T> descendants();

    UNI_API QString name()       const { return _objectName; }
    UNI_API QString objectName() const { return _objectName; }
    UNI_API QString fullName()   const { return fullName(this); }
    UNI_API QString pluginName() const { return _pluginName; }
    UNI_API QString doc()        const { return _doc; }
    UNI_API QString sourceCodeFilePathName() const;
    UNI_API QStringList sourceCodeFilePathNames() const { return _sourceCodeFilePathNames; }

    UNI_API static QString fullName (const Node *node);
    template <class T=Node*> static QStringList fullNames(const QVector<const T *> &nodes);
    template <class T=Node*> static QStringList names    (const QVector<const T *> &nodes);
    template <class T=Node*> static QStringList fullNames(const QVector<      T *> &nodes);
    template <class T=Node*> static QStringList names    (const QVector<      T *> &nodes);

    UNI_API QString className(Namespace ns = Namespace::Exclude) const;
    UNI_API const Pedigree & pedigree() const { return _pedigree; }
    UNI_API QString pedigreeAsString(Namespace ns = Namespace::Exclude) const;
    UNI_API bool isType(QString name) const;

    UNI_API int order() const       { return _order; }
    UNI_API const QMap<QString, QString> & attributes() const { return _attributes; }

    UNI_API bool doWriteOnCommand() const { return _doWriteOnCommand; }
    UNI_API void doWriteOnCommand(bool doWrite) { _doWriteOnCommand = doWrite; }

    UNI_API static void setCurrent(Node const* node);
    UNI_API static const Node *current();

private:
    // Data
    Pedigree _pedigree;
    QString _objectName, _pluginName, _doc;
    QStringList _sourceCodeFilePathNames;
    Node *_parent;
    QVector<Node*> _children;
    int _order;
    QMap<QString, QString> _attributes;
    bool _doWriteOnCommand;
    static Node const* _current;

    // Methods
    void addChild(Node *child);
    void enumerate(int &i);
    template <class T=Node*> QVector<T> descendants(bool includeMe);
};

template <class T> T Node::parent() const {
    return dynamic_cast<T>(_parent);
}

template <class T> QVector<T> Node::children() {
    QVector<T> result;
    for (auto child : _children) {
        T res = dynamic_cast<T>(child);
        if (res) result << res;
    }
    return result;
}

template <> inline QVector<Node*> Node::children() {
    return _children;
}

template <> QVector<Box*> Node::children();

template <class T> QVector<T> Node::descendants() {
    return descendants<T>(false);
}

template <class T> QVector<T> Node::descendants(bool includeMe) {
    QVector<T> result;
    for (auto child : _children)
        result.append(child->descendants<T>(true));
    if (includeMe) {
        T res = dynamic_cast<T>(this);
        if (res) result << res;
    }
    return result;
}

template <class T> QStringList Node::fullNames(const QVector<const T *> &nodes) {
    QStringList list;
    for (auto node : nodes)
        list << node->fullName();
    return list;
}

template <class T> QStringList Node::names(const QVector<const T *> &nodes) {
    QStringList list;
    for (auto node : nodes)
        list << node->name();
    return list;
}

template <class T> QStringList Node::fullNames(const QVector<T *> &nodes) {
    QStringList list;
    for (auto node : nodes)
        list << node->fullName();
    return list;
}

template <class T> QStringList Node::names(const QVector<T *> &nodes) {
    QStringList list;
    for (auto node : nodes)
        list << node->name();
    return list;
}

inline bool operator<(const Node &a, const Node &b) {
    return a.order() < b.order();
}

inline bool operator==(const Node::Type &a, const Node::Type &b) {
    return a.namespaceName==b.namespaceName && a.className==b.className;
}

}
#endif
