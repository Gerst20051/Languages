/**********************************************************************
 *  This program is free software; you can redistribute it and/or     *
 *  modify it under the terms of the GNU General Public License       *
 *  as published by the Free Software Foundation; either version 2    *
 *  of the License, or (at your option) any later version.            *
 *                                                                    *
 *  This program is distributed in the hope that it will be useful,   *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of    *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the     *
 *  GNU General Public License for more details.                      *
 *                                                                    *
 *  You should have received a copy of the GNU General Public License *
 *  along with this program; if not, write to the Free Software       *
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor,                *
 *  Boston, MA  02110-1301, USA.                                      *
 *                                                                    *
 *  ---                                                               *
 *  Copyright (C) 2009, Justin Davis <tuxdavis@gmail.com>             *
 **********************************************************************/

#ifndef WINVER
#define WINVER 0x0500
#endif

#include <QtGui>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <winioctl.h>
#include "disk.h"

int getPhysicalDeviceID(int device)
{
	HANDLE hDevice;
	DWORD bytesreturned;
	BOOL bResult;
	DEVICE_NUMBER deviceInfo;
	char devicename[] = "\\\\.\\A:";
	devicename[4] += device;
	hDevice = CreateFile(devicename, 0, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		char *errormessage=NULL;
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, errormessage, 0, NULL);
		QMessageBox::critical(NULL, "File Error", QString("An error occurred when attempting to get a handle on the device.\nThis usually means something is currently accessing the device; please close all applications and try again.\n\nError %1: %2").arg(GetLastError()).arg(errormessage));
		LocalFree(errormessage);
	}
	bResult = DeviceIoControl(hDevice, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0, &deviceInfo, sizeof(deviceInfo), &bytesreturned, NULL);
	if (!bResult)
	{
		char *errormessage=NULL;
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, errormessage, 0, NULL);
		QMessageBox::critical(NULL, "File Error", QString("An error occurred when attempting to get a handle on the device.\nThis usually means something is currently accessing the device; please close all applications and try again.\n\nError %1: %2").arg(GetLastError()).arg(errormessage));
		LocalFree(errormessage);
		return -1;
	}
	CloseHandle(hDevice);
	return (int)deviceInfo.DeviceNumber;
}

HANDLE getHandleOnFile(char *filelocation, DWORD access)
{
	HANDLE hFile;
	char *location = new char[5 + strlen(filelocation)];
  sprintf(location, "\\\\.\\%s", filelocation);
  hFile = CreateFile(location, access, 0, NULL, (access == GENERIC_READ) ? OPEN_EXISTING:CREATE_ALWAYS, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		char *errormessage=NULL;
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, errormessage, 0, NULL);
		QMessageBox::critical(NULL, "File Error", QString("An error occurred when attempting to get a handle on the file.\nError %1: %2").arg(GetLastError()).arg(errormessage));
		LocalFree(errormessage);
	}
	delete location;
	return hFile;
}

HANDLE getHandleOnDevice(int device, DWORD access)
{
	HANDLE hDevice;
	QString devicename = QString("\\\\.\\PhysicalDrive%1").arg(device);
	hDevice = CreateFile(devicename.toAscii().data(), access, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		char *errormessage=NULL;
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, errormessage, 0, NULL);
		QMessageBox::critical(NULL, "Device Error", QString("An error occurred when attempting to get a handle on the device.\nError %1: %2").arg(GetLastError()).arg(errormessage));
		LocalFree(errormessage);
	}
	return hDevice;
}

HANDLE getHandleOnVolume(int volume, DWORD access)
{
	HANDLE hVolume;
	char volumename[] = "\\\\.\\A:";
	volumename[4] += volume;
	hVolume = CreateFile(volumename, access, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (hVolume == INVALID_HANDLE_VALUE)
	{
		char *errormessage=NULL;
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, errormessage, 0, NULL);
		QMessageBox::critical(NULL, "Volume Error", QString("An error occurred when attempting to get a handle on the volume.\nError %1: %2").arg(GetLastError()).arg(errormessage));
		LocalFree(errormessage);
	}
	return hVolume;
}

bool getLockOnVolume(HANDLE handle)
{
	DWORD bytesreturned;
	BOOL bResult;
	bResult = DeviceIoControl(handle, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &bytesreturned, NULL);
	if (!bResult)
	{
		char *errormessage=NULL;
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, errormessage, 0, NULL);
		QMessageBox::critical(NULL, "Lock Error", QString("An error occurred when attempting to lock the volume.\nError %1: %2").arg(GetLastError()).arg(errormessage));
		LocalFree(errormessage);
	}
	return (bResult == TRUE);
}

bool removeLockOnVolume(HANDLE handle)
{
	DWORD junk;
	BOOL bResult;
	bResult = DeviceIoControl(handle, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, &junk, NULL);
	if (!bResult)
	{
		char *errormessage=NULL;
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, errormessage, 0, NULL);
		QMessageBox::critical(NULL, "Unlock Error", QString("An error occurred when attempting to unlock the volume.\nError %1: %2").arg(GetLastError()).arg(errormessage));
		LocalFree(errormessage);
	}
	return (bResult == TRUE);
}

bool unmountVolume(HANDLE handle)
{
	DWORD junk;
	BOOL bResult;
	bResult = DeviceIoControl(handle, FSCTL_DISMOUNT_VOLUME, NULL, 0, NULL, 0, &junk, NULL);
	if (!bResult)
	{
		char *errormessage=NULL;
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, errormessage, 0, NULL);
		QMessageBox::critical(NULL, "Dismount Error", QString("An error occurred when attempting to dismount the volume.\nError %1: %2").arg(GetLastError()).arg(errormessage));
		LocalFree(errormessage);
	}
	return (bResult == TRUE);
}

bool isVolumeUnmounted(HANDLE handle)
{
	DWORD junk;
	BOOL bResult;
	bResult = DeviceIoControl(handle, FSCTL_IS_VOLUME_MOUNTED, NULL, 0, NULL, 0, &junk, NULL);
	return (bResult == FALSE);
}

char *readSectorDataFromHandle(HANDLE handle, unsigned long long startsector, unsigned long long numsectors, unsigned long long sectorsize)
{
	unsigned long bytesread;
	char *data = new char[sectorsize * numsectors];
	SetFilePointer(handle, startsector * sectorsize, NULL, FILE_BEGIN);
	if (!ReadFile(handle, data, sectorsize * numsectors, &bytesread, NULL))
	{
		char *errormessage=NULL;
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, errormessage, 0, NULL);
		QMessageBox::critical(NULL, "Read Error", QString("An error occurred when attempting to read data from handle.\nError %1: %2").arg(GetLastError()).arg(errormessage));
		LocalFree(errormessage);
		delete data;
		data = NULL;
	}
	return data;
}

bool writeSectorDataToHandle(HANDLE handle, char *data, unsigned long long startsector, unsigned long long numsectors, unsigned long long sectorsize)
{
	unsigned long byteswritten;
	BOOL bResult;
	SetFilePointer(handle, startsector * sectorsize, NULL, FILE_BEGIN);
	bResult = WriteFile(handle, data, sectorsize * numsectors, &byteswritten, NULL);
	if (!bResult)
	{
		char *errormessage=NULL;
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, errormessage, 0, NULL);
		QMessageBox::critical(NULL, "Write Error", QString("An error occurred when attempting to write data from handle.\nError %1: %2").arg(GetLastError()).arg(errormessage));
		LocalFree(errormessage);
	}
	return (bResult == TRUE);
}

unsigned long long getNumberOfSectors(HANDLE handle, unsigned long long *sectorsize)
{
	DWORD junk;
	DISK_GEOMETRY geometry;
	BOOL bResult;
	bResult = DeviceIoControl(handle, IOCTL_DISK_GET_DRIVE_GEOMETRY, NULL, 0, &geometry, sizeof(geometry), &junk, NULL);
	if (!bResult)
	{
		char *errormessage=NULL;
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, errormessage, 0, NULL);
		QMessageBox::critical(NULL, "Device Error", QString("An error occurred when attempting to get the device's geometry.\nError %1: %2").arg(GetLastError()).arg(errormessage));
		LocalFree(errormessage);
		return 0;
	}
	if (sectorsize != NULL)
		*sectorsize = (unsigned long long)geometry.BytesPerSector;
	return (unsigned long long)geometry.Cylinders.QuadPart * (unsigned long long)geometry.TracksPerCylinder * (unsigned long long)geometry.SectorsPerTrack;
}

unsigned long long getFileSizeInSectors(HANDLE handle, unsigned long long sectorsize)
{
	LARGE_INTEGER filesize;
	GetFileSizeEx(handle, &filesize);
	return (unsigned long long)filesize.QuadPart / sectorsize;
}

bool spaceAvailable(char *location, unsigned long long spaceneeded)
{
	ULARGE_INTEGER freespace;
	BOOL bResult;
	bResult = GetDiskFreeSpaceEx(location, NULL, NULL, &freespace);
	if (!bResult)
	{
		char *errormessage=NULL;
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, errormessage, 0, NULL);
		QMessageBox::critical(NULL, "Free Space Error", QString("Failed to get the free space on drive %1.\nError %2: %3\nChecking of free space will be skipped.").arg(location).arg(GetLastError()).arg(errormessage));
		return true;
	}
	return (spaceneeded <= freespace.QuadPart);
}
