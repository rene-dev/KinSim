//
//  KinOpenGLView.h
//  kinsim
//
//  Created by Rene on 11/11/13.
//  Copyright (c) 2013 Rene. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <OpenGL/gl.h>
#import <OpenGL/glext.h>
#import <OpenGL/glu.h>
#import "easygl.h"
#import "socket.h"
#import "interpolator.h"
#import "gcode.h"

@interface KinOpenGLView : NSOpenGLView <NSWindowDelegate>

{
    IBOutlet NSSlider* joint1;
    IBOutlet NSSlider* joint2;
    IBOutlet NSSlider* joint3;
    IBOutlet NSSlider* joint4;
    IBOutlet NSSlider* joint5;
    IBOutlet NSSlider* joint6;
    IBOutlet NSSlider* pos;
    IBOutlet NSSlider* speed;
    IBOutlet NSTextField* debugtext;
    
    NSTimer* timer;
    NSTimer* postimer;
    
    NSPoint drag;
    NSPoint startcam;
    easygl renderer;
}

-(IBAction)stop:(id)sender;
-(IBAction)send:(id)sender;
//-(void)newPath:(path*)newpath;

@end
