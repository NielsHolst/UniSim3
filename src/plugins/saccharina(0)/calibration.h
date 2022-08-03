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
    Calibration(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    QString obsFileName;
    QDate date;
    double area, yield, pctN, pctC;
    // Data
    bool noObservations;
    QDate nextDate;
    QVector<QDate> dates;
    struct Output {
        // Buffers
        QVector<double> obs, pred;
        // Outputs
        double sumSq;
        // Methods
        void clear();
        void update(double predicted);
    };
    struct {
        Output area, yield, pctN, pctC;
    } output;
    int row;
};

}

#endif
