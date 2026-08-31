/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef SPLIT_H
#define SPLIT_H

#include <base/base_dllexport.h>
#include <base/box.h>

namespace boxes {

class UNI_API Split : public base::Box
{
public: 
    Split(QString name, Box *parent);
    void amend() override;
    void initialize() override;
    void reset() override;
    void update() override;
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
