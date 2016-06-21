#ifndef NOTEPROC_H
#define NOTEPROC_H

#include <comm/isender.h>

class noteProc
{
public:
    noteProc();
    ~noteProc();

    void processNote(int eventId, int eventState, float freq, int midinote, int pitch, int velocity);

private:
    // - networking
    ISender * out;

    // - event stack/hashmap
    //eventStackElement eventStack[EVENT_STACK_SIZE];
    int nextVoiceId = 0;
    int chan = 0;

};

#endif // NOTEPROC_H
