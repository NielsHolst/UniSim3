/* Copyright 2019-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Teis Boderskov, Aarhus University [tebo atbios.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include <base/test_num.h>
#include "proportions.h"

using namespace base;

namespace saccharina {

PUBLISH(Proportions)

Proportions::Proportions(QString name, Box *parent)
    : Box(name, parent)
{
    help("holds plant proportion parameters");
    Input(concStructureC).unit("g C/g dw").help("Concentration of carbon in structure");
    Input(concStructureN).unit("g N/g dw").help("Concentration of nitrogen in structure");
    Input(concStoreC).unit("g C/g dw").help("Concentration of carbon in structure");
    Input(concStoreN).unit("g N/g dw").help("Concentration of nitrogen in nitrogen store");
    Input(concStoreP).unit("g P/g dw").help("Concentration of phosphorus in phosphorus store");
    Input(concWholeCmax).unit("g C/g dw").help("Concentration of carbon in whole plant");
    Input(concWholeNmax).unit("g N/g dw").help("Concentration of nitrogen in whole plant");
    Input(concWholePmax).unit("g P/g dw").help("Concentration of carbon in whole plant");
    Output(idealStructure).unit("[0;1]").help("Ideal proportion of dry weight in structure");
    Output(idealStoreC).unit("[0;1]").help("Ideal proportion of dry weight in carbon store");
    Output(idealStoreN).unit("[0;1]").help("Ideal proportion of dry weight in nitrogen store");
    Output(idealStoreP).unit("[0;1]").help("Ideal proportion of dry weight in phosphorus store");
}

void Proportions::reset() {
    // Equations for ideal proportions were copied from the saccharina-solve-concentrations.R script
    // Parameter names equal LaTex variables in documentation
    double
        &cSC(concStructureC),
        &cCC(concStoreC),
        &cSN(concStructureN),
        &cNN(concStoreN),
        &cPP(concStoreP),
        &cWCmax(concWholeCmax),
        &cWNmax(concWholeNmax),
        &cWPmax(concWholePmax),
        k = cSC*cNN*cPP + cCC*cSN*cPP - cCC*cNN*cPP;
    idealStructure = (cCC*cNN*cWPmax - cCC*cNN*cPP + cCC*cPP*cWNmax + cNN*cPP*cWCmax)/k;
    idealStoreC = (cSC*cNN*cPP - cSC*cNN*cWPmax - cSC*cPP*cWNmax + cSN*cPP*cWCmax - cNN*cPP*cWCmax)/k;
    idealStoreN = (cSC*cPP*cWNmax + cCC*cSN*cPP - cCC*cSN*cWPmax - cCC*cPP*cWNmax - cSN*cPP*cWCmax)/k;
    idealStoreP = cWPmax/cPP;
    // Check sum
    double sum = idealStructure + idealStoreC + idealStoreN + idealStoreP;
    if (TestNum::ne(sum, 1.)) {
        ThrowException("Ideal proportions must add up to 1").value(sum).context(this);
    }
}


}
