/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QMap>
#include "dialog.h"
#include "environment.h"
#include "exception.h"
#include "computation.h"
#include "resolved_references.h"

namespace base {

Computation::Step Computation::_currentStep = Computation::Step::Ready;
QStack<Computation::Step> Computation::_stack;

Computation::Step Computation::lookup(QString step, Node *context) {
    static QMap<QString, Computation::Step> map =
    {
        {"ready"     , Step::Ready     },
        {"construct" , Step::Construct },
        {"amend"     , Step::Amend     },
        {"initialize", Step::Initialize},
        {"reset"     , Step::Reset     },
        {"update"    , Step::Update    },
        {"cleanup"   , Step::Cleanup   },
        {"debrief"   , Step::Debrief   },
        {"scratch"   , Step::Scratch   }
    };
    if (!map.contains(step)) {
        QStringList list = map.keys();
        QString s = list.join(", ");
        ThrowException("Unknown computation step").value(step).hint(s).context(context);
    }
    return map.value(step);
}

QString Computation::toString(Computation::Step step) {
    static QMap<Computation::Step, QString> map =
    {
        {Step::Ready     , "ready"     },
        {Step::Construct , "construct" },
        {Step::Amend     , "amend"     },
        {Step::Initialize, "initialize"},
        {Step::Reset     , "reset"     },
        {Step::Update    , "update"    },
        {Step::Cleanup   , "cleanup"   },
        {Step::Debrief   , "debrief"   },
        {Step::Scratch   , "scratch"   }
    };
    return map.value(step);
}

void Computation::changeStep(Step step) {
    static QMap<Computation::Step, QString> map =
    {
        {Step::Ready     , "Ready..."       },
        {Step::Construct , "Constructing..."},
        {Step::Amend     , "Amending..."    },
        {Step::Initialize, "Initializing..."},
        {Step::Reset     , "Resetting..."   },
        {Step::Update    , "Updating..."    },
        {Step::Cleanup   , "Cleaning up..." },
        {Step::Debrief   , "Debriefing..."  },
        {Step::Scratch   , "Scratch..."   }
    };
    if (step != Step::Ready && !environment().isSilent())
        dialog().information(map.value(step));
    _currentStep = step;
    if (step <= Step::Amend)
        ResolvedReferences::clear();
}

Computation::Step Computation::currentStep() {
    return _currentStep;
}

void Computation::pushStep(Step step) {
    _stack.push(_currentStep);
    _currentStep = step;
}

void Computation::popStep() {
    _currentStep = _stack.pop();
}

}
