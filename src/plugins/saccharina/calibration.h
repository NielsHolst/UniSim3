/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef CALIBRATION_H
#define CALIBRATION_H
#include <base/box.h>

namespace saccharina {

class Calibration : public base::Box
{
public:
    Calibration(QString name, Box *parent);
    void initialize();
    void reset();
    void update();
private:
    // Input
    QString obsFileName;
    QDate date;
    double biomass, concC, concN, concP;
    // Output
    double value;

    // Data
    QVector<QDate> obsDates;
    struct Data {
        QVector<double> obs, pred;
        double obsAvg, obsSd;
        void standardizeObs();
        void enterPrediction(int row, double value);
        double sqDev(int row) const;
    };
    struct {
        Data biomass, concC, concN, concP;
    } variable;
    int currentRow;
};

}

#endif
