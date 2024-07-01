/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <boost/range/algorithm/random_shuffle.hpp>
#include "exception.h"
#include "random_generator.h"
#include "random_order.h"

namespace base {

RandomOrder::RandomOrder(int n, Node *parent)
    : Node("randomOrder", parent), distribution(nullptr), variate(nullptr)
{
    resize((n<1) ? 1 : n);
}

RandomOrder::~RandomOrder() {
    delete distribution;
    delete variate;
}

void RandomOrder::resize(int n) {
    // Set up random generator
    delete distribution;
    delete variate;
    distribution = new Distribution(0, n-1);
    variate = new Variate(randomGenerator(), *distribution);

    // Fill vector
    numbers.resize(static_cast<unsigned>(n));
    for (int i=0; i<n; ++i)
        numbers[static_cast<unsigned>(i)] = i;

    // Shuffle vector
    shuffle();
}

void RandomOrder::shuffle() {
    boost::random_shuffle(numbers, *variate);
    _next = 0;
}

int RandomOrder::next() {
    if (size() == 1)
        return 0;
    if (_next >= size())
        ThrowException("Index out of range").value(_next).value2(size());
    return numbers.at(static_cast<unsigned>(_next++));
}

int RandomOrder::size() const {
    return static_cast<int>(numbers.size());
}

int RandomOrder::at(int i) const {
    return numbers.at(i);
}

} // namespace
