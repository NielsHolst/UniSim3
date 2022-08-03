/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BUFFER_H
#define BUFFER_H
#include <boost/circular_buffer.hpp>
#include <QVector>
#include <base/box.h>

namespace boxes {

class Buffer : public base::Box
{
public:
    Buffer(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double initial, input, increment;
    int size;
    // Outputs
    double sum, average;
    QVector<double> buffer;
    // Data
    bool _firstUpdate;
    boost::circular_buffer<double> _buffer;
    // Methods
    void updateOutputBuffer();
};

}

#endif
