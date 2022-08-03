/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QMap>
#include <base/exception.h>
#include <base/publish.h>
#include "output_selector.h"

using namespace base;

namespace boxes {

PUBLISH(OutputSelector)

OutputSelector::OutputSelector(QString name, Box *parent)
    : Box(name, parent)
{
    help("selects what to output");
    Input(beginStep).equals(0).help("Output is written then this step is reached").unit("int");
    Input(beginDateTime).equals(QDateTime()).help("Output is written then this time point is reached (optional)").unit("DateTime");
    Input(step).imports("/.[step]");
    Input(dateTime).computes("if exists(Calendar::*[dateTime]) then Calendar::*[dateTime] else .[beginDateTime]");
    Input(period).equals(1).help("If >1: A row of summary output will be produced with this period").unit("int>0");
    Input(final).equals(false).help("Overrules 'period'").unit("bool");
    Input(useLocalDecimalChar).equals(false).help("Use local decimal character in output?").unit("bool");
    Input(skipFormats)
            .computes("!exists(OutputR::*[skipFormats])")
            .help("Skip line with column formats?").unit("bool");
    Output(isActive).help("Should output be written?").unit("bool");
    Output(isSkipping).help("Are lines being skipped?").unit("bool");
}

void OutputSelector::initialize() {
    updateSkipping();
    isActive = false;
}

void OutputSelector::reset() {
    _totalRowCount =
    _periodCount   = 0;
    update();
}

void OutputSelector::update() {
    updateSkipping();
    isActive = !(isSkipping || final) ? (_periodCount++ % period == 0) : false;
}

void OutputSelector::cleanup() {
    isSkipping = false;
    isActive = final;
}

void OutputSelector::debrief() {
    isSkipping = false;
    isActive = false;
}

void OutputSelector::updateSkipping() {
    bool dateReached = beginDateTime.isValid() && dateTime < beginDateTime;
    isSkipping = (step < beginStep && !dateReached);
}

}
