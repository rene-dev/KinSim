//
//  AppDelegate.m
//  kinsim
//
//  Created by Rene on 11/11/13.
//  Copyright (c) 2013 Rene. All rights reserved.
//

#import "AppDelegate.h"

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{

}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication{
    return YES;
}

-(BOOL)application:(NSApplication *)sender openFile:(NSString *)filename
{
    gcode([filename cStringUsingEncoding:NSASCIIStringEncoding]);
    return YES;
}

@end
