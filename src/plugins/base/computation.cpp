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
bool Computation::_hasShownConstruct = false,
     Computation::_hasShownAmend = false;

Computation::Step Computation::lookup(QString step, Node *context) {
    static QMap<QString, Computation::Step> map =
    {
        {"ready"     , Step::Ready     },
        {"construct" , Step::Construct },
        {"modify"    , Step::Modify    },
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
        {Step::Modify    , "modify"    },
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
        {Step::Modify    , "Modifying..."   },
        {Step::Amend     , "Amending..."    },
        {Step::Initialize, "Initializing..."},
        {Step::Reset     , "Resetting..."   },
        {Step::Update    , "Updating..."    },
        {Step::Cleanup   , "Cleaning up..." },
        {Step::Debrief   , "Debriefing..."  },
        {Step::Scratch   , "Scratch..."   }
    };
    if (!(environment().isSilent())) {
        bool messageOnly = (step == Step::Construct && _hasShownConstruct) ||
                           (step == Step::Amend && _hasShownAmend) ||
                           (step == Step::Modify) ||
                           (step == Step::Ready) ||
                           (step == Step::Scratch);
        dialog().message(map.value(step));
        if (!messageOnly) {
            dialog().information(map.value(step));
            if (step == Step::Construct)
                _hasShownConstruct = true;
            if (step == Step::Amend)
                _hasShownAmend = true;
        }
    }
    _currentStep = step;
    if (step == Step::Ready) {
        _hasShownConstruct = false;
        _hasShownAmend = false;
    }
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
