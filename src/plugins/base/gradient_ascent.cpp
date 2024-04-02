/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <math.h>
#include "phys_math.h"
#include "gradient_ascent.h"

#define SOL Solution{x, y, s.gradientX, s.gradientY, iter}

using std::fabs;
using namespace phys_math;

namespace base {

GradientAscent::GradientAscent(Function f)
    : function(f), isUserInitialised(false),
      iceptX(0), slopeX(1), iceptY(0), slopeY(1) // default to identity transformation
{
}

GradientAscent::Solution GradientAscent::solve() {
    double
        x = isUserInitialised ? i.initX : (i.maxX + i.minX)/2.,
        y = isUserInitialised ? i.initY : (i.maxY + i.minY)/2.,
        transformedX = transformX(x),
        transformedY = transformY(y),
        velocityX = 0.,
        velocityY = 0.;

    s.history.clear();
    int iter;
    for (iter = 0; iter < i.maxIterations; ++iter) {
        updateGradients(x, y);
        velocityX = i.momentum*velocityX + i.learningRate*s.gradientX;
        velocityY = i.momentum*velocityY + i.learningRate*s.gradientY;
        transformedX += velocityX;
        transformedY += velocityY;
        x = backTransformX(transformedX);
        y = backTransformY(transformedY);
        if (i.limitX) {
            x = minmax(i.minX, x, i.maxX);
            transformedX = transformX(x);
        }
        if (i.limitY) {
            y = minmax(i.minY, y, i.maxY);
            transformedY = transformY(y);
        }
        if (i.keepHistory)
            s.history << SOL;
        bool zeroGradient = fabs(s.gradientX) < i.precisionX && fabs(s.gradientY) < i.precisionY,
             limitsReached = !zeroGradient && (x == i.minX || x == i.maxX) && (y == i.minY || y == i.maxY);
        if (zeroGradient || limitsReached)
            break;
    }
    return SOL;
}

void GradientAscent::updateGradients(double x, double y) {
    const double
        xDx = backTransformX(transformX(x) + i.delta),
        yDy = backTransformY(transformY(y) + i.delta),
        dx = xDx - x,
        dy = yDy - y,
        z = function(x,y),
        zDx = function(xDx, y) - z,
        zDy = function(x, yDy) - z;
    s.gradientX = zDx / dx;
    s.gradientY = zDy / dy;
}

std::ostream& operator<<(std::ostream &os, GradientAscent::Solution solution) {
    os << solution.x << "\t"
      << solution.y << "\t"
      << solution.gradientX << "\t"
      << solution.gradientY << "\t"
      << solution.iterations;
    return os;
}

}
