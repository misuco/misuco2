#include "noteproc.h"

noteProc::noteProc()
{    
}

noteProc::~noteProc()
{

}

void noteProc::processNote(int eventId,int eventState,float freq, int midinote, int pitch, int velocity)
{
    /*
    int eventHash=eventId%64;
    eventStackElement * es = &eventStack[eventHash];

    switch(eventState) {
    case Qt::TouchPointPressed:
        es->eventId=eventId;
        es->voiceId=nextVoiceId++;
        es->midinote=midinote;
        es->row=row;
        es->col=col;
        pf->pressed++;
        out->noteOn(chan,es->voiceId,freq,midinote,pitch,velocity);
        //paintField(row,col);
        update();
        break;
    case Qt::TouchPointMoved:
        if(row!=es->row || col!=es->col) {
            MWPlayfield * ppf = &fields[es->row][es->col];
            ppf->pressed--;
            out->noteOff(chan,es->voiceId,es->midinote);

            es->voiceId=nextVoiceId++;
            es->midinote=midinote;
            out->noteOn(chan,es->voiceId,freq,midinote,pitch,velocity);

            es->row=row;
            es->col=col;
            pf->pressed++;
            //paintField(row,col);
            update();
        }
        break;
    case Qt::TouchPointReleased:
        out->noteOff(chan,es->voiceId,es->midinote);
        es->eventId=-1;
        es->row=-1;
        es->col=-1;
        pf->pressed--;
        //paintField(row,col);
        update();
        break;
    }
    */
}

