

#include <iostream>
#include <windows.h>

#ifndef BIONUKG_DISKDRIVE_H
#define BIONUKG_DISKDRIVE_H

#ifdef namespace_bionukg
namespace bionukg
{
#endif /* namespace_bionukg */

	DWORD readDisk(void *buffer, int size, long long offset, HANDLE disk);
	DWORD readDiskContinue(void *buffer, int size, HANDLE disk);
	DWORD writeDisk(void *buffer, int size, long long offset, HANDLE disk);
	HANDLE openDiskRead(int physicalDiskNumber);
	HANDLE openDiskWrite(int physicalDiskNumber);
	HANDLE openDiskReadWrite(int physicalDiskNumber);
	BOOL closeDisk(HANDLE disk);
	long long getDiskOffset(HANDLE disk);
	bool diskOffsetAvailablity(HANDLE disk, long long offset);
	long long getDiskSize(char disk);
	bool setDiskOffset(HANDLE disk, long long offset);

	DWORD readDisk(void *buffer, int size, long long offset, HANDLE disk)
	{
		DWORD read;
		LONG high = offset >> 32, low = offset & 0xffffffff;
		SetFilePointer(disk,
					   low,
					   &high,
					   FILE_BEGIN);
		ReadFile(disk, buffer, size, &read, NULL);
		return read;
	}
	DWORD readDiskContinue(void *buffer, int size, HANDLE disk)
	{
		DWORD read;
		auto f = ReadFile(disk, buffer, size, &read, NULL);
		if (f == 0)
		{
			printf("error in readDiskContinue\n");
			return -1;
		}
		return read;
	}
	DWORD writeDisk(void *buffer, int size, long long offset, HANDLE disk)
	{
		DWORD write;
		LONG high = offset >> 32, low = offset & 0xffffffff;
		SetFilePointer(disk,
					   low,
					   &high,
					   FILE_BEGIN);
		WriteFile(disk, buffer, size, &write, NULL);
		return write;
	}
	DWORD writeDiskContinue(void *buffer, int size, HANDLE disk)
	{
		DWORD write;
		auto f = WriteFile(disk, buffer, size, &write, NULL);
		if (f == 0)
		{
			printf("error in writeDiskContinue\n");
			return -1;
		}
		return write;
	}

	HANDLE openDiskRead(int physicalDiskNumber)
	{
		char diskName[20];
		sprintf(diskName, "\\\\.\\PhysicalDrive%d", physicalDiskNumber);
		HANDLE disk = CreateFileA(diskName,
								  GENERIC_READ | FILE_READ_ATTRIBUTES,
								  FILE_SHARE_READ | FILE_SHARE_WRITE,
								  NULL,
								  OPEN_EXISTING,
								  FILE_ATTRIBUTE_NORMAL,
								  NULL);

		return disk;
	}
	HANDLE openDiskWrite(int physicalDiskNumber)
	{
		char diskName[20];
		sprintf(diskName, "\\\\.\\PhysicalDrive%d", physicalDiskNumber);
		HANDLE disk = CreateFileA(diskName,
								  GENERIC_WRITE | FILE_READ_ATTRIBUTES,
								  FILE_SHARE_READ | FILE_SHARE_WRITE,
								  NULL,
								  OPEN_EXISTING,
								  FILE_ATTRIBUTE_NORMAL,
								  NULL);

		return disk;
	}
	HANDLE openDiskReadWrite(int physicalDiskNumber)
	{
		char diskName[20];
		sprintf(diskName, "\\\\.\\PhysicalDrive%d", physicalDiskNumber);
		HANDLE disk = CreateFileA(diskName,
								  GENERIC_READ | GENERIC_WRITE | FILE_READ_ATTRIBUTES,
								  FILE_SHARE_READ | FILE_SHARE_WRITE,
								  NULL,
								  OPEN_EXISTING,
								  FILE_ATTRIBUTE_NORMAL,
								  NULL);

		return disk;
	}

	inline BOOL closeDisk(HANDLE disk)
	{
		return CloseHandle(disk);
	}
	HANDLE openDisk(char diskName)
	{
		char diskName0[20];
		sprintf(diskName0, "\\\\.\\%c:", diskName);
		HANDLE disk = CreateFileA(diskName0,
								  GENERIC_READ | GENERIC_WRITE | FILE_READ_ATTRIBUTES,
								  FILE_SHARE_READ | FILE_SHARE_WRITE,
								  NULL,
								  OPEN_EXISTING,
								  FILE_ATTRIBUTE_NORMAL,
								  NULL);

		return disk;
	}

	long long getDiskOffset(HANDLE disk)
	{
		LARGE_INTEGER offset, zero = {0};
		SetFilePointerEx(disk, zero, &offset, FILE_CURRENT);
		return offset.QuadPart;
	}

	inline bool diskOffsetAvailablity(HANDLE disk, long long offset)
	{
		char buffer[4096] = {};
		DWORD read;
		LARGE_INTEGER offs;
		offs.QuadPart = offset;
		SetFilePointer(disk, offs.LowPart, &offs.HighPart, FILE_BEGIN);
		ReadFile(disk, buffer, sizeof(buffer), &read, NULL);

		return read != 0;
	}

	long long getDiskSize(char disk)
	{

		char str[8] = {};
		sprintf(str, "%c:\\", disk);
		DWORD64 FreeBytesToCaller = 0;
		DWORD64 TotalBytes = 0;
		DWORD64 FreeBytes = 0;

		BOOL bResult = GetDiskFreeSpaceExA(str,
										   (PULARGE_INTEGER)&FreeBytesToCaller,
										   (PULARGE_INTEGER)&TotalBytes,
										   (PULARGE_INTEGER)&FreeBytes);

		if (bResult)
		{
			return TotalBytes;
		}
		else
		{
			return -1;
		}
	}

	long long getDiskSize(HANDLE disk)
	{

		long long offset = getDiskOffset(disk);
		LARGE_INTEGER idx = {0}, size = {0}, uplimit = {0}, downlimit = {0};
		// get the upper limit of the disk size
		idx.QuadPart = 4096;

		while (true)
		{
			if (!diskOffsetAvailablity(disk, idx.QuadPart))
			{
				break;
			}
			idx.QuadPart *= 2;
			if (idx.QuadPart == 0)
			{
				break;
			}
		}

		uplimit.QuadPart = idx.QuadPart;
		downlimit.QuadPart = idx.QuadPart / 2;

		for (; uplimit.QuadPart - downlimit.QuadPart > 4096 * 2;)
		{
			idx.QuadPart = (uplimit.QuadPart + downlimit.QuadPart) / 2;
			const long long mask = ~(4096 - 1);
			idx.QuadPart &= mask;
			if (diskOffsetAvailablity(disk, idx.QuadPart))
			{
				downlimit.QuadPart = idx.QuadPart;
			}
			else
			{
				uplimit.QuadPart = idx.QuadPart;
			}
		}
		setDiskOffset(disk, offset);
		return downlimit.QuadPart;
	}

	bool setDiskOffset(HANDLE disk, long long offset)
	{
		LARGE_INTEGER offset0;
		offset0.QuadPart = offset;
		return SetFilePointerEx(disk, offset0, NULL, FILE_BEGIN);
	}

	enum diskAccessFlag:uint8_t
	{
		DISK_ACCESS_READ = 1,
		DISK_ACCESS_WRITE = 2,
		DISK_ACCESS_READWRITE = 3
	};
	enum diskSeekFlag
	{
		DISK_SEEK_BEGIN = FILE_BEGIN,
		DISK_SEEK_CURRENT = FILE_CURRENT,
		DISK_SEEK_END = FILE_END
	};

	class disk
	{
	private:
		HANDLE diskHandle;

		diskAccessFlag FlagAccess;
		void init(char diskName, diskAccessFlag flag)
		{
			char diskNameStr[16];
			sprintf(diskNameStr, "\\\\.\\%c:", diskName);
			diskHandle = CreateFileA(diskNameStr,
									 GENERIC_READ | GENERIC_WRITE | FILE_READ_ATTRIBUTES,
									 FILE_SHARE_READ | FILE_SHARE_WRITE,
									 NULL,
									 OPEN_EXISTING,
									 FILE_ATTRIBUTE_NORMAL,
									 NULL);
			if (diskHandle == INVALID_HANDLE_VALUE)
			{
				printf("disk %s open failed\n", diskNameStr);
			}
			FlagAccess = flag;
		}
		void init(int physicalDiskNumber, diskAccessFlag flag)
		{
			char diskName[32];
			sprintf(diskName, "\\\\.\\PhysicalDrive%d", physicalDiskNumber);
			diskHandle = CreateFileA(diskName,
									 GENERIC_READ | GENERIC_WRITE | FILE_READ_ATTRIBUTES,
									 FILE_SHARE_READ | FILE_SHARE_WRITE,
									 NULL,
									 OPEN_EXISTING,
									 FILE_ATTRIBUTE_NORMAL,
									 NULL);
			if (diskHandle == INVALID_HANDLE_VALUE)
			{
				printf("disk %s open failed\n", diskName);
			}
			FlagAccess = flag;
		}

	public:
		disk(diskAccessFlag flag, char diskName = 'C')
		{
			init(diskName, flag);
		}
		disk(int physicalDiskNumber, diskAccessFlag flag = DISK_ACCESS_READWRITE)
		{
			init(physicalDiskNumber, flag);
		}
		~disk()
		{
			if (diskHandle != INVALID_HANDLE_VALUE)
				CloseHandle(diskHandle);
		}

		void seek(long long offset, diskSeekFlag flag = DISK_SEEK_BEGIN)
		{
			// seek must be 4096 aligned
			const long long alignMask = ~(4096 - 1);
			offset &= alignMask;
			LARGE_INTEGER offset0;
			offset0.QuadPart = offset;
			auto ret=SetFilePointer(diskHandle, offset0.LowPart, &offset0.HighPart, flag);
			return;
		}
		long long tell()
		{
			LARGE_INTEGER offset0;
			offset0.QuadPart = 0;
			offset0.LowPart = SetFilePointer(diskHandle, offset0.LowPart, &offset0.HighPart, FILE_CURRENT);			
			return offset0.QuadPart;
		}
		long long getSize()
		{
			LARGE_INTEGER size;
			size.LowPart =
				GetFileSize(diskHandle, (LPDWORD)&size.HighPart);
			return size.QuadPart;
		}
		/// @brief 
		/// @param buffer 
		/// @param size 
		/// @return the size of data be read
		int read(void *buffer, int size)
		{
			// must has read permission
			if ((FlagAccess & DISK_ACCESS_READ) == 0)
			{
				printf("disk access error\n");
				throw("disk access error: no read permission");
				return 0;
			}
			// read must be 4096 aligned
			const long long alignMask = ~(4096 - 1);
			size &= alignMask;
			DWORD read = 0;
			ReadFile(diskHandle, buffer, size, &read, NULL);
			if (read != size)
			{
				printf("disk read error\n");
				throw("disk read error:size not match");
			}
			return read;
		}
		int write(void *buffer, int size)
		{
			// must has write permission
			if ((FlagAccess & DISK_ACCESS_WRITE) == 0)
			{
				printf("disk access error\n");
				throw("disk access error: no write permission");
				return 0;
			}
			// write must be 4096 aligned
			const long long alignMask = ~(4096 - 1);
			size &= alignMask;
			DWORD write = 0;
			WriteFile(diskHandle, buffer, size, &write, NULL);
			if (write != size)
				throw("disk write error:size not match");
			return write;
		}

		public:
		static DWORD getLogicalDrives()
		{
			DWORD drives = GetLogicalDrives();
			// print the available drives, from A:/ to Z:/
			for (int i = 0; i < 26; i++)
			{
				if (drives & 1)
				{
					std::cout << char('A' + i) << " ";
				}
				drives >>= 1;
			}
			return drives;
		}
	};

#ifdef namespace_bionukg
};
#endif // namespace_bionukg

#endif // BIONUKG_DISKDRIVE_H