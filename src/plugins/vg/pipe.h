/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef PIPE_H
#define PIPE_H

#include <QMap>
#include <base/box.h>

namespace vg {

class Pipe : public base::Box
{
public:
    Pipe(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    QString material;
    double density, diameter, flowRate, minTemperature, maxTemperature,
        b, k,
        inflowTemperature, indoorsTemperature, groundArea;
    int numSpans;
    // Outputs
    double emissivity,
        length, volume, lengthPerSpan, volumePerSpan, transitTime,
        outflowTemperature, temperatureDrop, energyFlux;
    // Methods
    void parseMaterial();
    // Data
    double _k;
    struct MaterialInputs {
        double emissivity;
    };
    QMap<QString, MaterialInputs> materialInputs;
};

} //namespace


#endif
