#ifndef MWPLAYFIELD_H
#define MWPLAYFIELD_H

#include <QObject>
#include <QColor>
#include "freqtriple.h"
#include "types.h"

class MWPlayfield : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int fieldType MEMBER type NOTIFY textChanged)
    Q_PROPERTY(QColor colorF1 MEMBER colorF1 NOTIFY colorChanged)
    Q_PROPERTY(QColor colorF1b MEMBER colorF1b NOTIFY colorChanged)
    Q_PROPERTY(QColor colorF2 MEMBER colorF2 NOTIFY colorChanged)
    Q_PROPERTY(QColor colorF2b MEMBER colorF2b NOTIFY colorChanged)
    Q_PROPERTY(QColor fontColor MEMBER fontColor NOTIFY colorChanged)
    Q_PROPERTY(QString text1 MEMBER text1 NOTIFY textChanged)
    Q_PROPERTY(QString text2 MEMBER text2 NOTIFY textChanged)
    Q_PROPERTY(QString text3 MEMBER text3 NOTIFY textChanged)

public:
    explicit MWPlayfield(QObject *parent = nullptr);

    int type = NORMAL;
    FreqTriple * f1;      // for type normal
    FreqTriple * f2;      // for type vert/hoiz
    float hue1bent;         // cache for vertical bent hue values
    float hue2bent;
    unsigned int pressed = 0;

    void calcColor();

signals:
    void colorChanged();
    void textChanged();

private:
    QColor colorF1;
    QColor colorF1b;
    QColor colorF2;
    QColor colorF2b;
    QColor fontColor;
    QString text1;
    QString text2;
    QString text3;
};

#endif // MWPLAYFIELD_H
