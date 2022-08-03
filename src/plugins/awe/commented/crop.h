/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef CROP_H
#define CROP_H

#include <QMap>
#include <base/box.h>

namespace awe {

class Crop : public base::Box
{
    Q_OBJECT
public:
    Crop(QString name, QObject *parent=0);

    void initialize();
    void reset();
    void update();

private:
    // inputs
    QString currentCrop, nextCrop;
    QDate sowingDate, harvestDate, date;
    double weedExchangeRate, maxYield,
           maxYieldLossPct, slopeYieldLossPct, ddSum;
    QVector<double> laiX, laiY;

    // outputs
    double lai;
    bool sowToday, harvestToday, isSown, isHarvested;

    // data
    QMap<double, double> laiXY;

    // methods
    double interpolateLai() const;
};

}
#endif
