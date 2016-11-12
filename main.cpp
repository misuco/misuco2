#include "mainwindow.h"
#include <QApplication>
#include <time.h>
#include <sys/time.h>

#include <QStringList>

#define HIST_RESO 1000

struct Spot {
    double lastTs;
    int count;
    int histogram[HIST_RESO];
};

QMap<QString,struct Spot *> tokenmap;
double lastStat =0;

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    //QByteArray localMsg = msg.toLocal8Bit();
    struct timeval  tv;
    gettimeofday(&tv, NULL);

    double ts = (tv.tv_sec) + (double)tv.tv_usec / 1000000.0 ;

    QStringList msgParts=msg.split(" ");
    QString token=msgParts.at(0);
    struct Spot * spot;
    if(tokenmap.contains(token)) {
        spot=tokenmap.value(token);
        spot->count++;
        double tdiff=ts-spot->lastTs;
        tdiff*=1000;
        if(tdiff<HIST_RESO) {
            spot->histogram[(int)tdiff]++;
        }
        spot->lastTs=ts;
    } else {
        spot= new Spot;
        spot->count=1;
        spot->lastTs=ts;
        for(int i=0;i<HIST_RESO;i++) {
            spot->histogram[i]=0;
        }
        tokenmap.insert(token,spot);
    }

    if(ts-lastStat>1) {
        fprintf(stderr, "\nstat %f \n", ts);
        QList<QString> spots = tokenmap.keys();
        QList<QString>::iterator i;
        for (i = spots.begin(); i != spots.end(); ++i) {
            spot=tokenmap.value(*i);
            if(spot->count>0) {
                fprintf(stderr, "token %s f: %d h:", i->toLocal8Bit().constData(),spot->count);
                for(int j=0;j<HIST_RESO;j++) {
                    if(spot->histogram[j]>0) {
                        fprintf(stderr, " %d ms %d val" , j, spot->histogram[j]);
                    }
                    spot->histogram[j]=0;
                }
                fprintf(stderr, "\n");
                spot->count=0;
            }
        }
        lastStat=ts;
    }
    //fprintf(stderr, "QT: %f %s (%s:%u, %s)\n", ts, localMsg.constData(), context.file, context.line, context.function);
/*
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtInfoMsg:
        fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        abort();
    }
    */
}

int main(int argc, char *argv[])
{

    qInstallMessageHandler(myMessageOutput);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
