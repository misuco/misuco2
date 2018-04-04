#ifndef PURCHASES_H
#define PURCHASES_H

#include <QObject>
#include <QInAppStore>
#include <QInAppTransaction>

class Purchases: public QObject
{
    Q_OBJECT

public:
    Purchases( QObject *parent = 0);

    static QInAppProduct * productPresetManager;

    void initInAppStore();

private slots:
    void onProductRegistered(QInAppProduct* product);
    void onProductUnknown(QInAppProduct::ProductType type, QString name);
    void onTransactionReady(QInAppTransaction* transaction);

private:
    QInAppStore *   _inAppStore;

};

#endif // PURCHASES_H
