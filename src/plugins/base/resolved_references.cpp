/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <iostream>
#include "box.h"
#include "dialog.h"
#include "node.h"
#include "phys_math.h"
#include "port.h"
#include "resolved_references.h"

namespace base {

std::unique_ptr<ResolvedReferences> ResolvedReferences::_singleton;

// Functions

ResolvedReferences& ResolvedReferences::my() {
    if (!_singleton)
        _singleton = std::unique_ptr<ResolvedReferences>( new ResolvedReferences );
    return *_singleton;
}

void ResolvedReferences::clear() {
    my()._ports.clear();
    my()._boxes.clear();
    my()._orderChecked = false;
}

void ResolvedReferences::add(const Node *referee, QVector<Port*> ports) {
    my()._ports.add(referee, ports);
}

void ResolvedReferences::add(const Node *referee, QVector<Box*>  boxes) {
    my()._boxes.add(referee, boxes);
}

void ResolvedReferences::check() {
    my()._ports.check();
    my()._boxes.check();
}

bool ResolvedReferences::fixed() {
    bool
            portsFixed = my()._ports.isFixed,
            boxesFixed = my()._boxes.isFixed,
            isFixed = portsFixed && boxesFixed;
    if (isFixed && !my()._orderChecked) {
        my().checkOrder();
        my()._orderChecked = true;
    }
    return isFixed;
}

bool ResolvedReferences::checkOrder() {
  // TODO
    // QHashIterator<ResolvedReferenceKey<Port>, Computation::Step> it(my()._ports.table);
    // while (it.hasNext()) {
        // it.next();
        // auto referee = it.key().referee;
        // auto reference = it.key().reference;
        // if (referee->order() < reference->order() && !reference->isConstant()) {
            // QString line = referee->fullName() + " (" + QString::number(referee->order()) +
                           // ") refers to " +
                           // reference->fullName() + " (" + QString::number(reference->order()) + ")";
            // dialog().information(line);
        // }
    // }

    return true;
}

//QStringList ResolvedReferences::dump(QString text) {
//    std::cout << qPrintable(text) << " dump: " << my()._table.size() << std::endl;
//    QStringList list;
//    auto it = table.constBegin();
//    while (it != table.constEnd()) {
//        const Node *referee   = it.key().referee;
//        const Port *reference = it.key().reference;
//        QString step = Computation::toString(it.value());
//         list << (referee->fullName() + " => " + reference->fullName() + " -- " + step);
//        ++it;
//    }
//    std::cout << qPrintable(list.join("\n")) << std::endl;
//    return list;
//}

template <>
void ResolvedReferences::References<Port>::add(const Node *referee, QVector<Port*> references) {
    for (auto port : references) {
        if (!port->value().isNull()) {
            auto ref = ResolvedReferenceKey<Port> {referee, port};
            table[ref] = Computation::currentStep();
        }
    }
}

template <>
void ResolvedReferences::References<Box>::add(const Node *referee, QVector<Box*> references) {
    for (auto box : references) {
        if (box) {
            auto ref = ResolvedReferenceKey<Box> {referee, box};
            table[ref] = Computation::currentStep();
        }
    }
}

size_t qHash(const ResolvedReferenceKey<Port> &key) {
    return phys_math::hashPointers(key.referee, key.reference);
}

bool operator==(const ResolvedReferenceKey<Port> &a, const ResolvedReferenceKey<Port> &b) {
    return a.referee==b.referee && a.reference==b.reference;
}

size_t qHash(const ResolvedReferenceKey<Box> &key) {
    return phys_math::hashPointers(key.referee, key.reference);
}

bool operator==(const ResolvedReferenceKey<Box> &a, const ResolvedReferenceKey<Box> &b) {
    return a.referee==b.referee && a.reference==b.reference;
}

}
