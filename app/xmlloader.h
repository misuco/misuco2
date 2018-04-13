#ifndef XMLLOADER_H
#define XMLLOADER_H

#include <QObject>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "misuco2.h"

class Misuco2;

class XmlLoader : public QObject
{
    Q_OBJECT

public:
    explicit XmlLoader(Misuco2 * misuco2, QObject *parent = nullptr);

    void readXml(QString filetype);
    void writeXml(QString filetype);

signals:
    void sendCC1(bool);

public slots:

private:
    Misuco2* _app;
    QString _configPath;
    QXmlStreamWriter _xmlWriter;
    QXmlStreamReader _xmlReader;

    void decodeConfigRecord();
    void decodeScaleRecord();
    void decodeSynthRecord();
    void decodeTuneRecord();

};

#endif // XMLLOADER_H
