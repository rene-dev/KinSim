//
//  KinOpenGLView.m
//  kinsim
//
//  Created by Rene on 11/11/13.
//  Copyright (c) 2013 Rene. All rights reserved.
//

#import "KinOpenGLView.h"

@implementation KinOpenGLView

- (void)mouseDown:(NSEvent *)theEvent
{
    NSPoint location = [self convertPoint:[theEvent locationInWindow] fromView:self];
    drag.x = location.x;
    drag.y = location.y;
    startcam.x = cam.x;
    startcam.y = cam.y;
}

- (void)mouseDragged:(NSEvent *)theEvent
{
    NSPoint location = [self convertPoint:[theEvent locationInWindow] fromView:self];
    cam.x = location.x-drag.x+startcam.x;
    cam.y = drag.y-location.y+startcam.y;
}

- (void)scrollWheel:(NSEvent *)theEvent
{
	float wheelDelta = [theEvent deltaX] +[theEvent deltaY] + [theEvent deltaZ];
	if (wheelDelta)
	{
		GLfloat deltaAperture = wheelDelta * -camera.aperture / 200.0f;
		camera.aperture += deltaAperture;
		if (camera.aperture < 0.1) // do not let aperture <= 0.1
			camera.aperture = 0.1;
		if (camera.aperture > 179.9) // do not let aperture >= 180
			camera.aperture = 179.9;
		updateProjection(&camera,&shapeSize); // update projection matrix
		//[self setNeedsDisplay: YES];
	}
}

- (void)animationTimer:(NSTimer *)timer
{
    [debugtext setStringValue:[NSString stringWithFormat:@"%f\n%f\n%f",j1,j2,j3]];
    curr_pos += (int)[speed floatValue];
    
    if(curr_pos < 0){
        curr_pos = p.length - 1;
    }
    if(curr_pos >= p.length){
        curr_pos = 0;
    }
    if([speed floatValue] != 0){
        [pos setFloatValue:curr_pos * 100 / p.length];
    }else{
        curr_pos = [pos floatValue]/100*(p.length-1);
    }
    
    easydraw(&cam,&display,&curr_pos,&j1,&j2,&j3,&p,currentPath);
}

-(IBAction)stop:(id)sender{
    [speed  setFloatValue:0];
    [joint1 setFloatValue:0];
    [joint2 setFloatValue:0];
    [joint3 setFloatValue:0];
    [joint4 setFloatValue:0];
    [joint5 setFloatValue:0];
    [joint6 setFloatValue:0];
}

-(IBAction)send:(id)sender{
    int i1 = -j1 / 0.055;
    int i2 =  j2 / 0.038;
    int i3 =  j3 / 0.038;
    
    char buffer[1024];
    snprintf(buffer, sizeof(buffer), "D %i %i %i %i %i %i\n", i1, i2, i3, 0, 0, 0);
    NSLog(@"%@",[NSString stringWithCString:buffer encoding:NSASCIIStringEncoding]);
    swrite(buffer);
}


- (void) prepareOpenGL
{
    NSRect rectView = [self bounds];
	camera.viewHeight = rectView.size.height;
	camera.viewWidth = rectView.size.width;
    easyinit(&camera,&cam,&shapeSize,&frame,&curr_pos,&display);
    timer = [NSTimer
             timerWithTimeInterval:(1.0f/60.0f)
             target:self
             selector:@selector(animationTimer:)
             userInfo:nil
             repeats:YES];
    
    [[NSRunLoop currentRunLoop] addTimer:timer forMode:NSDefaultRunLoopMode];
    // ensure timer fires during resize
	[[NSRunLoop currentRunLoop] addTimer:timer forMode:NSEventTrackingRunLoopMode];
}

- (void) newPath:(struct path*)newpath
{
    display = NO;
    free(p.jointpos1);
    free(p.jointpos2);
    free(p.jointpos3);
    freepath(currentPath);
    
    currentPath = newpath;
    p = interpol(currentPath);
    display = YES;
}

- (void)windowDidResize:(NSNotification *)notification{
    NSRect rectView = [self bounds];
	camera.viewHeight = rectView.size.height;
	camera.viewWidth = rectView.size.width;
    updateCamera(&camera,&shapeSize);
}

@end
