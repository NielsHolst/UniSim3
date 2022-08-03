/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef COMBINATIONS_H
#define COMBINATIONS_H
#include <base/box.h>

namespace boxes {

class Combinations : public base::Box
{
public: 
    Combinations(QString name, Box *parent);
    void amend();
    void initialize();
    void cleanup();
    static QString counterName(const Box *box);
private:
    // Outputs
    QVector<int> counters;
    int iterations;
    // Data
    QVector<Box*> children;
    QVector<int> n;
};

} //namespace
#endif
