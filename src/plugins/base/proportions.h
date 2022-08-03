/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_PROPORTIONS_H
#define BASE_PROPORTIONS_H
#include <QObject>
#include <QSet>
#include <QVector>
#include "node.h"

namespace base {

class Proportions : public base::Node, QObject {
public:
    Proportions(base::Node *parent);
    void add(double proportion);
    const QVector<double> &reconciled();
private:
    typedef QSet<int> NodeSet;
    class Node : public QObject {
    public:
        Node() {}
        Node(Node *parent, int depth, double proportion, bool on);
        const Node* nodeParent() const {return _nodeParent;}
        NodeSet whichOn() const;
        double product() const;
        int depth() const { return _depth; }
        virtual void x();
    private:
        const Node* _nodeParent;
        int _depth;
        double _proportion;
        bool _on;
        mutable Proportions::NodeSet _whichOn;
        mutable bool _whichOnUpdated;
    };
    QVector<Node*> leaves;
    Node *root;
    QVector<double> _reconciled;
    int depth;
};

} //namespace
#endif
