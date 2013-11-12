//
//  KinOpenGLView.m
//  kinsim
//
//  Created by Rene on 11/11/13.
//  Copyright (c) 2013 Rene. All rights reserved.
//

#import "KinOpenGLView.h"

// single set of interaction flags and states
GLint gDollyPanStartPoint[2] = {0, 0};
GLfloat gTrackBallRotation [4] = {0.0f, 0.0f, 0.0f, 0.0f};
GLboolean gDolly = GL_FALSE;
GLboolean gPan = GL_FALSE;
GLboolean gTrackball = GL_FALSE;
KinOpenGLView * gTrackingViewInfo = NULL;

// simple cube data
GLint cube_num_vertices = 8;

GLfloat cube_vertices [8][3] = {
    {1.0, 1.0, 1.0}, {1.0, -1.0, 1.0}, {-1.0, -1.0, 1.0}, {-1.0, 1.0, 1.0},
    {1.0, 1.0, -1.0}, {1.0, -1.0, -1.0}, {-1.0, -1.0, -1.0}, {-1.0, 1.0, -1.0} };

short cube_faces [6][4] = {
    {3, 2, 1, 0}, {2, 3, 7, 6}, {0, 1, 5, 4}, {3, 0, 4, 7}, {1, 2, 6, 5}, {4, 5, 6, 7} };

//ugly hacked cube
void wireBox(GLdouble width, GLdouble height, GLdouble depth){
    glPushMatrix();
    glTranslatef(0, height/2, 0);
    glScalef(width/2, height/2, depth/2);
    glColor3f (0, 0, 4.0);
    long f,i,fSize=1;
    for (f = 0; f < 6; f++) {
        glBegin (GL_LINE_LOOP);
        for (i = 0; i < 4; i++)
            glVertex3f(cube_vertices[cube_faces[f][i]][0] * fSize, cube_vertices[cube_faces[f][i]][1] * fSize, cube_vertices[cube_faces[f][i]][2] * fSize);
        glEnd ();
    }
    glPopMatrix();
}

@implementation KinOpenGLView

// sets the camera data to initial conditions
- (void) resetCamera
{
    camera.aperture = 40;
    //camera.rotPoint = gOrigin;
    
    camera.viewPos.x = 0.0;
    camera.viewPos.y = 0.0;
    camera.viewPos.z = 0.0;
    camera.viewDir.x = -camera.viewPos.x;
    camera.viewDir.y = -camera.viewPos.y;
    camera.viewDir.z = -camera.viewPos.z;
    
    camera.viewUp.x = 0;
    camera.viewUp.y = 0;
    camera.viewUp.z = 1;
    
    shapeSize = 20.0f; // max radius of of objects
    
    NSRect rectView = [self bounds];
	
	// ensure camera knows size changed
	if ((camera.viewHeight != rectView.size.height) ||
	    (camera.viewWidth != rectView.size.width)) {
		camera.viewHeight = rectView.size.height;
		camera.viewWidth = rectView.size.width;
		
		glViewport (0, 0, camera.viewWidth, camera.viewHeight);
		[self updateProjection];  // update projection matrix
		//[self updateInfoString];
	}
}

- (void)mouseDown:(NSEvent *)theEvent
{
    drag = [self convertPoint:[theEvent locationInWindow] fromView:nil];
}

- (void)mouseDragged:(NSEvent *)theEvent
{
    NSPoint location = [self convertPoint:[theEvent locationInWindow] fromView:nil];
    cam.x = drag.x+location.x;
    cam.y = drag.y-location.y;
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
		[self updateProjection]; // update projection matrix
		//[self setNeedsDisplay: YES];
	}
}

// update the projection matrix based on camera and view info
- (void) updateProjection
{
	GLdouble ratio, radians, wd2;
	GLdouble left, right, top, bottom, near, far;
    
    [[self openGLContext] makeCurrentContext];
    
	// set projection
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	near = -camera.viewPos.z - shapeSize * 0.5;
	if (near < 0.00001)
		near = 0.00001;
	far = -camera.viewPos.z + shapeSize * 0.5;
	if (far < 1.0)
		far = 1.0;
	radians = 0.0174532925 * camera.aperture / 2; // half aperture degrees to radians
	wd2 = near * tan(radians);
	ratio = camera.viewWidth / (float) camera.viewHeight;
	if (ratio >= 1.0) {
		left  = -ratio * wd2;
		right = ratio * wd2;
		top = wd2;
		bottom = -wd2;
	} else {
		left  = -wd2;
		right = wd2;
		top = wd2 / ratio;
		bottom = -wd2 / ratio;
	}
	glFrustum (left, right, bottom, top, near, far);
	//[self updateCameraString];
}

- (void)animationTimer:(NSTimer *)timer
{
    GLfloat j1,j2,j3;
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, -1, -5);
    glRotatef(cam.y, 1, 0, 0);
    glRotatef(cam.x, 0, 1, 0);
    glRotatef(0, 0, 0, 1);
    
    // Set every pixel in the frame buffer to the current clear color.
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    for (GLfloat i = -2; i <= 2; i += 1) {
        glVertex3f(i, 0, 2.5); glVertex3f(i, 0, -2.5);
        glVertex3f(2.5, 0, i); glVertex3f(-2.5, 0, i);
    }
    glEnd();
    
    j1 = [joint1 floatValue]+p.jointpos1[(int)[pos floatValue]];
    j2 = [joint2 floatValue]+p.jointpos2[(int)[pos floatValue]];
    j3 = [joint3 floatValue]+p.jointpos3[(int)[pos floatValue]];
    
    j1 = - j1 - 90;
    j2 = j2 - 45;
    j3 = j3 - j2 - 45;
    
    glPushMatrix();
    glRotatef(j1, 0, 1, 0);
    wireBox(0.5, 2.45, 0.5);
    
    glTranslatef(0, 2.45, 0);
    glRotatef(j2, 1, 0, 0);
    wireBox(0.5, 1.9, 0.5);
    
    glTranslatef(0, 1.9, 0);
    glRotatef(j3, 1, 0, 0);
    wireBox(0.5, 1.9, 0.5);
    glPopMatrix();
    
    // Draw a red x-axis, a green y-axis, and a blue z-axis.
    glBegin(GL_LINES);
    glColor3f(1, 0, 0); glVertex3f(0, 0, 0); glVertex3f(1, 0, 0);
    glColor3f(0, 1, 0); glVertex3f(0, 0, 0); glVertex3f(0, 1, 0);
    glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glVertex3f(0, 0, 1);
    glEnd();
    
    //zu fahrender path
    glBegin(GL_LINES);
    glColor3f(1, 1, 0);
    struct path* tmp = AB;
    while(tmp){
        glVertex3f(tmp->pos.axis_pos[0] / 100, tmp->pos.axis_pos[2] / 100, tmp->pos.axis_pos[1] / 100);
        tmp = tmp->next;
    }
    glEnd();
    
    // Flush drawing command buffer to make drawing happen as soon as possible.
    glFlush();
}

- (void) initPath{
    AB = (struct path *)malloc(sizeof(struct path));

    
    struct vec A;
	A.axis_pos[0] = 100;
	A.axis_pos[1] = 0;
	A.axis_pos[2] = 0;
    
	struct vec B;
	B.axis_pos[0] = 200;
	B.axis_pos[1] = 0;
	B.axis_pos[2] = 0;
    
    struct vec C;
	C.axis_pos[0] = 200;
	C.axis_pos[1] = 100;
	C.axis_pos[2] = 0;
    
    struct vec D;
	D.axis_pos[0] = 100;
	D.axis_pos[1] = 100;
	D.axis_pos[2] = 0;
    
    struct vec E;
	E.axis_pos[0] = 100;
	E.axis_pos[1] = 0;
	E.axis_pos[2] = 100;
    
    struct vec F;
	F.axis_pos[0] = 200;
	F.axis_pos[1] = 0;
	F.axis_pos[2] = 100;
	
	AB->next = 0;
	AB->prev = 0;
	AB->pos = A;
    append(AB, B);
	append(AB, C);
	append(AB, D);
	append(AB, A);
    append(AB, E);
    append(AB, F);
}

- (void) prepareOpenGL
{
    [self resetCamera];
    cam.x = 0;
    cam.y = 0;
    [speed setContinuous:YES];

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    [self initPath];
    
    p = interpol(AB);
    
    [pos setMaxValue:p.length-1];
    [pos setMinValue:0];
    
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

-(IBAction)sliderValueChanged:(NSSlider *)sender
{

}

@end
