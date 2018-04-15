#include <QDebug>
#include "purchases.h"

QInAppProduct * Purchases::productPresetManager = nullptr;

Purchases::Purchases(QObject *parent): QObject(parent)
{

}

void Purchases::initInAppStore()
{
    _inAppStore = new QInAppStore(this);

    connect(_inAppStore, SIGNAL(productRegistered(QInAppProduct*)),
            this, SLOT(onProductRegistered(QInAppProduct*)));

    connect(_inAppStore, SIGNAL(productUnknown(QInAppProduct::ProductType,QString)),
            this, SLOT(onProductUnknown(QInAppProduct::ProductType,QString)));

    connect(_inAppStore, SIGNAL(transactionReady(QInAppTransaction*)),
            this, SLOT(onTransactionReady(QInAppTransaction*)));

    _inAppStore->registerProduct(QInAppProduct::Unlockable,
                               QStringLiteral("PresetManager"));

    productPresetManager = _inAppStore->registeredProduct(QStringLiteral("PresetManager"));
}

void Purchases::onProductRegistered(QInAppProduct *product)
{
    qDebug() << "wlayout::onProductRegistered " << product;
}

void Purchases::onProductUnknown(QInAppProduct::ProductType type, QString name)
{
    qDebug() << "wlayout::onProductUnknown " << type << " id " << name;
}

void Purchases::onTransactionReady(QInAppTransaction *transaction)
{
    if (transaction->status() == QInAppTransaction::PurchaseApproved
        && transaction->product()->identifier() == QStringLiteral("PresetManager")) {
        /*
        if (!hasAlreadyStoredTransaction(transaction->orderId()) {
            ++m_healthPotions;
            if (!addHealthPotionToPersistentStorage(transaction->orderId())
                popupErrorDialog(tr("Unable to write to persistent storage. Please make sure there is sufficient space and restart."))
            else
                transaction->finalize();
        }
        */
    } else if (transaction->status() == QInAppTransaction::PurchaseFailed) {
        qDebug() << "Purchase not completed.";
        //transaction->finalize();
    }
    transaction->finalize();
}

