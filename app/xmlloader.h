#ifndef XMLLOADER_H
#define XMLLOADER_H

#include <QObject>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

class Misuco2;

class XmlLoader : public QObject
{
    Q_OBJECT

public:
    explicit XmlLoader(Misuco2 * misuco2, QObject *parent = nullptr);

signals:

public slots:

private:
    Misuco2* _app;
    QString _configPath;
    QXmlStreamWriter xml;
    QXmlStreamReader xmlr;

    void readXml(QString filetype);
    void writeXml(QString filetype);

    void decodeConfigRecord();
    void decodeScaleRecord();
    void decodeSynthRecord();
    void decodeTuneRecord();

};

#endif // XMLLOADER_H
