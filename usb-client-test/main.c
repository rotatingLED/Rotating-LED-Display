/* Name: main.c
 * Project: AVR bootloader HID
 * Author: Christian Starkjohann
 * Creation Date: 2007-03-19
 * Tabsize: 4
 * Copyright: (c) 2007 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: Proprietary, free under certain conditions. See Documentation.
 * This Revision: $Id: main.c 787 2010-05-30 20:54:25Z cs $
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "usbcalls.h"

#include <sys/time.h>
#include <time.h>


#define IDENT_VENDOR_NUM        0xffff
#define IDENT_VENDOR_STRING     "ZHAW"
//#define IDENT_PRODUCT_NUM       0x001f
#define IDENT_PRODUCT_NUM       0x5740
#define IDENT_PRODUCT_STRING    "LED controller"

/* ------------------------------------------------------------------------- */

char *usbErrorMessage(int errCode) {
	static char buffer[80];

	switch (errCode) {
	case USB_ERROR_ACCESS:
		return "Access to device denied";
	case USB_ERROR_NOTFOUND:
		return "The specified device was not found";
	case USB_ERROR_BUSY:
		return "The device is used by another application";
	case USB_ERROR_IO:
		return "Communication error with device";
	default:
		sprintf(buffer, "Unknown USB error %d", errCode);
		return buffer;
	}
	return NULL; /* not reached */
}

typedef struct deviceInfo {
	char reportId;
	char pageSize[2];
	char flashSize[4];
} deviceInfo_t;

typedef struct deviceData {
	char reportId;
	char address[3];
	char data[128];
} deviceData_t;

//static int uploadData(char *dataBuffer, int startAddr, int endAddr) {
//	usbDevice_t *dev = NULL;
//	int err = 0, len, mask, pageSize, deviceSize;
//	union {
//		char bytes[1];
//		deviceInfo_t info;
//		deviceData_t data;
//	} buffer;
//
//	if ((err = usbOpenDevice(&dev, IDENT_VENDOR_NUM, IDENT_VENDOR_STRING, IDENT_PRODUCT_NUM, IDENT_PRODUCT_STRING, 1))
//			!= 0) {
//		fprintf(stderr, "Error opening HIDBoot device: %s\n", usbErrorMessage(err));
//		goto errorOccurred;
//	}
//	len = sizeof(buffer);
//	if (endAddr > startAddr) { // we need to upload data
//		if ((err = usbGetReport(dev, USB_HID_REPORT_TYPE_FEATURE, 1, buffer.bytes, &len)) != 0) {
//			fprintf(stderr, "Error reading page size: %s\n", usbErrorMessage(err));
//			goto errorOccurred;
//		}
//		if (len < sizeof(buffer.info)) {
//			fprintf(stderr, "Not enough bytes in device info report (%d instead of %d)\n", len, (int) sizeof(buffer.info));
//			err = -1;
//			goto errorOccurred;
//		}
//		pageSize = getUsbInt(buffer.info.pageSize, 2);
//		deviceSize = getUsbInt(buffer.info.flashSize, 4);
//		printf("Page size   = %d (0x%x)\n", pageSize, pageSize);
//		printf("Device size = %d (0x%x); %d bytes remaining\n", deviceSize, deviceSize, deviceSize - 2048);
//		if (endAddr > deviceSize - 2048) {
//			fprintf(stderr, "Data (%d bytes) exceeds remaining flash size!\n", endAddr);
//			err = -1;
//			goto errorOccurred;
//		}
//		if (pageSize < 128) {
//			mask = 127;
//		} else {
//			mask = pageSize - 1;
//		}
//		startAddr &= ~mask; /* round down */
//		endAddr = (endAddr + mask) & ~mask; /* round up */
//		printf("Uploading %d (0x%x) bytes starting at %d (0x%x)\n", endAddr - startAddr, endAddr - startAddr, startAddr,
//				startAddr);
//		while (startAddr < endAddr) {
//			buffer.data.reportId = 2;
//			memcpy(buffer.data.data, dataBuffer + startAddr, 128);
//			setUsbInt(buffer.data.address, startAddr, 3);
//			printf("\r0x%05x ... 0x%05x", startAddr, startAddr + (int) sizeof(buffer.data.data));
//			fflush(stdout);
//			if ((err = usbSetReport(dev, USB_HID_REPORT_TYPE_FEATURE, buffer.bytes, sizeof(buffer.data))) != 0) {
//				fprintf(stderr, "Error uploading data block: %s\n", usbErrorMessage(err));
//				goto errorOccurred;
//			}
//			startAddr += sizeof(buffer.data.data);
//		}
//		printf("\n");
//	}
//	if (leaveBootLoader) {
//		/* and now leave boot loader: */
//		buffer.info.reportId = 1;
//		usbSetReport(dev, USB_HID_REPORT_TYPE_FEATURE, buffer.bytes, sizeof(buffer.info));
//		/* Ignore errors here. If the device reboots before we poll the response,
//		 * this request fails.
//		 */
//	}
//	errorOccurred: if (dev != NULL)
//		usbCloseDevice(dev);
//	return err;
//}

/* ------------------------------------------------------------------------- */

int main(int argc, char **argv) {
	int err;
   struct timeval start, end;
   long mtime, seconds, useconds;

	usbDevice_t *dev = NULL;

	if ((err = usbOpenDevice(&dev, IDENT_VENDOR_NUM, IDENT_VENDOR_STRING, IDENT_PRODUCT_NUM, IDENT_PRODUCT_STRING, 1))
			!= 0) {
		fprintf(stderr, "Error opening HIDBoot device: %s\n", usbErrorMessage(err));
		return 1;
	} else {
		printf("Device opened: %04x/%s:%04x/%s\n", IDENT_VENDOR_NUM, IDENT_VENDOR_STRING, IDENT_PRODUCT_NUM, IDENT_PRODUCT_STRING);
	}

	char buffer[10240] = {0};

	for(int i = 0; i < 100; i++) {
	    gettimeofday(&start, NULL);
		if ((err = usbWriteString(dev, buffer, sizeof(buffer))) != 0) {
			fprintf(stderr, "Error uploading data block: %s\n", usbErrorMessage(err));
		} else {
			printf("Dummy %i sent\n", i + 1);
		}
	    gettimeofday(&end, NULL);


	    seconds  = end.tv_sec  - start.tv_sec;
	    useconds = end.tv_usec - start.tv_usec;

	    mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;

	    printf("Sent %i Bytes within %ld milliseconds: %lf Bytes/s\n", (int)sizeof(buffer), mtime, (double)sizeof(buffer) / mtime * 1000.0);
	}

   time_t t;
   struct tm *tmp;
   t = time(NULL);
   tmp = localtime(&t);
   if (tmp == NULL) {
       perror("localtime");
       exit(EXIT_FAILURE);
   }
   if (strftime(buffer, sizeof(buffer), "%H:%M:%S", tmp) == 0) {
       fprintf(stderr, "strftime returned 0");
   }
//   strcpy(buffer, "Hoi Dave");

	if ((err = usbWriteString(dev, buffer, strlen(buffer)+1)) != 0) {
		fprintf(stderr, "Error uploading data block: %s\n", usbErrorMessage(err));
	} else {
		printf("String sent...\n");
	}

	if (dev != NULL) {
		usbCloseDevice(dev);
	}

	return 0;
}

/* ------------------------------------------------------------------------- */

