/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef ROTATION_H
#define ROTATION_H

#include <QVector>
#include <base/box.h>

namespace awe {

class Crop;

class Rotation : public base::Box
{
public:
    Rotation(QString name, QObject *parent=0);
    void reset();
    void update();

private:
    // inputs
    QVector<QString> crops;
    // output
    QString currentCrop, nextCrop;
    int currentCropIndex, nextCropIndex;
    double lai;
    // state
    int _length;
    Box *_currentCropModel;
    QVector<bool> _isGrowing;

    // methods
    Box* findCropModel(int cropIndex);
    void updateCropIndex();
    void updateCrop();
};

}
#endif
