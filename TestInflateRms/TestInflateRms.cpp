#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#define ZLIB_WINAPI
#include "zlib.h"


#define ZLIB_BUFFER_SIZE  (4 * 1024)
#define ZLIB_DRM_HEADER_MAGIC (4000)

typedef struct
{
	ULONG ulCheck;
	ULONG cbUncompressed;
	ULONG cbCompressed;
} ZLIBDRMHEADER;

int main(int argc, char *argv[])
{

    int BlockSizeUncompress=0x1000;
    int cprLevel=Z_BEST_COMPRESSION;
    long lCompressedSize=0;
    long lSizeUncpr;
    DWORD dwGetTick,dwMsecQP;
    LARGE_INTEGER li_qp,li_rdtsc,dwResRdtsc;
    bool	fCompress = false;
    FILE* streamIn = NULL;
    FILE* streamOut = NULL;

    enum
    {
          paramExe,
          paramOperation,
          paramFileIn,
          paramFileOut,
          paramBlockSizeUncompress,
          paramCompressLevel
    };

    if (argc<paramFileOut)
    {
        printf("run TestZlib compress <FileIn> <FileOut> [BlockSizeUncompress] [compres. level]\n"
			"    TestZlib uncompress <FileIn> <FileOut>\n"
			"For example: TestZlib uncompress message.rpmsg uncompressed.data");
        return 0;
    }

	if (argv[paramOperation][0] == 'u')
		fCompress = false;
	else if (argv[paramOperation][0] == 'c')
		fCompress = true;
	else
	{
		return -1;
	}

    if (argc>paramBlockSizeUncompress)
        BlockSizeUncompress=atol(argv[paramBlockSizeUncompress]);

    if (argc>paramCompressLevel)
        cprLevel=(int)atol(argv[paramCompressLevel]);

	if (fCompress)
	{

                        // Do compression logic here.

	}
	else
	{
		static const char c_szCompressedDrmMessageHeader[] = "\x76\xE8\x04\x60\xC4\x11\xE3\x86";
		CHAR szHeader[sizeof(c_szCompressedDrmMessageHeader)] = {0};
		byte          rgbOriginal  [ZLIB_BUFFER_SIZE];
		byte          rgbCompressed[ZLIB_BUFFER_SIZE*2];
		streamIn=fopen(argv[paramFileIn], "rb");

		if (streamIn==NULL)
			return 0;

		if (fread(szHeader, 1, sizeof(szHeader)-1, streamIn) == 0)
			return 0;

		if (strcmp(szHeader, c_szCompressedDrmMessageHeader) != 0)
		{
			printf("Not a DRM message header");
			return -1;
		}

		streamOut=fopen(argv[paramFileOut], "wb");
		if (streamOut==NULL)
			return 0;

		{
			z_stream zcpr;
			ZLIBDRMHEADER theHeader = {0};
			int ret=Z_OK;
			long lOrigDone = 0;
			int step=0;
			memset(&zcpr,0,sizeof(z_stream));
			inflateInit(&zcpr);

			printf("%lu\n", theHeader.ulCheck);
			printf("%lu\n", theHeader.cbUncompressed);
			printf("%lu\n", theHeader.cbCompressed);

			while (step < 2048)
			{
				ZeroMemory(rgbOriginal, sizeof(rgbOriginal));
				if (fread(&theHeader, sizeof(theHeader), 1, streamIn) == 0)
				{
					printf("read failed - A ");
					printf("%lu\n", theHeader.ulCheck);
					printf("%lu\n", theHeader.cbUncompressed);
					printf("%lu\n", theHeader.cbCompressed);
					//break;
				}

				if (theHeader.ulCheck != ZLIB_DRM_HEADER_MAGIC)
				{
					printf("Header Check failed!");
					return -3;
				}

				if (fread(rgbCompressed, theHeader.cbCompressed, 1, streamIn) == 0)
				{
					printf("read failed - B");
					//return -4;
				}
				
				zcpr.next_in = rgbCompressed;
				zcpr.next_out = rgbOriginal;
				zcpr.avail_in = theHeader.cbCompressed;
				zcpr.avail_out = sizeof(rgbOriginal);

				do
				{
					ret=inflate(&zcpr,Z_SYNC_FLUSH);
					step++;
					printf("STEP: %lu \n", step);
				} while (ret==Z_OK);

				fwrite(rgbOriginal, 1, theHeader.cbUncompressed, streamOut);

				
			}

				lSizeUncpr=zcpr.total_out;
				inflateEnd(&zcpr);
		}
	}

	fclose(streamIn);
	fclose(streamOut);

    return 0;
}