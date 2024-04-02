/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_GRADIENT_ASCENT_H
#define BASE_GRADIENT_ASCENT_H
#include <iostream>
#include <QVector>

namespace base {

class GradientAscent {
public:
    typedef double (*Function)(double, double);

    struct Solution {
        double x, y, gradientX, gradientY;
        int iterations;
    };

    GradientAscent(Function f);

    GradientAscent& bracketX(double minX, double maxX, bool fixed = false) {
        i.minX = minX; i.maxX = maxX; i.limitX = fixed;
        return *this;
    }

    GradientAscent& bracketY(double minY, double maxY, bool fixed = false) {
        i.minY = minY; i.maxY = maxY; i.limitY = fixed;
        return *this;
    }

    GradientAscent& init(double x, double y) {
        i.initX = x; i.initY = y;
        isUserInitialised = true;
        return *this;
    }

    GradientAscent& delta(double value) { i.delta = value; return *this; }
    GradientAscent& learningRate(double value) { i.learningRate = value; return *this; }
    GradientAscent& momentum(double value) { i.momentum = value; return *this; }
    GradientAscent& precision(double x, double y) { i.precisionX = x; i.precisionY = y; return *this; }
    QVector<Solution> history() const { return s.history; }

    GradientAscent& maxIterations(int value) {
            i.maxIterations = value;
            if (i.keepHistory)
                s.history.reserve(value);
            return *this;
    }

    GradientAscent& keepHistory(bool value) {
        i.keepHistory = value;
        if (value)
            s.history.reserve(i.maxIterations);
        return *this;
    }

    Solution solve();

    friend std::ostream& operator<<(std::ostream &os, Solution solution);

private:
    Function function;
    bool isUserInitialised;
    double iceptX, slopeX, iceptY, slopeY;
    // Inputs
    struct {
        double
            minX = 0, maxX = 1, limitX = false,
            minY = 0, maxY = 1, limitY = false,
            initX = 0.5, initY = 0.5,
            delta = 1e-6, learningRate = 0.01, momentum = 0.2,
            precisionX = 1e-5, precisionY = 1e-5;
        int maxIterations = 30;
        bool keepHistory = false;
     } i;

    // State
    struct {
        double gradientX, gradientY;
        QVector<Solution> history;
    } s;

    // Compute the gradients of the function at a given point
    void updateGradients(double x, double y);

    double transformX(double x) {
        return (x - i.minX)/(i.maxX - i.minX);
    }

    double backTransformX(double transformedX) {
        return i.minX + transformedX*(i.maxX - i.minX);
    }

    double transformY(double y) {
        return (y - i.minY)/(i.maxY - i.minY);
    }

    double backTransformY(double transformedY) {
        return i.minY + transformedY*(i.maxY - i.minY);
    }

};

}

#endif

