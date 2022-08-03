#ifndef FOOD_WEB_POPULATION_H
#define FOOD_WEB_POPULATION_H
#include <base/food_web_box_base.h>

namespace cowpea {

class FoodWebPopulation : public base::FoodWebBoxBase
{
public:
    FoodWebPopulation(QString name, QObject *parent);
    void reset();
private:
    // Input
    double initial, demandPerCap;
};

}

#endif
