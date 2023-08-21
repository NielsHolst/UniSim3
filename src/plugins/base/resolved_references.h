/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_RESOLVED_REFERENCES_H
#define BASE_RESOLVED_REFERENCES_H
#include <memory>
#include <QHash>
#include <QStringList>
#include <QVector>
#include "computation.h"
#include "exception.h"

namespace base {

class Box;
class Node;
class Port;

template <class T>
struct ResolvedReferenceKey {
    const Node *referee;
    const T *reference;
};

class ResolvedReferences {
public:
    static void clear();
    static void add(const Node *referee, QVector<Port*> ports);
    static void add(const Node *referee, QVector<Box*>  boxes);
    static void check();
    static bool fixed();
    static bool checkOrder();
//    static QStringList dump(QString text);
private:
    ResolvedReferences() {}
    static std::unique_ptr<ResolvedReferences> _singleton;
    static ResolvedReferences& my();

    template <class T>
    struct References {
        // Data
        QHash<ResolvedReferenceKey<T>, Computation::Step> table;
        int sizeAtLatestCheck;
        bool isFixed;
        // Methods
        References<T>() { clear(); }
        void clear();
        void check();
        void add(const Node *referee, QVector<T*> references);
    };
    References<Port> _ports;
    References<Box> _boxes;
    bool _orderChecked;

};

template <class T>
void ResolvedReferences::References<T>::clear() {
    table.clear();
    sizeAtLatestCheck = 0;
    isFixed = false;
}

template <class T>
void ResolvedReferences::References<T>::check() {
    if (!isFixed) {
        if (table.size() == sizeAtLatestCheck)
            isFixed = true;
        else
            sizeAtLatestCheck = table.size();
    }
}

template <> void ResolvedReferences::References<Port>::add(const Node *referee, QVector<Port*> references);
template <> void ResolvedReferences::References<Box >::add(const Node *referee, QVector<Box* > references);

size_t qHash(const ResolvedReferenceKey<Port> &key);
bool operator==(const ResolvedReferenceKey<Port> &a, const ResolvedReferenceKey<Port> &b);

size_t qHash(const ResolvedReferenceKey<Box> &key);
bool operator==(const ResolvedReferenceKey<Box> &a, const ResolvedReferenceKey<Box> &b);

}

#endif
