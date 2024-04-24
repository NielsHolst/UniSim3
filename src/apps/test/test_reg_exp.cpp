#include <QRegularExpression>
#include "test_reg_exp.h"


void TestRegExp::testMatchOne() {
    static QRegularExpression re("abc");
    QRegularExpressionMatch match;
    match = re.match("xxabcyy");
    QVERIFY(match.hasMatch());
    QCOMPARE(match.capturedStart(), 2);
}

void TestRegExp::testMatchAll() {
    static QRegularExpression re("abc");
    auto matches = re.globalMatch("xxabcyyabczz");
    QVector<int> starts;
    while (matches.hasNext()) {
        QRegularExpressionMatch match = matches.next();
        starts << match.capturedStart();
    }
    QCOMPARE(starts.size(), 2);
    QCOMPARE(starts.at(0), 2);
    QCOMPARE(starts.at(1), 7);
}

void TestRegExp::testMatchPairedApostrophes() {
    static QRegularExpression re(R"("[^"]*")");
    QRegularExpressionMatch match;
    QString s = R"(load "new model.box")";

    match = re.match(s);
    QVERIFY(match.hasMatch());
    QStringList texts = match.capturedTexts();
    QCOMPARE(texts.size(), 1);
    QCOMPARE(texts.at(0), R"("new model.box")");
}

void TestRegExp::testNumberInParentheses() {
    static QRegularExpression re(R"([^(]*\([0-9]*\).*)");
    QRegularExpressionMatch match;
    QString s0 = "saccharina-growth-rate.box",
            s1 = "saccharina-growth-rate(0).box";
    match = re.match(s0);
    QVERIFY(!match.hasMatch());
    match = re.match(s1);
    QVERIFY(match.hasMatch());
}
