#import <Foundation/Foundation.h>
#import <ApplicationServices/ApplicationServices.h>

int main() {
    @autoreleasepool {
        for (int i = 0; i < 10; ++i) {
            // Create a key down event
            CGEventRef keyDown = CGEventCreateKeyboardEvent(NULL, kVK_ANSI_A, true);
            CGEventPost(kCGHIDEventTap, keyDown);

            // Create a key up event
            CGEventRef keyUp = CGEventCreateKeyboardEvent(NULL, kVK_ANSI_A, false);
            CGEventPost(kCGHIDEventTap, keyUp);

            // Release the events
            CFRelease(keyDown);
            CFRelease(keyUp);

            // Wait for a second
            [NSThread sleepForTimeInterval:1.0];
        }
    }

    return 0;
}
