#ifndef PRESETCOLLECTION_H
#define PRESETCOLLECTION_H

#include <QObject>

class PresetCollection : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool dialogVisible MEMBER dialogVisible NOTIFY layoutChange)
    Q_PROPERTY(QList<QObject *> items MEMBER _items NOTIFY itemsChange)

public:
    explicit PresetCollection(QObject *parent = 0);

    static QObject * dialogContext;
    static bool dialogVisible;

    Q_INVOKABLE void closeDialogPreset();
    Q_INVOKABLE void overwritePreset();
    Q_INVOKABLE void buyPresetManager();

    void append(QObject * item);

    QList<QObject *> getItems();
    QObject * getItem(int i);
    int getItemCount();

signals:
    void layoutChange();
    void itemsChange();

private slots:
    void onEditPreset();

private:
    QList<QObject *> _items;


};

#endif // PRESETCOLLECTION_H
