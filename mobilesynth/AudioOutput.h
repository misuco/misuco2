//
//  AudioOutput.h
//  mobilesynth
//
//  Created by Allen Porter on 12/20/08.
//  Copyright 2008 thebends. All rights reserved.
//
#include <QtGlobal>
#ifdef Q_OS_IOS

#import <Foundation/Foundation.h>
#import <AudioUnit/AUComponent.h>

@protocol SampleGenerator 
@required
- (OSStatus)generateSamples:(AudioBufferList*)buffers;
@end

@interface AudioOutput : NSObject {
 @private
  id <SampleGenerator> sampleDelegate;
  AudioComponentInstance audioUnit;
  AudioStreamBasicDescription audioFormat;
}

- (id)initWithAudioFormat:(const AudioStreamBasicDescription*)streamDescription;

@property (nonatomic, retain) IBOutlet id <SampleGenerator> sampleDelegate;

- (void) start;

+ (void)displayErrorAndExit:(NSString*)message
                  errorCode:(OSStatus)status;

@end

#endif
