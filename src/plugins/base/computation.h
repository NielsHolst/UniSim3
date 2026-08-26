/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef BASE_COMPUTATIONSTEP_H
#define BASE_COMPUTATIONSTEP_H
#include <QStack>
#include <QString>

#include "base_dllexport.h"

namespace base {

class Node;

// @TODO(bjarke): re-check this one
class Computation {
public:
    enum class Step {Ready, Construct, Modify, Amend, Initialize, Reset, Update, Cleanup, Debrief, Scratch};
    UNI_API static Step lookup(QString step, Node *context=nullptr);
    UNI_API static QString toString(Step step);
    UNI_API static void changeStep(Step step);
    UNI_API static Step currentStep();

    UNI_API static void pushStep(Step step);
    UNI_API static void popStep();

private:
    static Step _currentStep;
    static QStack<Step> _stack;
    static bool _hasShownConstruct, _hasShownAmend;
};

inline bool operator<(Computation::Step a, Computation::Step b)
    { return static_cast<int>(a) < static_cast<int>(b); }

inline bool operator<=(Computation::Step a, Computation::Step b)
    { return static_cast<int>(a) <= static_cast<int>(b); }

inline bool operator>(Computation::Step a, Computation::Step b)
    { return static_cast<int>(a) > static_cast<int>(b); }

inline bool operator>=(Computation::Step a, Computation::Step b)
    { return static_cast<int>(a) >= static_cast<int>(b); }

}

#endif
