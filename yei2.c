// tested only on Samsung Galaxy Note 3

#include <linux/uinput.h>
#include <sys/param.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char adb_command[] = "adb shell getevent -q";
   
int landscape = 1;
int left_handed = 0;

int touchpad_enabled = 0;
int pen_enabled = 1;

void emit(int fd, int type, int code, int val) {
	struct input_event ie;

	ie.type = type;
	ie.code = code;
	ie.value = val;
	/* timestamp values below are ignored */
	ie.time.tv_sec = 0;
	ie.time.tv_usec = 0;

	write(fd, &ie, sizeof(ie));
}

void emit_input_event(int fd, struct input_event *ie) {
	write(fd, ie, sizeof(*ie));
}

int main(void) {
    int resolution = 10;
    int swapXY = landscape;
    int invertX = landscape & left_handed;
    int invertY = landscape & (1 - left_handed);

    int touchpad_MAX_X = 1079;
    int touchpad_MAX_Y = 1919;
    int touchpad_resolution = resolution;
    int touchpad_max_fingers = 5;
    __u16 touchpad_BNT_TOOLS[] = {BTN_TOOL_FINGER, BTN_TOOL_DOUBLETAP, BTN_TOOL_TRIPLETAP, BTN_TOOL_QUADTAP, BTN_TOOL_QUINTTAP};

    int pen_MAX_X = 7074;
    int pen_MAX_Y = 12576;
    // int pen_resolution = (int)(resolution * 6.55);
    // int pen_resolution = (int)( ((float) resolution * pen_MAX_X) / touchpad_MAX_X + 0.5);
	int pen_resolution = 30;

	struct uinput_setup usetup;
	struct uinput_abs_setup abssetup;

	int touchpad_fd = -1;
    if (touchpad_enabled) {
        touchpad_fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

        /* enable mouse button left and relative events */
        ioctl(touchpad_fd, UI_SET_EVBIT, EV_KEY);
        ioctl(touchpad_fd, UI_SET_KEYBIT, BTN_LEFT);
        // ioctl(touchpad_fd, UI_SET_KEYBIT, BTN_RIGHT);
        ioctl(touchpad_fd, UI_SET_KEYBIT, BTN_TOUCH);
        ioctl(touchpad_fd, UI_SET_KEYBIT, BTN_TOOL_FINGER);
        ioctl(touchpad_fd, UI_SET_KEYBIT, BTN_TOOL_DOUBLETAP);
        ioctl(touchpad_fd, UI_SET_KEYBIT, BTN_TOOL_TRIPLETAP);
        ioctl(touchpad_fd, UI_SET_KEYBIT, BTN_TOOL_QUADTAP);
        ioctl(touchpad_fd, UI_SET_KEYBIT, BTN_TOOL_QUINTTAP);

        /*
        ABS (0003): ABS_MT_SLOT		   : value 0, min 0, max 9, fuzz 0, flat 0, resolution 0
                    ABS_MT_TOUCH_MAJOR	: value 0, min 0, max 255, fuzz 0, flat 0, resolution 0
                    ABS_MT_TOUCH_MINOR	: value 0, min 0, max 255, fuzz 0, flat 0, resolution 0
                    ABS_MT_WIDTH_MAJOR	: value 0, min 0, max 255, fuzz 0, flat 0, resolution 0
                    ABS_MT_POSITION_X	 : value 0, min 0, max 1079, fuzz 0, flat 0, resolution 0
                    ABS_MT_POSITION_Y	 : value 0, min 0, max 1919, fuzz 0, flat 0, resolution 0
                    ABS_MT_TRACKING_ID	: value 0, min 0, max 65535, fuzz 0, flat 0, resolution 0
                    ABS_MT_DISTANCE	   : value 0, min 0, max 255, fuzz 0, flat 0, resolution 0
                    003e				  : value 0, min 0, max 1, fuzz 0, flat 0, resolution 0
        */

        ioctl(touchpad_fd, UI_SET_EVBIT, EV_ABS);
        ioctl(touchpad_fd, UI_SET_ABSBIT, ABS_MT_SLOT);
        // ioctl(touchpad_fd, UI_SET_ABSBIT, ABS_MT_TOUCH_MAJOR);
        // ioctl(touchpad_fd, UI_SET_ABSBIT, ABS_MT_TOUCH_MINOR);
        // ioctl(touchpad_fd, UI_SET_ABSBIT, ABS_MT_WIDTH_MAJOR);
        // ioctl(touchpad_fd, UI_SET_ABSBIT, ABS_X);
        // ioctl(touchpad_fd, UI_SET_ABSBIT, ABS_Y);
        ioctl(touchpad_fd, UI_SET_ABSBIT, ABS_MT_POSITION_X);
        ioctl(touchpad_fd, UI_SET_ABSBIT, ABS_MT_POSITION_Y);
        ioctl(touchpad_fd, UI_SET_ABSBIT, ABS_MT_TRACKING_ID);
        // ioctl(touchpad_fd, UI_SET_ABSBIT, ABS_MT_DISTANCE);
        // ioctl(touchpad_fd, UI_SET_ABSBIT, 0x003e);
        // ioctl(touchpad_fd, UI_SET_ABSBIT, ABS_TOOL_WIDTH);
        ioctl(touchpad_fd, UI_SET_ABSBIT, ABS_PRESSURE);

        abssetup = (struct uinput_abs_setup) {ABS_MT_SLOT,		  (struct input_absinfo) {0, 0, 9, 0, 0, 0}};
        ioctl(touchpad_fd, UI_ABS_SETUP, &abssetup);
        // abssetup = (struct uinput_abs_setup) {ABS_MT_TOUCH_MAJOR,   (struct input_absinfo) {0, 0, 255, 0, 0, 0}};
        // ioctl(touchpad_fd, UI_ABS_SETUP, &abssetup);
        // abssetup = (struct uinput_abs_setup) {ABS_MT_TOUCH_MINOR,   (struct input_absinfo) {0, 0, 255, 0, 0, 0}};
        // ioctl(touchpad_fd, UI_ABS_SETUP, &abssetup);
        // abssetup = (struct uinput_abs_setup) {ABS_MT_WIDTH_MAJOR,   (struct input_absinfo) {0, 0, 255, 0, 0, 0}};
        // ioctl(touchpad_fd, UI_ABS_SETUP, &abssetup);
        abssetup = (struct uinput_abs_setup) {ABS_PRESSURE,         (struct input_absinfo) {0, 0, 255, 0, 0, 0}};
        ioctl(touchpad_fd, UI_ABS_SETUP, &abssetup);
        abssetup = (struct uinput_abs_setup) {ABS_MT_POSITION_X,	(struct input_absinfo) {0, 0, touchpad_MAX_X, 0, 0, touchpad_resolution}};
        ioctl(touchpad_fd, UI_ABS_SETUP, &abssetup);
        abssetup = (struct uinput_abs_setup) {ABS_MT_POSITION_Y,	(struct input_absinfo) {0, 0, touchpad_MAX_Y, 0, 0, touchpad_resolution}};
        ioctl(touchpad_fd, UI_ABS_SETUP, &abssetup);
        // abssetup = (struct uinput_abs_setup) {ABS_X,            	(struct input_absinfo) {0, 0, touchpad_MAX_X, 0, 0, 0}};
        // ioctl(touchpad_fd, UI_ABS_SETUP, &abssetup);
        // abssetup = (struct uinput_abs_setup) {ABS_Y,            	(struct input_absinfo) {0, 0, touchpad_MAX_Y, 0, 0, 0}};
        // ioctl(touchpad_fd, UI_ABS_SETUP, &abssetup);
        abssetup = (struct uinput_abs_setup) {ABS_MT_TRACKING_ID,   (struct input_absinfo) {0, 0, 65535, 0, 0, 0}};
        ioctl(touchpad_fd, UI_ABS_SETUP, &abssetup);
        // abssetup = (struct uinput_abs_setup) {ABS_MT_DISTANCE,	  (struct input_absinfo) {0, 0, 255, 0, 0, 0}};
        // ioctl(touchpad_fd, UI_ABS_SETUP, &abssetup);
        // abssetup = (struct uinput_abs_setup) {0x003e,			   (struct input_absinfo) {0, 0, 1, 0, 0, 0}};
        // ioctl(touchpad_fd, UI_ABS_SETUP, &abssetup);
        
        memset(&usetup, 0, sizeof(usetup));
        usetup.id.bustype = BUS_USB;
        usetup.id.vendor = 0x1234; /* sample vendor */
        usetup.id.product = 0x5678; /* sample product */
        strcpy(usetup.name, "Touchpad Note 3");

        ioctl(touchpad_fd, UI_DEV_SETUP, &usetup);
        ioctl(touchpad_fd, UI_DEV_CREATE);
    }


    int pen_fd;
    if (pen_enabled) {
        pen_fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

        ioctl(pen_fd, UI_SET_EVBIT, EV_KEY);
        ioctl(pen_fd, UI_SET_KEYBIT, KEY_MENU);
        ioctl(pen_fd, UI_SET_KEYBIT, KEY_WAKEUP);
        ioctl(pen_fd, UI_SET_KEYBIT, KEY_BACK);
        ioctl(pen_fd, UI_SET_KEYBIT, KEY_UNKNOWN);
        ioctl(pen_fd, UI_SET_KEYBIT, BTN_DIGI);
        ioctl(pen_fd, UI_SET_KEYBIT, BTN_TOOL_RUBBER);
        ioctl(pen_fd, UI_SET_KEYBIT, BTN_TOUCH);
        ioctl(pen_fd, UI_SET_KEYBIT, BTN_STYLUS);
        ioctl(pen_fd, UI_SET_KEYBIT, KEY_ALS_TOGGLE);
        ioctl(pen_fd, UI_SET_KEYBIT, 0x2f1);
        ioctl(pen_fd, UI_SET_KEYBIT, 0x2f2);
        ioctl(pen_fd, UI_SET_KEYBIT, 0x2f3);
        ioctl(pen_fd, UI_SET_KEYBIT, 0x2f4);
        ioctl(pen_fd, UI_SET_KEYBIT, 0x2f5);
        ioctl(pen_fd, UI_SET_KEYBIT, 0x2f6);

        ioctl(pen_fd, UI_SET_EVBIT, EV_ABS);
        ioctl(pen_fd, UI_SET_ABSBIT, ABS_X);
        ioctl(pen_fd, UI_SET_ABSBIT, ABS_Y);
        ioctl(pen_fd, UI_SET_ABSBIT, ABS_PRESSURE);
        ioctl(pen_fd, UI_SET_ABSBIT, ABS_DISTANCE);

        abssetup = (struct uinput_abs_setup) {ABS_X,	        (struct input_absinfo) {0, 0, pen_MAX_X, 0, 0, pen_resolution}};
        ioctl(pen_fd, UI_ABS_SETUP, &abssetup);
        abssetup = (struct uinput_abs_setup) {ABS_Y,	        (struct input_absinfo) {0, 0, pen_MAX_Y, 0, 0, pen_resolution}};
        ioctl(pen_fd, UI_ABS_SETUP, &abssetup);
        abssetup = (struct uinput_abs_setup) {ABS_PRESSURE,     (struct input_absinfo) {0, 0, 1023, 0, 0, 0}};
        ioctl(pen_fd, UI_ABS_SETUP, &abssetup);
        abssetup = (struct uinput_abs_setup) {ABS_DISTANCE,     (struct input_absinfo) {0, 0, 1024, 0, 0, 0}};
        ioctl(pen_fd, UI_ABS_SETUP, &abssetup);

        memset(&usetup, 0, sizeof(usetup));
        usetup.id.bustype = BUS_USB;
        usetup.id.vendor = 1386;
        usetup.id.product = 890;
        strcpy(usetup.name, "S Pen Note 3");

        ioctl(pen_fd, UI_DEV_SETUP, &usetup);
        ioctl(pen_fd, UI_DEV_CREATE);
    }



	sleep(1);

    FILE *adb_file = popen(adb_command, "r");
	if (adb_file == NULL){
		perror(adb_command);
		exit(1);
	}

    int touchpad_fingers = 0;
    int touchpad_touch = 0;
    __u32 touchpad_touch_value = 0;
	while (1) {
        char adb_event[32];
        struct input_event ie;

        fscanf(adb_file, "%s %" SCNx16 " %" SCNx16 " %" SCNx32 "\n", adb_event, &ie.type, &ie.code, &ie.value);

        if (memcmp(adb_event, "/dev/input/event", 16) == 0){
            int n_event = atoi(adb_event + 16);

            if (n_event == 4 && touchpad_enabled) {
                if (ie.code == ABS_MT_SLOT) {
                    touchpad_fingers = MAX(touchpad_fingers, ie.value);
                }

                if (invertX && ie.code == ABS_MT_POSITION_X) {
                    ie.value = touchpad_MAX_X - ie.value;
                }
                if (invertY && ie.code == ABS_MT_POSITION_Y) {
                    ie.value = touchpad_MAX_Y - ie.value;
                }
                if (swapXY) {
                    __u16 code = ie.code;
                    if (code == ABS_MT_POSITION_X)
                        ie.code = ABS_MT_POSITION_Y;
                    if (code == ABS_MT_POSITION_Y)
                        ie.code = ABS_MT_POSITION_X;
                }
              
                if (ie.code == ABS_MT_WIDTH_MAJOR)
                    ie.code = ABS_PRESSURE;
                
                if (ie.type == EV_SYN && touchpad_touch) {
                	printf("touch %d \n", touchpad_touch_value);
                    // touchpad_fingers = MIN(touchpad_fingers, touchpad_max_fingers);
                // //  printf("%d finfers, %d \n", touchpad_fingers, touchpad_touch_value);
                    struct input_event ie_touch = {EV_KEY, BTN_TOUCH, touchpad_touch_value};
                    emit_input_event(touchpad_fd, &ie_touch);
                    // struct input_event ie_tool = {EV_KEY, BTN_TOOL_FINGER, touchpad_touch_value};
                    // emit_input_event(touchpad_fd, &ie_tool);
        		    touchpad_touch = 0;
                    // touchpad_fingers = 0;
                }
// 
                if (ie.code == BTN_TOUCH) {
                    touchpad_touch = 1;
                    touchpad_touch_value = ie.value;
                }
                else
                    emit_input_event(touchpad_fd, &ie);
                
                // if (ie.code == BTN_TOUCH) { /// TODO: multitouch
                    // ie.code = BTN_TOOL_FINGER;
                    // emit_input_event(touchpad_fd, &ie);
                // }
                if (ie.code == ABS_MT_POSITION_X){
                    ie.code = ABS_X;
                    // emit_input_event(touchpad_fd, &ie);
                }
                if (ie.code == ABS_MT_POSITION_Y){
                    ie.code = ABS_Y;
                    // emit_input_event(touchpad_fd, &ie);
                }   
            }

            if (n_event == 3 && pen_enabled) {
                if (invertX && ie.code == ABS_X) {
                    ie.value = pen_MAX_X - ie.value;
                }
                if (invertY && ie.code == ABS_Y) {
                    ie.value = pen_MAX_Y - ie.value;
                }
                if (ie.code == ABS_Y) {
                    // ie.value /= 2;
                }
                if (swapXY) {
                    __u16 code = ie.code;
                    if (code == ABS_X)
                        ie.code = ABS_Y;
                    if (code == ABS_Y)
                        ie.code = ABS_X;
                }
                emit_input_event(pen_fd, &ie);
            }

        }
    }

	sleep(1);

	ioctl(touchpad_fd, UI_DEV_DESTROY);
	close(touchpad_fd);

	return 0;
}
