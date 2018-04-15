#include "openarchive.h"
#include "lib/misulib/models/types.h"
#include <QDesktopServices>
#include <QUrl>

OpenArchive::OpenArchive(QString text, int state, QObject * parent) : ButtonBase(text,state,parent)
{}

void OpenArchive::onSetRootNote(int p)
{
    _rootNote = p;
}

void OpenArchive::onSetScale(int n, bool v)
{
    _scale[n] = v;
}

void OpenArchive::releaseAction()
{
    QString link ="http://scales.misuco.org/";
    link.append(midi2TextUrl(_rootNote));
    link.append("-");
    for(int i=0;i<SCALE_SIZE;i++) {
        if(_scale[i]) {
            int currnote=_rootNote+i+1;
            link.append(midi2TextUrl(currnote%12));
            link.append("-");
        }
    }
    link.chop(1);
    QDesktopServices::openUrl(QUrl(link));
}

QString OpenArchive::midi2TextUrl(int midinote)
{
    QString t;
    switch(midinote) {
    case 0:
        t="c";
        break;
    case 1:
        t="cs";
        break;
    case 2:
        t="d";
        break;
    case 3:
        t="ds";
        break;
    case 4:
        t="e";
        break;
    case 5:
        t="f";
        break;
    case 6:
        t="fs";
        break;
    case 7:
        t="g";
        break;
    case 8:
        t="gs";
        break;
    case 9:
        t="a";
        break;
    case 10:
        t="as";
        break;
    case 11:
        t="b";
        break;
    default:
        t="";
        break;
    }
    return t;
}
