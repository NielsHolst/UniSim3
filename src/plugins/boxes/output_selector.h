/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef OUTPUT_SELECTOR_H
#define OUTPUT_SELECTOR_H

#include <QDateTime>

#include <base/box.h>
#include <base/base_dllexport.h>

namespace boxes {

class UNI_API OutputSelector : public base::Box
{
public:
    OutputSelector(QString name, Box *parent);
    void initialize() override;
    void reset() override;
    void update() override;
    void cleanup() override;
    void debrief() override;
private:
    // Inputs
    int step, beginStep, period;
    QDateTime dateTime, beginDateTime;
    QString summary;
    bool final, useLocalDecimalChar, skipFormats;
    // Outputs
    bool isActive, isSkipping;
    // Data
    int _totalRowCount,
        _periodCount;
    // Methods
    void updateSkipping();
};

} // boxes

#endif
