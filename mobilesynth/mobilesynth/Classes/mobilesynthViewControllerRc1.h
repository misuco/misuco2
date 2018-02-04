//
//  mobilesynthViewController.h
//  mobilesynth
//
//  Created by Allen Porter on 12/7/08.
//  Copyright thebends 2008. All rights reserved.
//
#include <QtGlobal>
#ifdef Q_OS_IOS

#import <UIKit/UIKit.h>
#import "AudioOutput.h"
#import <AudioToolbox/AudioConverter.h>

namespace synth { class Controller; }
namespace synth { class Envelope; }
namespace synth { class LFO; }
namespace synth { class Oscillator; }
namespace synth { class Note; }
namespace synth { class LowPass; }

@interface mobilesynthViewControllerRc1 : UIViewController <SampleGenerator> {
 @private
  // Synthesizer components
  AudioOutput* output;
  synth::Controller* controller_;
  
  AudioStreamBasicDescription outputFormat;
}

- (void)noteOn:(int)note :(float)freq;
- (void)noteOff:(int)note;
- (void)pc:(int)prog;
- (void)setController:(synth::Controller*)ctl;
- (OSStatus)generateSamples:(AudioBufferList*)buffers;

// For control panel
- (IBAction)changePage:(id)sender;

@end

#endif
