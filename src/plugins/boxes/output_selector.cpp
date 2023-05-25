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
    Input(beginStep).equals(0).help("Output is written when this step is reached").unit("int");
    Input(beginDateTime).equals(QDateTime()).help("Output is written when this time point is reached (optional)").unit("DateTime");
    Input(step).imports("/.[step]");
    Input(dateTime).imports("Calendar::*[dateTime]");
    Input(period).equals(1).help("If >1: A row of output will be produced with this period").unit("int>0");
    Input(summary).equals("average").unit("string").help("If period>1 then either'current or 'average' value");
    Input(final).equals(false).help("Overrules 'period'").unit("bool");
    Input(useLocalDecimalChar).equals(false).help("Use local decimal character in output?").unit("bool");
    Input(skipFormats).computes("!exists(OutputR::*[skipFormats])").unit("bool");
    Output(isActive).help("Should output be written?").unit("bool");
    Output(isSkipping).help("Are lines being skipped?").unit("bool");
}

void OutputSelector::initialize() {
    if (!findMaybeOne<Port*>("Calendar::*[dateTime]")) {
        dateTime = beginDateTime;
        port("dateTime")->setConstness(true);
    }
    if (summary != "average" && summary != "current")
        ThrowException("Summary must be either 'avearge' or 'current'").context(this);
    updateSkipping();
    isActive = false;
}

void OutputSelector::reset() {
    if (summary == "average" && period == 1)
        summary = "current";
    _totalRowCount =
    _periodCount   = 0;
    update();
}

void OutputSelector::update() {
    updateSkipping();
    isActive = (isSkipping || final) ? false : (_periodCount++ % period == 0);
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
    bool beforeBeginStep = (beginStep > 0) && (step < beginStep),
         beforebeginDate = beginDateTime.isValid() && (dateTime < beginDateTime);
    isSkipping = (beforeBeginStep || beforebeginDate);
}

}
