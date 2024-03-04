#pragma clang optimize off

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#include <auby.hh>
#include <dlfcn.h>
#include <filesystem>
#import <mach-o/dyld.h>
#include <thread>

namespace auby {

void run_in_main_queue(std::function<void()> fn) {
    dispatch_async(dispatch_get_main_queue(), ^{
      fn(); // stupid clang format
    });
}

void alert(std::string msg) {
    UIAlertView* alert = [[UIAlertView alloc]
            initWithTitle:@"Auby"
                  message:[NSString stringWithUTF8String:msg.c_str()]
                 delegate:nil
        cancelButtonTitle:@"OK"
        otherButtonTitles:nil];
    [alert show];
    [alert release];
}

} // namespace auby

#pragma clang optimize on
