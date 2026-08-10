#include <base/exception.h>
#include <base/publish.h>
#include "random_preferences.h"

using namespace base;

namespace callosobruchus {

PUBLISH(RandomPreferences)

RandomPreferences::RandomPreferences(QString name, Box *parent)
    : Box(name, parent)
{
    Help("models random female oviposition preferences");
    Output(p0).unit("[0;1]").help("Preference for undamaged seeds");
    Output(p1).unit("[0;1]").help("Preference for seeds parasitised once");
    Output(p2).unit("[0;1]").help("Preference for seeds parasitised more than once");
}

void RandomPreferences::reset() {
    // Draw and scale
    p0 = u();
    p1 = u();
    p2 = u();
    const double sum = p0 + p1 + p2;
    p0 /= sum;
    p1 /= sum;
    p2 /= sum;
    // Order descending
    if (p0 < p1) std::swap(p0, p1);
    if (p1 < p2) std::swap(p1, p2);
    if (p0 < p1) std::swap(p0, p1);
}


double RandomPreferences::u() {
    const double mn = 0.01, mx = 0.99;
    return mn + (mx - mn)*_distribution(_generator);
}

} //namespace
