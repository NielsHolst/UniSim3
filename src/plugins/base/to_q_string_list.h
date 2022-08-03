#ifndef BASE_TO_Q_STRING_LIST_H
#define BASE_TO_Q_STRING_LIST_H

#if QT_VERSION >= 0x050E00
    inline QStringList toQStringList(QVector<QString> v) {return QStringList( QList<QString>(v.begin(), v.end()) ); }
#else
    inline QStringList toQStringList(QVector<QString> v) {return  QStringList( QList<QString>(v.toList()) ); }
#endif

#endif