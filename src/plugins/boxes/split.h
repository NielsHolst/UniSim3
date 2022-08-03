/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SPLIT_H
#define SPLIT_H
#include <base/box.h>

namespace boxes {

class Split : public base::Box
{
public: 
    Split(QString name, Box *parent);
    void amend();
    void initialize();
    void reset();
    void update();
private:
    // Inputs
    double whole;
    QVector<double> proportions;
    // Outputs
    QVector<double> pieces;
    // Data
    int n;
};

} //namespace
#endif
