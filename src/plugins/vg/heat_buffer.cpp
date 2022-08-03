/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include <base/test_num.h>
#include "heat_buffer.h"

using namespace base;

namespace vg {

PUBLISH(HeatBuffer)

HeatBuffer::HeatBuffer(QString name, Box *parent)
    : Box(name, parent)
{
    help("models a heat buffer");
    Input(input).help("Heat input").unit("kW");
    Input(size).help("Max energy content").unit("kWh");
    Input(demand).help("Demanded heat output").unit("kW");
    Input(capacity).help("Maximum heat supply").unit("kW");
    Output(content).help("Energy content").unit("kWh");
    Output(supply).help("Energy supplied in response to demand").unit("kW");
}

void HeatBuffer::reset() {
    update();
}

void HeatBuffer::update() {
    supply = std::min(content+input, demand);
    content += input - supply;
    if (content>size)
        content = size;
    TestNum::snapToZero(content, 1e-3);
}

} //namespace

