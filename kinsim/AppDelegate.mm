//
//  AppDelegate.m
//  kinsim
//
//  Created by Rene on 11/11/13.
//  Copyright (c) 2013 Rene. All rights reserved.
//

#import "AppDelegate.h"

@implementation AppDelegate
@synthesize kinView,window;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    NSString *defaultgcode = [[NSBundle mainBundle] pathForResource:@"gcode" ofType:@"ngc"];
    //[kinView newPath:gcode([defaultgcode cStringUsingEncoding:NSASCIIStringEncoding])];
    [window setTitle:[defaultgcode lastPathComponent]];
    
    connected = sopen("rasp");
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication{
    return YES;
}

-(BOOL)application:(NSApplication *)sender openFile:(NSString *)filename
{
    //[kinView newPath:gcode([filename cStringUsingEncoding:NSASCIIStringEncoding])];
    [window setTitle:[filename lastPathComponent]];
    return YES;
}

-(void)applicationWillTerminate:(NSNotification *)notification{
    if (connected == 0) {
        swrite("q\n");
        sclose();
    }
}

@end
