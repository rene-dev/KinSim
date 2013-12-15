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
    
    path* currentPath;
    outpath p;
    
    // camera handling
	GLfloat worldRotation [4];
	GLfloat objectRotation [4];

    // spin
	GLfloat rRot [3];
	GLfloat rVel [3];
	GLfloat rAccel [3];
    
    //frame timer
    NSTimer* timer;
    NSTimer* postimer;
    
    NSPoint drag;
    NSPoint startcam;
    
    easyobj* obj;
    
    GLfloat j1,j2,j3;
    
    recCamera camera;
    recVec cam;
    int curr_pos;
    int frame;
    int display;
    GLfloat shapeSize;
}

-(IBAction)stop:(id)sender;
-(IBAction)send:(id)sender;
-(void)newPath:(struct path*)newpath;

@end
