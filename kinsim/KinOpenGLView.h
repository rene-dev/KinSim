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

extern "C" {
#import "socket.h"
#import "interpolator.h"
}

typedef struct {
    GLdouble x,y,z;
} recVec;

typedef struct {
	recVec viewPos; // View position
	recVec viewDir; // View direction vector
	recVec viewUp; // View up direction
	recVec rotPoint; // Point to rotate about
	GLdouble aperture; // pContextInfo->camera aperture
	GLint viewWidth, viewHeight; // current window/screen height and width
} recCamera;

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
    
    struct path* currentPath;
    struct outpath p;
    int curr_pos;
    
    // camera handling
	recCamera camera;
	GLfloat worldRotation [4];
	GLfloat objectRotation [4];
	GLfloat shapeSize;

    // spin
	GLfloat rRot [3];
	GLfloat rVel [3];
	GLfloat rAccel [3];
    
    //frame timer
    NSTimer* timer;
    NSTimer* postimer;
    
    NSPoint drag;
    NSPoint cam;
    NSPoint startcam;
    
    easyobj* obj;
    
    GLfloat j1,j2,j3;
    
    int frame;
    Boolean display;
}

-(IBAction)stop:(id)sender;
-(IBAction)send:(id)sender;
-(void)newPath:(struct path*)newpath;
-(void)updateCamera;
-(void)resetCamera;

@end
