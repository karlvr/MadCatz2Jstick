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

#include <glob.h>
#include <errno.h>
#include "input_madcatz.h"

// declaration of supplementary functions  -------------------
int findMadCatzDevice(void);

// relizations ----------------------
int16_t input_madcatz_open(INP_MADCATZ_DEV* const xdev, INPUT_MADCATZ_TYPE_E type) {
	int result;

	// TODO check input parameter type
	xdev->fevdev = findMadCatzDevice();
	result = ioctl(xdev->fevdev, EVIOCGRAB, 1);
	return result;
}

int16_t input_madcatz_read(INP_MADCATZ_DEV* const xdev) {
	int rd;

	rd = read(xdev->fevdev, xdev->ev, sizeof(struct input_event) * 64);
	if (rd < 0)
		return -errno;
	return (rd / sizeof(struct input_event));
}

int16_t input_madcatz_close(INP_MADCATZ_DEV* const xdev) {
	int result;

	result = ioctl(xdev->fevdev, EVIOCGRAB, 0);
	close(xdev->fevdev);
	return result;
}

// supplementary functions -------------------

int findMadCatzDevice(void) {
	char name[256];
	char *filename;
	int fevdev = -1;
	int ctr;
	int rc;
	glob_t pglob;

	rc = glob("/dev/input/event*", 0, NULL, &pglob);
	if (rc) {
		printf("Failed to open event devices\n");
		return -1;
	}

	for (ctr = 0; ctr < pglob.gl_pathc; ++ctr) {
		filename = pglob.gl_pathv[ctr];
		fevdev = open(filename, O_RDONLY);
		if (fevdev == -1) {
			printf("Failed to open event device %s.\n", filename);
			continue;
		}

		ioctl(fevdev, EVIOCGNAME(sizeof(name)), name);

		if ((strcmp(name, "MadCatz PC USB Wired Stick 8818") == 0)) {
			printf("Found %s (%s)\n", filename, name);
			break;
		} else {
			close(fevdev);
		}
	}
	globfree(&pglob);

	return fevdev;
}

