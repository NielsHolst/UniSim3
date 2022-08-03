/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include "crop.h"
#include "rotation.h"

using namespace base;

namespace awe {

PUBLISH(Rotation)

Rotation::Rotation(QString name, QObject *parent)
    : Box(name,parent)
{
    Input(crops).help("Vector specifying sequence of crops");
    Output(currentCrop);
    Output(nextCrop);
    Output(currentCropIndex);
    Output(nextCropIndex);
    Output(lai).help("Leaf area index of the current crop");
}

void Rotation::reset() {
    _length = crops.size();
    _isGrowing.fill(false, _length);
    currentCropIndex = _length - 1;
    nextCropIndex = (currentCropIndex + 1)%_length;
    updateCrop();
}

void Rotation::update() {
    updateCropIndex();
    updateCrop();
}

void Rotation::updateCropIndex() {
    bool currentIsGrowing = _isGrowing.at(currentCropIndex),
         sowToday = findCropModel(nextCropIndex)->port("sowToday")->value<bool>(),
         harvestToday = findCropModel(currentCropIndex)->port("harvestToday")->value<bool>();
    if (!currentIsGrowing && sowToday) {
        _isGrowing[currentCropIndex] = true;
        currentCropIndex = nextCropIndex;
        nextCropIndex = (currentCropIndex + 1)%_length;
    }
    else if (currentIsGrowing && harvestToday) {
        _isGrowing[currentCropIndex] = false;
    }
}

Box* Rotation::findCropModel(int cropIndex) {
    QString path = "./" + crops.at(cropIndex) + "<Crop>";
    Box *model = resolveMaybeOne<Box>(path);
    if (!model)
        ThrowException("Crop does not exist in rotation").value(crops.at(cropIndex));
    return model;
}

void Rotation::updateCrop() {
    currentCrop = crops.at(currentCropIndex);
    nextCrop = crops.at(nextCropIndex);
    lai = findCropModel(currentCropIndex)->port("lai")->value<double>();
}

}
