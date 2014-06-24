
#include <stdio.h>

#define SuperBlockPos 34
#define RootPos 35

#define BlockSize 512
#define FsStart 34
#define magic 1837083 //valor decimal de 'fs'
#define totalBlocksDeviceSize 2880
#define directoryCount 16

typedef struct
{ 
	unsigned int magicNumber;
	unsigned int totalBlocks;
	unsigned int totalFreeBlocks; 
	unsigned int firstFreeBlock; 
	unsigned int lastFreeBlock; 
	char wastedSpace[492]; 
}superBlock; 

typedef struct
{ 
	char fileName[20]; 
	unsigned int fileType; 
	unsigned int fileBlockStart; 
	unsigned int fileBlockSize; 
} directoryEntry; 

typedef struct 
{ 
	directoryEntry entries[directoryCount]; 
}directoryBlock;

 typedef struct 
{ 
	unsigned int nextFreeBlock; 
	char wastedSpace[508]; 
}unusedBlock;

directoryBlock createEmptyDirectory() 
{ 
  directoryBlock newDirectory; 
  int i = 0; 
  while(i<directoryCount) 
  { 
	newDirectory.entries[i].fileBlockStart = 0; 
	newDirectory.entries[i].fileBlockSize = 0; i++;
  } 
return newDirectory; 
}

void writeBlock(FILE *deviceFile, int blockNumber, void * blockData) 
{ 
	fseek(deviceFile,blockNumber*BlockSize,SEEK_SET);
	fwrite(blockData,1,BlockSize,deviceFile); 
}


void format(char * device)
{
	FILE* floppy;
	floppy=fopen(device,"rw+");
	if (floppy==0)
	{
		printf("floppya.img not found\n");
		return;
	}
	fseek(floppy,0,SEEK_END); 
	unsigned int totalBlocks = ftell(floppy)/BlockSize;


	fseek(floppy,BlockSize*FsStart,SEEK_SET);
	unsigned int Fsbeginning = ftell(floppy)/BlockSize;

	unsigned int fsBlocksSize= totalBlocks - Fsbeginning;
	unsigned int OsIndex = Fsbeginning;


	superBlock fsBlock;
	fsBlock.magicNumber = magic;
	fsBlock.totalBlocks = fsBlocksSize;
	fsBlock.totalFreeBlocks = fsBlocksSize-2;
	fsBlock.firstFreeBlock = OsIndex + 2;
	fsBlock.lastFreeBlock = totalBlocks - 1;
	fsBlock.wastedSpace[0] = 's';
	fsBlock.wastedSpace[1] ='u';
	fsBlock.wastedSpace[2] ='p';
	fsBlock.wastedSpace[3] ='e';
	fsBlock.wastedSpace[4] ='r';
	fsBlock.wastedSpace[5] ='b';
	fsBlock.wastedSpace[6] ='l';
	fsBlock.wastedSpace[7] ='o';
	fsBlock.wastedSpace[8] ='k';
	int blockNumber;
	int last = totalBlocks -1;

	for(blockNumber = fsBlock.firstFreeBlock;blockNumber<last;blockNumber++)
	{ 
	 	unusedBlock freeBlock; 
	 	freeBlock.nextFreeBlock = blockNumber +1; 
	 	writeBlock(floppy,blockNumber,&freeBlock);
	} 
	unusedBlock lastFreeBlock; 
	lastFreeBlock.nextFreeBlock=0; 
	writeBlock(floppy, fsBlock.lastFreeBlock,&lastFreeBlock);

	directoryBlock rootDirectory =createEmptyDirectory();
	writeBlock(floppy, RootPos, &rootDirectory);
	writeBlock(floppy, SuperBlockPos, &fsBlock);
	char* buffer;
	fclose(floppy);
}

main()
{
	format("floppya.img");
}


