//
//  AppDelegate.h
//  kinsim
//
//  Created by Rene on 11/11/13.
//  Copyright (c) 2013 Rene. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "gcode.h"
#import "KinOpenGLView.h"

@interface AppDelegate : NSObject <NSApplicationDelegate>

@property (assign) IBOutlet NSWindow *window;
@property (weak) IBOutlet KinOpenGLView *kinView;

@end
