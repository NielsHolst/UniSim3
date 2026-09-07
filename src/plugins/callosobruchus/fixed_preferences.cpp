#include <base/exception.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "fixed_preferences.h"

using namespace base;
using TestNum::gt;
using TestNum::snapToZero;

namespace callosobruchus {

PUBLISH(FixedPreferences)

FixedPreferences::FixedPreferences(QString name, Box *parent)
    : Box(name, parent)
{
    Doc("keeps fixed female oviposition preferences");
    Input(p0).unit("[0;1]").help("Preference for undamaged seeds");
    Input(p1).unit("[0;1]").help("Preference for seeds parasitised once");
    Output(p2).unit("[0;1]").help("Preference for seeds parasitised more than once");
}

void FixedPreferences::reset() {
    if (gt(p1, p0))
        ThrowException("p0 must be <= p1").value1(p0).value2(p1);
    if (gt(p0 + p1, 1.))
        ThrowException("p0 + p1 must be <= 1").value1(p0).value2(p1);
    p2 = snapToZero(1. - p0 - p1);
}


} //namespace
