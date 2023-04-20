/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "extract_layers.h"


namespace vg {

QStringList extractLayers(QStringList formulas) {
    using Members = QStringList;
    using Groups = QVector<Members>;
    Groups groups;

    for (const QString formula : formulas) {
        Members members = formula.split("+");
        groups << members;
    }
}

} //namespace

