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
    //startcam.x = cam.x;
    //startcam.y = cam.y;
}

- (void)mouseDragged:(NSEvent *)theEvent
{
    NSPoint location = [self convertPoint:[theEvent locationInWindow] fromView:self];
    //cam.x = location.x-drag.x+startcam.x;
    //cam.y = drag.y-location.y+startcam.y;
}

- (void)scrollWheel:(NSEvent *)theEvent
{
	float wheelDelta = [theEvent deltaX] +[theEvent deltaY] + [theEvent deltaZ];
	if (wheelDelta)
	{
		GLfloat deltaAperture = wheelDelta * -renderer.fieldOfView / 200.0f;
		renderer.fieldOfView += deltaAperture;
		if (renderer.fieldOfView < 0.1) // do not let aperture <= 0.1
			renderer.fieldOfView = 0.1;
		if (renderer.fieldOfView > 179.9) // do not let aperture >= 180
			renderer.fieldOfView = 179.9;
	}
    
}

- (void)animationTimer:(NSTimer *)timer
{
    NSRect rectView = [self bounds];
    renderer.viewportSize.y = rectView.size.height;
	renderer.viewportSize.x = rectView.size.width;
    /*
    [debugtext setStringValue:[NSString stringWithFormat:@"%f\n%f\n%f",j1,j2,j3]];
    
    curr_pos += (int)[speed floatValue];
    
    if(curr_pos < 0){
        renderer.curr_pos = 100 - 1;//todo: laenge wieder einbauen
    }
    if(curr_pos >= 100){
        curr_pos = 0;
    }
    if([speed floatValue] != 0){
        [pos setFloatValue:curr_pos * 100 / 100];
    }else{
        curr_pos = [pos floatValue]/100*(p.length-1);
    }
    */
    renderer.draw();
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
    //int i1 = -j1 / 0.055;
    //int i2 =  j2 / 0.038;
    //int i3 =  j3 / 0.038;
    
    char buffer[1024];
    //snprintf(buffer, sizeof(buffer), "D %i %i %i %i %i %i\n", i1, i2, i3, 0, 0, 0);
    NSLog(@"%@",[NSString stringWithCString:buffer encoding:NSASCIIStringEncoding]);
    swrite(buffer);
}


- (void) prepareOpenGL
{
    NSString *defaultgcode = [[NSBundle mainBundle] pathForResource:@"gcode" ofType:@"ngc"];
    renderer.currentPath = gcode([defaultgcode cStringUsingEncoding:NSASCIIStringEncoding]);
    interpol(renderer.currentPath);
    renderer.robotState = &renderer.currentPath->pos;
    renderer.init();
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

- (void) newPath:(path*)newpath
{
    /*
    display = NO;
    free(p.jointpos1);
    free(p.jointpos2);
    free(p.jointpos3);
    freepath(currentPath);
    
    currentPath = newpath;
    //p = interpol(currentPath);
    display = YES;
    */
}

@end
