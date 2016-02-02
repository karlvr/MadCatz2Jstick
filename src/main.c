/* ======================================================================== */
/*  This program is free software; you can redistribute it and/or modify    */
/*  it under the terms of the GNU General Public License as published by    */
/*  the Free Software Foundation; either version 2 of the License, or       */
/*  (at your option) any later version.                                     */
/*                                                                          */
/*  This program is distributed in the hope that it will be useful,         */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of          */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU       */
/*  General Public License for more details.                                */
/*                                                                          */
/*  You should have received a copy of the GNU General Public License       */
/*  along with this program; if not, write to the Free Software             */
/*  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.               */
/* ======================================================================== */
/*                 Copyright (c) 2016, Florian Mueller, Karl von Randow     */
/* ======================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/time.h>
#include <termios.h>
#include <signal.h>
#include <time.h>

#include "uinput_gamepad.h"
#include "uinput_kbd.h"
#include "input_madcatz.h"

// TODO Extract all magic numbers and collect them as defines in at a central location

#define GPADSNUM 2

UINP_KBD_DEV uinp_kbd;
UINP_GPAD_DEV uinp_gpads[GPADSNUM];
INP_MADCATZ_DEV madcatzdev;

int main(int argc, char* argv[]) {
	int result = 0;
	int rd, ctr;

	int detach = 0;
	int opt;
	while ((opt = getopt(argc, argv, "+d")) != -1) {
		switch (opt) {
			case 'd':
				detach = 1;
				break;
			default:
				fprintf(stderr, "Usage: %s [-d]\n", argv[0]);
				exit(1);
				break;
		}
	}

	printf("[MadCatz] Getting exclusive access: ");
	result = input_madcatz_open(&madcatzdev, INPUT_MADCATZ_TYPE);
	printf("%s\n", (result == 0) ? "SUCCESS" : "FAILURE");
	if (result != 0) {
		exit(-1);
	}

	uinput_gpad_open(&uinp_gpads[0], UINPUT_GPAD_TYPE_MADCATZ);
	uinput_kbd_open(&uinp_kbd);

	if (detach) {
		if (daemon(0, 1)) {
			perror("daemon");
			return 1;
		}
	}

	while (1) {
		rd = input_madcatz_read(&madcatzdev);
		if (rd < 0) {
			break;
		}

		for (ctr = 0; ctr < rd; ctr++) {
			if (madcatzdev.ev[ctr].type == 0)
				continue;
			if (madcatzdev.ev[ctr].type == EV_MSC)
				continue;
			if (EV_ABS == madcatzdev.ev[ctr].type) {
				const __s32 value = madcatzdev.ev[ctr].value;

				switch (madcatzdev.ev[ctr].code) {
					case 0: /* LS x */
						uinput_gpad_write(&uinp_gpads[0], ABS_X, value < 127 ? 0 : (value > 128 ? 4 : 2), EV_ABS);
						break;
					case 1: /* LS y */
						uinput_gpad_write(&uinp_gpads[0], ABS_Y, value < 127 ? 0 : (value > 128 ? 4 : 2), EV_ABS);
						break;

					case 16: /* DP x */
						uinput_gpad_write(&uinp_gpads[0], ABS_X, value < 0 ? 0 : (value > 0 ? 4 : 2), EV_ABS);
						break;

					case 17: /* DP y */
						uinput_gpad_write(&uinp_gpads[0], ABS_Y, value < 0 ? 0 : (value > 0 ? 4 : 2), EV_ABS);
						break;

					case 2: /* RS x */
						uinput_gpad_write(&uinp_gpads[0], ABS_X, value < 127 ? 0 : (value > 128 ? 4 : 2), EV_ABS);
						break;

					case 5: /* RS y */
						uinput_gpad_write(&uinp_gpads[0], ABS_Y, value < 127 ? 0 : (value > 128 ? 4 : 2), EV_ABS);
						break;
				}

			} else if (EV_KEY == madcatzdev.ev[ctr].type) {

				switch (madcatzdev.ev[ctr].code) {
					case BTN_TL2:
						uinput_gpad_write(&uinp_gpads[0], BTN_SELECT, madcatzdev.ev[ctr].value, EV_KEY);
						break;
					case BTN_TR2:
						uinput_gpad_write(&uinp_gpads[0], BTN_START, madcatzdev.ev[ctr].value, EV_KEY);
						break;
					default:
						uinput_gpad_write(&uinp_gpads[0], madcatzdev.ev[ctr].code, madcatzdev.ev[ctr].value, EV_KEY);
						break;
				}
			}
		}
	}

	printf("Exiting.\n");
	input_madcatz_close(&madcatzdev);
	uinput_gpad_close(&uinp_gpads[0]);
	uinput_gpad_close(&uinp_gpads[1]);
	uinput_kbd_close(&uinp_kbd);
	return 0;
}
