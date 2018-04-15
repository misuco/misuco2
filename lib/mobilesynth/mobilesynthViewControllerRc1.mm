//
//  mobilesynthViewController.m
//  mobilesynth
//
//  Created by Allen Porter on 12/7/08.
//  Copyright thebends 2008. All rights reserved.
//

#include <QtGlobal>
#ifdef Q_OS_IOS

#include "mobilesynthviewcontrollerrc1.hpp"
#import "mobilesynthViewControllerRc1.h"
#import "AudioOutput.h"
#include "synth/controller.h"
#include "synth/envelope.h"
#include "synth/modulation.h"
#include "synth/oscillator.h"
#include "synth/filter.h"
#include "synth/parameter.h"

namespace mobilesynthview
{
    struct WidgetImpl
    {
        mobilesynthViewControllerRc1* wrapped;
    };
    Widget::Widget() :
    impl(new WidgetImpl)
    {
        impl->wrapped = [[mobilesynthViewControllerRc1 alloc] init];
    }
    Widget::~Widget()
    {
        if (impl) {
            [impl->wrapped dealloc];
//            [impl->wrapped release];
        }
        delete impl;
    }
    void Widget::noteOn(int n, float f)
    {
        [impl->wrapped noteOn:n :f];
    }
    void Widget::noteOff(int n)
    {
        [impl->wrapped noteOff:n];
    }
    void Widget::pc(int n)
    {
        [impl->wrapped pc:n];
    }
    void Widget::setController(synth::Controller * ctl)
    {
        [impl->wrapped setController:ctl];
    }
}

@implementation mobilesynthViewControllerRc1

#define degreesToRadians(x) (M_PI * x / 180.0)

// Use A above Middle C as the reference frequency
static const float kNotesPerOctave = 12.0;
static const float kMiddleAFrequency = 440.0;
static const int kMiddleANote = 49;

/*
static float GetFrequencyForNote(int note) {
    return kMiddleAFrequency * powf(2, (note - kMiddleANote) / kNotesPerOctave);
}
*/

- (id)init {
    self = [super init];
    return self;
}

- (void)noteOn:(int)note :(float)freq {
    controller_->NoteOn(note, freq);
}

- (void)noteOff:(int)note {
    controller_->NoteOff(note);
}

- (void)pc:(int)prog {
    controller_->set_osc1_wave_type_int(prog%5);
}

- (void)setController:(synth::Controller*)ctl {
    controller_=ctl;
    ctl->set_sample_rate(44100.0);
    // Format preferred by the iphone (Fixed 8.24)
    outputFormat.mSampleRate = 44100.0;
    outputFormat.mFormatID = kAudioFormatLinearPCM;
    //outputFormat.mFormatFlags  = kAudioFormatFlagsAudioUnitCanonical;
    //outputFormat.mFormatFlags  = kAudioFormatFlagIsFloat | kAudioFormatFlagsNativeEndian | kAudioFormatFlagIsPacked | kAudioFormatFlagIsNonInterleaved;
    outputFormat.mFormatFlags  = kAudioFormatFlagIsSignedInteger;
#if  __IPHONE_OS_VERSION_MAX_ALLOWED >= 8000
    outputFormat.mBytesPerPacket = sizeof(SInt32);
    outputFormat.mBytesPerFrame = sizeof(SInt32);
    outputFormat.mBitsPerChannel = 8 * sizeof(SInt32);
#else
    outputFormat.mBytesPerPacket = sizeof(AudioUnitSampleType);
    outputFormat.mBytesPerFrame = sizeof(AudioUnitSampleType);
    outputFormat.mBitsPerChannel = 8 * sizeof(AudioUnitSampleType);
#endif
    outputFormat.mFramesPerPacket = 1;
    outputFormat.mChannelsPerFrame = 1;
    outputFormat.mReserved = 0;
    
    output = [[AudioOutput alloc] initWithAudioFormat:&outputFormat];
    [output setSampleDelegate:self];
    [output start];  // immediately invokes our callback to generate samples
}

- (void)syncControls {
    @synchronized(self) {
    }
}

- (OSStatus)generateSamples:(AudioBufferList*)buffers {
    assert(controller_);
    assert(buffers->mNumberBuffers == 1);  // mono output
    AudioBuffer* outputBuffer = &buffers->mBuffers[0];
    int* data = (int*)outputBuffer->mData;
    if (controller_->released()) {
        // Silence
        memset(data, 0, outputBuffer->mDataByteSize);
        return noErr;
    }
    int samples = outputBuffer->mDataByteSize / sizeof(SInt32);
    controller_->GetInt32Sapmles(data, samples);
    return noErr;
}

// Setup the scrolable control panel
- (void)loadControlViews {
    
}

- (void)startLoadAnimations {
    //
    // Attempt some visual cues that will hopefully let the user notice that they
    // can scroll the control view and keyboard view.
    //
    
    // Start at the bottm and scroll to the top
}

- (void)viewDidLoad {
    [super viewDidLoad];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return (interfaceOrientation == UIInterfaceOrientationLandscapeRight);
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
    // Release anything that's not essential, such as cached data
}

- (void)dealloc {
    [output dealloc];
    //delete controller_;
    [super dealloc];
}

- (void)syncPageControl {
    // Switch the indicator when more than 50% of the previous/next page is visible
}

// At the end of scroll animation, reset the boolean used when scrolls originate from the UIPageControl
- (void)scrollViewDidEndDecelerating:(UIScrollView *)scrollView {
    
}

- (IBAction)changePage:(id)sender {
}


@end

#endif
