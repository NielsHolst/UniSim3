#include <cmath>
#include "proportions.h"

namespace base {

Proportions::Proportions(base::Node *parent)
    : base::Node("proportions", parent), QObject(nullptr), root(nullptr)
{
    root = new Node(nullptr, 0, 1., true);
    root->setParent(this);
    leaves << root;
    depth = 0;
}

void Proportions::add(double proportion) {
    QVector<Node*> newLeaves;
    ++depth;
    for (Node *node : leaves) {
        newLeaves << new Node(node, depth, proportion, true)
                  << new Node(node, depth, proportion, false);
    }
    leaves = newLeaves;
}

const QVector<double>& Proportions::reconciled() {
    // Result contains as many proportions as added
    _reconciled.fill(0., depth);
    // For each leaf
    for (const Node *node : leaves) {
        NodeSet whichOn = node->whichOn();
        double p = node->product()/whichOn.size();
        // Add additional proportion for those on the path
        int i = 0;
        for (double &rec : _reconciled) {
            if (whichOn.contains(++i))
                rec += p;
        }
    }
    return _reconciled;
}

Proportions::Node::Node(Node *parent, int depth, double proportion, bool on)
    : QObject(parent),
      _nodeParent(parent), _depth(depth), _proportion(proportion), _on(on),
      _whichOnUpdated(false)
{
}

Proportions::NodeSet Proportions::Node::whichOn() const {
    if (!_whichOnUpdated) {
        const Node *node = this;
        do {
            if (node->_on)
                _whichOn << node->_depth;
            node = node->nodeParent();
        } while (node->nodeParent());
        _whichOnUpdated = true;
    }
    return _whichOn;
}

double Proportions::Node::product() const {
    double p = 1.;
    const Node *node = this;
    do {
        p *= node->_on ? node->_proportion : 1. - node->_proportion;
        node = node->nodeParent();
    } while (node->nodeParent());
    return p;
}

void Proportions::Node::x() {
}

} // namespace
