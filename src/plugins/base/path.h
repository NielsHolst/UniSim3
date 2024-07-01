/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef BASE_PATH_H
#define BASE_PATH_H
#include <algorithm>
#include <optional>
#include <string>
#include <vector>
#include <QSet>
#include <QVector>
#include "exception.h"
#include "node.h"
#include "port_type.h"

namespace base {

class Expression;
class Port;

class Path {
public:
    // Types
    using Object  = base::Node;
    using Objects = QList<base::Node*>;

    enum class Directive {
        Any, Children, Descendants, Ancestors, Siblings, Preceding, Following
    };
    // Directive can be Any only for the first Node in an Alternative

    class Alternative;

    class Node {
    public:
        Node(QString directive, QString className, QString objectName);
        Node(std::vector<std::string> names);
        QString toString() const;
        Directive directive() const { return _directive; }
        QString className() const { return _className; }
        QString objectName() const { return _objectName; }
    private:
        Directive _directive;
        QString _className, _objectName;
        friend Alternative;
    };

    class Port {
    public:
        Port(QString directive, QString name);
        Port(std::vector<std::string> names);
        QString name() const { return _name; }
        bool hasDirective() const { return _directive.has_value(); }
        PortType directive() const { return *_directive; }
        bool matches(const base::Port *port);
        QString toString() const;
    private:
        std::optional<PortType> _directive;
        QString _name;
        friend Alternative;
    };

    class Alternative {
    public:
        Alternative();
        void setRoot(bool hasRoot);
        void addNode(Node node);
        void setPort(const Port &port);
        bool hasRoot() const;
        const QVector<Node> &nodes() const;
        const std::optional<Port> &port() const;
        const Objects& matches(base::Box *parent);
        QString toString() const;

    private:
        // Data
        bool _hasRoot;
        QVector<Node> _nodes;
        std::optional<Port> _port;
        Objects _matches;

        // Methods
        void initiateMatches(const Node &node, base::Node *parent);
        void filterMatches  (const Node &node);

        Objects any(Object *p, QString className, QString objectName);

        Objects descendants(Object *p, QString className, QString objectName);
        Objects descendants(Objects candidates, QString className, QString objectName);

        Object* parent (Object *p, QString className, QString objectName);
        Objects parents(Objects candidates, QString className, QString objectName);

        Objects ancestors(Object *p, QString className, QString objectName);
        Objects ancestors(Objects candidates, QString className, QString objectName);

        Objects children(Object *p, QString className, QString objectName);
        Objects children(Objects candidates, QString className, QString objectName);

        Objects siblings(Object *p, QString className, QString objectName);
        Objects siblings(Objects candidates, QString className, QString objectName);

        Object* preceding(Object *p, QString className, QString objectName);
        Objects preceding(Objects candidates, QString className, QString objectName);

        Object* following(Object *p, QString className, QString objectName);
        Objects following(Objects candidates, QString className, QString objectName);
    };

    //
    // Path
    //

    // Methods
    Path(base::Port *parent=nullptr);
    Path(QString path, base::Port *parent=nullptr);
    Path(const Expression *expression);
    void setParent(base::Port *parent);
    base::Port *parent();
    void add(const Alternative &alternative);
    void add(const Path &path);
    QString toString() const;
    const QVector<Alternative> &alternatives() const;
    const Objects& matches(base::Node *anchor = nullptr) const;
    template<class T> T findOne(base::Node *anchor = nullptr) const;
    template<class T> T findMaybeOne(base::Node *anchor = nullptr) const;
    template<class T> QVector<T> findMany(base::Node *anchor = nullptr) const;
    template<class T> QStringList findManyNames(base::Node *anchor = nullptr) const;
    QSet<base::Node*> nodesOnPath() const;
    static bool isValid(QString path);
private:
    // Data
    base::Port *_parent;
    mutable QVector<Alternative> _alternatives;
    mutable Objects _matches;
    // Methods
    void initialize(Expression expression);
};

template<class T> T Path::findOne(base::Node *anchor) const {
    QVector<T> result = findMany<T>(anchor);
    if (result.size() != 1) {
        ThrowException("Expected exactly one match").value(toString()).
                hint("Nodes found:\n" + findManyNames<T>(anchor).join("\n")).
                context(_parent);
    }
    return result[0];
}

template<class T> T Path::findMaybeOne(base::Node *anchor) const {
    QVector<T> result = findMany<T>(anchor);
    switch (result.size()) {
    case 0: return nullptr;
    case 1: return result[0];
    default: ThrowException("Expected at most one match").value(toString()).
                hint("Nodes found:\n" + findManyNames<T>(anchor).join("\n")).
                context(_parent);
    }
}

template<class T> QVector<T> Path::findMany(base::Node *anchor) const {
    QVector<T> result;
    for (auto candidate : matches(anchor)) {
        auto typed = dynamic_cast<T>(candidate);
        if (typed)
            result << typed;
    }
    std::sort(result.begin(), result.end(),  [](T a, T b) { return a->order() < b->order(); });
    return result;
}

template<class T> QStringList Path::findManyNames(base::Node *anchor) const {
    QVector<T> results = findMany<T>(anchor);
    QStringList names;
    for (auto result : results)
        names << result->fullName();
    return names;
}

template<> QVector<base::Port*> Path::findMany(base::Node *anchor) const;

inline uint qHash(const base::Node* node) {
    return node->order();
}


}

#endif
