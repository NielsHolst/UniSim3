#include <iostream>
#include <QTextStream>
#include <base/environment.h>
#include <base/gradient_ascent.h>
#include <base/test_num.h>
#include "test_gradient_ascent.h"

using namespace base;
using namespace TestNum;

// Function with optimum at (x,y) = (20,20)
double parabolic(double x, double y) {
    return   -(x-10)*(x-30) - (y-18)*(y-22);
}

void TestGradientAscent::testParabolic() {
    auto solver = std::unique_ptr<base::GradientAscent>(new base::GradientAscent(parabolic));
    solver->bracketX(5., 35.).
            bracketY(15., 25.).
            init(8., 24.).
            maxIterations(200).
            keepHistory(true);

    auto solution = solver->solve();
    QVERIFY2(solution.iterations<200, qPrintable(QString::number(solution.iterations)));
    QVERIFY2(eq(solution.x, 20., 1e-3), qPrintable(QString::number(solution.x)));
    QVERIFY2(eq(solution.y, 20., 1e-3), qPrintable(QString::number(solution.y)));

    std::ostringstream os;
    os <<  "x\ty\tgradientX\tgradientY\titerations" << std::endl;
    auto history = solver->history();
    for (auto it = history.begin(); it != history.end(); ++it) {
        os << *it << std::endl;
    }

    QFile file;
    QString fileName = environment().openOutputFile(file, "txt");
    std::cout << "Writing solution to: " << qPrintable(fileName) << std::endl;
    file.write(os.str().c_str());
    file.close();
}


