/*lib.c
Michael Black, 2007
This file contains the various library functions called by the shell and 
user applications
*/

/*setup enables multitasking.  it should be called at the beginning of 
any user program*/
#include "lib.h"
void setup()
{
	seti();
}

/*computes a mod b*/
int mod(int a, int b)
{
        while(a>=b)
                a=a-b;
        return a;
}

/*computes a / b using integer mydivision*/
int mydiv(int a, int b)  
{               
        int q=0;
        while (q*b<=a)
                q++;
        return q-1;
}               

/*reads a sector into the buffer*/
void readsector(int sectornumber, char* buffer)
{
        int sec,head,cyl;

	/*convert to CHS*/
        sec=mod(sectornumber,0x12)+1;
        head=mod(mydiv(sectornumber,0x12),2);
        cyl=mydiv(sectornumber,0x24);

        readsect(buffer,sec,head,cyl);
}

/*writes buffer to a sector*/
void writesector(int sectornumber, char* buffer)
{
        int sec,head,cyl;

	/*convert to CHS*/
        sec=mod(sectornumber,0x12)+1;
        head=mod(mydiv(sectornumber,0x12),2);
        cyl=mydiv(sectornumber,0x24);

        writesect(buffer,sec,head,cyl);
}

/*prints a character*/
void putchar(char c)
{
	printc(c);
}

/*reads a character*/
char getchar()
{
	return (char)readc();
}

/*prints a string terminated with 0*/
void printstring(char* string)
{
	int21(1,string);
}

/*prints an integer*/
void printnumber(int number)
{
	char num[12];
	char pnum[12];
	int d=1;
	int i=0;
	int j;

	/*convert the number to a string digit by digit*/
	while(i<10)
	{
		num[i]=mod(mydiv(number,d),10)+0x30;
		i++;
		d=d*10;
		if (mydiv(number,d)==0)
			break;
	}

	/*reverse it to read most significant to least significant*/
	j=0;
	for (d=i-1; d>=0; d--)
		pnum[j++]=num[d];
	pnum[j]=0;
	printstring(pnum);
}

/*read a line from the keyboard and put it in buffer*/
void readstring(char* buffer)
{
	int21(2,buffer);
}

/*read the file name[] into buffer[]*/
void readfile(char* name, char* buffer)
{
	int21(3,name,buffer);
}

/*write buffer[] to a file called name[]*/
void writefile(char* name, char* buffer, int sectorlength)
{
	int21(4,name,buffer,sectorlength);
}

/*delete the file name[]*/
void deletefile(char* name)
{
	int21(5,name);
}

/*execute the program*/
void executeprogram(char* buffer, int bytelength)
{
	int21(8,buffer,bytelength);
}

/*execute the program, but don't make the caller wait*/
void executeprogrambackground(char* buffer, int bytelength)
{
	int21(6,buffer,bytelength);
}

/*terminate program - this must be called by the program before ending*/
void exit()
{
	int21(7);
}

/*sets the video to 1 - graphics, 0 - text*/
void setvideo(int mode)
{
	if (mode==0)
		setvideotext();
	else
		setvideographics();
}

/*sets the pixel at columnxrow to color*/
void setpixel(int color, int column, int row)
{
	drawpixel(color,row*320+column);
}

/*sets the cursor position to row, column*/
void setcursor(int row, int column)
{
	interrupt(0x10,2*256,0,0,row*256+column);
}

/*clear the screen*/
void clearscreen()
{
        int r,c;
        for (c=0; c<80; c++)
                for (r=0; r<25; r++)
                {
                        putInMemory(0xB800,(r*80+c)*2,0x20);
                        putInMemory(0xB800,(r*80+c)*2+1,7);
                }
}

/*prints a character at a certain cursor position*/
void setchar(char c, char color, int row, int column)
{
	putInMemory(0xB800,(row*80+column)*2,c);
	putInMemory(0xB800,(row*80+column)*2+1,color);
}

/*prints a string at a certain cursor position*/
void setstring(char* c, char color, int row, int column)
{
	int i=0;
	while(c[i]!=0x0)
	{
		putInMemory(0xB800,(row*80+column+i)*2,c[i]);
		putInMemory(0xB800,(row*80+column+i)*2+1,color);
		i++;
	}
}

void ps()
{
    int21(17);
}


/*turns an integer into a string*/
void getnumberstring(char* pnum, int number)
{
	char num[12];
	int d=1;
	int i=0;
	int j;

	/*convert the number to a string digit by digit*/
	while(i<10)
	{
		num[i]=mod(mydiv(number,d),10)+0x30;
		i++;
		d=d*10;
		if (mydiv(number,d)==0)
			break;
	}

	/*reverse it to read most significant to least significant*/
	j=0;
	for (d=i-1; d>=0; d--)
		pnum[j++]=num[d];
	pnum[j]=0;
}

void memcpy()
{

};

void *mallocFS(int sz)
{
	unsigned char our_memory[1024 * 1024];
	int next_index = 0;

	void *mem;

	if (sizeof our_memory - next_index < sz)
		return 0;
	mem = &our_memory[next_index];
	next_index += sz;
	return mem;
}

int caracterValido(char caracter){
	int codigo = (int)caracter;
	return (codigo >= 30 && codigo <= 39 || codigo >= 41 && codigo <= 90 || codigo >= 97 && codigo <= 122);
}

int mystrlen(char *cadena){
	int i = 0;

	while (cadena[i] != '\0'){
		i++;
	}
	return i;
}

char * mistrcat(char * destino, char * fuente)
{
	int i=0; 
	int j=0; 
	int leng = mystrlen(destino) + mystrlen(fuente); 
	char * temp = mallocFS(leng);
	int pos = 0;  
	while(destino[i])
		{ temp[i] = destino[i];
			 i++; 
		}
	while(fuente[j]) 
		{ 
			temp[i]=fuente[j]; 
			i++; 
			j++; 
		} 
	temp[i]='\0'; 
	
	destino = mallocFS(leng); 
	for(pos;pos<leng;pos++) 
		destino[pos] = temp[pos]; 
	return destino; 
}

char * strcpy(char *to, char* from)
{
	int i = 0;
	while (to[i] != '\0')
	{
		from[i] = to[i];
		i++;
	}
	return from;
}

int strCmp(char *s1, char *s2)
{
	unsigned char *p1 = (unsigned char *)s1;
	unsigned char *p2 = (unsigned char *)s2;

	while (*p1 != '\0') {
		if (*p2 == '\0') return  0;
		if (*p2 > *p1)   return 0;
		if (*p1 > *p2)   return  0;
		p1++;
		p2++;
	}

	if (*p2 != '\0') return 0;

	return 1;
}

int stlen(char * cadena, int start, char delim)
{
	int count, pos;
	count = 0;
	pos = 0;

	while (cadena[pos] != '\0' && cadena[pos] != delim){
		count++;
		pos++;
	}

	return count;
}

int qtytoken(char * token, char delim)
{
	int totalTokens = 1;
	int pos = 0;

	while (token[pos] != '\0')
	{
		if (token[pos] == delim){
			totalTokens++;
		}
		pos++;
	}
	return totalTokens;
}

char *mystrtok(char * str, char delim, int pos)
{
	int tokenPosition = 0;
	int totalTokens = qtytoken(str, delim), count = 1, posStr = 0;
	int len = stlen(str, posStr, delim);
	char * token = (char*)mallocFS(len);

	if (len == 1)
		return str;
	
	while (str[posStr] != '\0')
	{
		if (str[posStr] == delim)
		{
			count++;
			if (count < pos)token = (char*)mallocFS(len);
		}
		else if (count == pos){
			token[tokenPosition] = str[posStr];
			tokenPosition++;
		}
		posStr++;
	}
	return token;
}

char *getData(char *cadena)
{
	char *temp = (char*)mallocFS(mystrlen(cadena));
	int i = 0;
	for (i = 0; i < mystrlen(cadena); i++)
	{
		temp[i] = (caracterValido(cadena[i])) ? cadena[i] : '\0';
	}
	temp[i] = '\0';
	return temp;
}

char * strtok(char * str, char delim, int pos)
{
	int totalTokens = qtytoken(str, delim), count = 1, posStr = 0, tokenPosition = 0;
	int len = stlen(str, posStr, delim);
	char * token;
	if (len == 0)
		return '\0';
	token = (char*)mallocFS(len);
	while (str[posStr] != '\0')
	{
		if (str[posStr] == delim)
		{
			int len = stlen(str, posStr, delim);
			count++;
			if (count < pos)token = (char*)mallocFS(len);
		}
		else if (count == pos){
			token[tokenPosition] = str[posStr];
			tokenPosition++;
		}
		posStr++;
	}
	return token;
}

char * getPathName(char * pathName, int type)
{
	int lengthName = mystrlen(pathName);
	char *path = (char*)mallocFS(lengthName);
	int tokens = qtytoken(pathName, '/');
	int tokenPosition = 2;
	int i = 0;
	char *dirName = (char*)mallocFS(lengthName);
	if (tokens == 1 && type == 1)
	{
		path[0] = pathName[0];
		path[1] = '\0';
	}
	if (tokens == 2)
	{
		path[0] = '/';
		path = getData(path);
	}
	while (tokenPosition <= tokens - 1)
	{
		char *token = mystrtok(pathName, '/', tokenPosition);

		if (tokenPosition == 2)
			path = getData(mistrcat("/", token));
		else
			path = getData(mistrcat(path, token));

		if (tokenPosition < tokens - 1)
		{
			path = getData(path);
			path = mistrcat(path, "/");
			path = getData(path);
		}
		tokenPosition++;
	}

	if (type == 1)//retorna path sin la ultima carpeta o archivo
	{
		return path;
	}
	else
	{
		dirName = mystrtok(pathName, '/', tokens);
		dirName = getData(dirName);
		return dirName;
	}
}

int findFileInDirectory(directoryBlock directory, char * fileName)
{
	int y = 0;
	while (y < DIRECTORYENTRYCOUNT)
	{
		char *tempFileName = (char*)mallocFS(mystrlen(directory.entries[y].fileName));
		tempFileName = getData(directory.entries[y].fileName);
		if (strCmp(tempFileName, fileName) == 1 || strCmp(directory.entries[y].fileName, fileName) == 1)
			return y;
		y++;
	}
	return -1;
}


unsigned int findFileBlockInPath(char *fileName)
{
	char * path; 
	char * directoryName;
	int tokens,directoryEntryNumber;
	unsigned int directoryBlockNumber;
	directoryBlock directory;
	int tokenPosition = 2;
	fileName = getData(fileName);
	path = (char*)mallocFS(mystrlen(fileName));
	path = getData(strcpy(fileName, path));
	 tokens= qtytoken(path, '/');
	directoryName = getData(mystrtok(path, '/', tokenPosition));
	directoryEntryNumber = 0;
	directoryBlockNumber = ROOTBLOCK;
	readsector(2, &directory);
	if (directoryName[0] == '\0')
		return directoryBlockNumber;
	while (tokenPosition <= tokens)
	{
		directoryEntryNumber = findFileInDirectory(directory, directoryName);

		if (directoryEntryNumber == -1)
		{
			return -1;
		}
		directoryBlockNumber = directory.entries[directoryEntryNumber].fileBlockStart;
		readsector(directoryBlockNumber, (char *)&directory);
		directoryName = getData(mystrtok(path, '/', tokenPosition + 1));
		tokenPosition++;
	}
	return directoryBlockNumber;
}

unsigned int AllocateBlock()
{
	superBlock superBlock;
	unsigned int allocatedBlockNumber,newFirstBlockNumber;
	unusedBlock  allocatedBlock;
	allocatedBlockNumber= superBlock.firstFreeBlock;
	readsector(1, &superBlock);
	readsector(allocatedBlockNumber, &allocatedBlock);
	newFirstBlockNumber = allocatedBlock.nextFreeBlock;
	superBlock.firstFreeBlock = newFirstBlockNumber;
	superBlock.totalFreeBlocks--;
	writesector(1, &superBlock);
	return allocatedBlockNumber;
}


int mkdir(char * filePath, char * fileName)
{
	unsigned int destinationDirBlockNumber,newDirBlockNumber;
	directoryBlock destinationDirectory,newDirectory;
	int z, y, isfile;
	if (strCmp(filePath, "/") == 1)
		destinationDirBlockNumber = ROOTBLOCK;
	else
		destinationDirBlockNumber = findFileBlockInPath(filePath);
	if (destinationDirBlockNumber == -1)
	{
		printstring("Directory does not exists");
		return -1;
	}
	
	filePath = getData(filePath);
	fileName = getData(fileName);	
	readsector(destinationDirBlockNumber, &destinationDirectory);
	isfile = IsNewFile(destinationDirectory, fileName);
	if (isfile == -1)
	{
		printstring("Directory already exist\n");
		return -1;
	}
	newDirBlockNumber = AllocateBlock();
	y  = 0;
	while(y < DIRECTORYENTRYCOUNT)
	{
		newDirectory.entries[y].fileType = IS_FILE;
		newDirectory.entries[y].fileBlockStart = 0;
		newDirectory.entries[y].fileBlockSize = 0;
		y++;
	}
	writesector(newDirBlockNumber, &newDirectory);
	z = 0;
	while (z < DIRECTORYENTRYCOUNT)
	{
		if (destinationDirectory.entries[z].fileBlockStart == 0)
		{
			strcpy(fileName, destinationDirectory.entries[z].fileName);
			destinationDirectory.entries[z].fileType = IS_DIRECTORY;
			destinationDirectory.entries[z].fileBlockStart = newDirBlockNumber;
			destinationDirectory.entries[z].fileBlockSize = 1;
			break;
		}
		z++;
	}
	writesector(destinationDirBlockNumber, &destinationDirectory);
	printstring("Directory ");
	printstring(fileName);
	printstring(" Created!\n");
	return newDirBlockNumber;
}

void ls(char * filePath)
{
	unsigned int destinationDirBlockNumber; 
	int y;
	directoryBlock destinationDirectory;
	destinationDirBlockNumber = findFileBlockInPath(filePath);
	
	if (destinationDirBlockNumber == -1)
	{
		printstring("Directory does not exists");
		return;
	}
	
	readsector(destinationDirBlockNumber, &destinationDirectory);
	y = 0;
	while (y < DIRECTORYENTRYCOUNT)
	{
		if (destinationDirectory.entries[y].fileBlockStart != 0)
		{
			char *tempName = (char*)mallocFS(mystrlen(destinationDirectory.entries[y].fileName));
			tempName = getData(destinationDirectory.entries[y].fileName);
			if (destinationDirectory.entries[y].fileType == IS_DIRECTORY)
			{
				printstring("Directory: %s\n", tempName);
			}
			else if (destinationDirectory.entries[y].fileType == IS_FILE)
			{
				printstring("File: %s\n", tempName);
			}
		}
		y++;
	}
}

void rn(char *dirPath, char *newDirName)
{
	directoryBlock dir;
	int dirBlockPos,entryNumber, pathLenght,y;
	char *path,*previousDirName,*tempDirName;
	pathLenght = mystrlen(getData(dirPath)); 
	 path= (char*)mallocFS(pathLenght);
	path = getPathName(getData(dirPath), 1);
	previousDirName = getPathName(getData(dirPath), 2);
	dirBlockPos = findFileBlockInPath(path);
	if (dirBlockPos == -1)
	{
		printstring("Directory does not exists");
		return;
	}
	readsector(dirBlockPos, &dir);
	entryNumber = findFileInDirectory(dir, previousDirName);
	if (entryNumber == -1)
	{
		printstring("Directory does not exists");
		return;
	}
	tempDirName = (char*)mallocFS(mystrlen(getData(dirPath)));
	tempDirName = getData(dir.entries[entryNumber].fileName);
	if (strCmp(tempDirName, previousDirName) == 1)
	{
		y = 0;
		while( y < mystrlen(dir.entries[entryNumber].fileName))
		{
			dir.entries[entryNumber].fileName[y] = ' ';
			y++;
		}
		strcpy(dir.entries[entryNumber].fileName, getData(newDirName));
		dir.entries[entryNumber].fileType = IS_DIRECTORY;
		writesector(dirBlockPos, (char*)&dir);
	}
}

void setFreeBlock(unsigned int blockNumber)
{
	superBlock spBlock;
	unusedBlock newBlock,lastFreeBlock;
	unsigned int secondToLastBlock;
	newBlock.nextFreeBlock = 0;
	readsector(1, &spBlock);
	readsector(spBlock.lastFreeBlock, (char*)&lastFreeBlock);
	lastFreeBlock.nextFreeBlock = blockNumber;
	secondToLastBlock = spBlock.lastFreeBlock;
	spBlock.lastFreeBlock = blockNumber;
	spBlock.totalFreeBlocks++;
	writesector(FsStart, (char*)&spBlock);
	writesector(secondToLastBlock, (char*)&lastFreeBlock);
	writesector(blockNumber, (char*)&newBlock);
}

void cpyDir(int sourceBlock, int destinyBlock, char *destinyPath)
{
	directoryBlock source, destiny;
	int y, z, dirStart, dirSize, newBlockDir;
	char *dirName;
	readsector(sourceBlock, (char *)&source);
	readsector(destinyBlock, (char *)&destiny);
	if (sourceBlock == 0)
	{
		return;
	}
	y = 0;
	while (y < DIRECTORYENTRYCOUNT)
	{
		if (source.entries[y].fileType == IS_FILE)
		{
			dirStart = source.entries[y].fileBlockStart;
			dirSize = source.entries[y].fileBlockSize;
			if (y == DIRECTORYENTRYCOUNT - 1)
				return;
		}
		else
		{
			newBlockDir = mkdir(destinyPath, source.entries[y].fileName);
			dirName = source.entries[y].fileName;
			z=0;
			while (z < DIRECTORYENTRYCOUNT)
			{
				if (destiny.entries[z].fileBlockStart == 0)
				{
					destiny.entries[z].fileName[0] = '\0';
					strcpy(dirName, destiny.entries[z].fileName);
					destiny.entries[z].fileType = IS_DIRECTORY;
					destiny.entries[z].fileBlockStart = newBlockDir;
					destiny.entries[z].fileBlockSize = 1;
					break;
				}
				z++;
			}
			writesector(destinyBlock, (char *)&destiny);
			cpyDir(source.entries[y].fileBlockStart, newBlockDir, dirName);
		}
		y++;
	}
}


void cpy(char *pathSource, char *destinyPath, char *copyName)
{
	directoryBlock source, destiny;
	int sourceBlock, destinyBlock, newBlockDir, z;
	char *sourcePathName,*dirName;
	sourceBlock = findFileBlockInPath(getData(pathSource));
	destinyBlock = findFileBlockInPath(getData(destinyPath));
	if (sourceBlock == 0 || destinyBlock == 0)
	{
		return;
	}
	sourcePathName = getPathName(getData(pathSource), 2);
	readsector(sourceBlock, (char *)&source);
	readsector(destinyBlock, (char *)&destiny);
	{
		newBlockDir = mkdir(getData(destinyPath), getData(copyName));
		if (strCmp(getData(getData(destinyPath)), "/") != 1)
		{
			dirName = mistrcat(getData(destinyPath), "/");
			dirName = mistrcat(getData(dirName), getData(copyName));
			dirName = getData(dirName);
		}
		else	
			dirName = getData(getData(destinyPath));
		z = 0;
		while(z < DIRECTORYENTRYCOUNT)
		{
			if (destiny.entries[z].fileBlockStart == 0)
			{
				destiny.entries[z].fileName[0] = '\0';
				strcpy(getData(copyName), destiny.entries[z].fileName);
				destiny.entries[z].fileType = IS_DIRECTORY;
				destiny.entries[z].fileBlockStart = newBlockDir;
				destiny.entries[z].fileBlockSize = 1;
				break;
			}
			z++;
		}
		writesector(destinyBlock, (char *)&destiny);
		cpyDir(sourceBlock, newBlockDir, dirName);
	}
}

void rmDir(int DirBlock)
{
	directoryBlock dir;
	int dirStart, dirSize, y, z;
	readsector(DirBlock, (char*)&dir);
	if (DirBlock == 0)
	{
		return;
	}
	y = 0;
	while (y < DIRECTORYENTRYCOUNT)
	{
		if (dir.entries[y].fileType == IS_FILE)
		{
			dirStart = dir.entries[y].fileBlockStart;
			dirSize = dir.entries[y].fileBlockSize;
			z = dirStart;
			while( z < dirSize)
			{
				setFreeBlock(z);
				z++;
			}
			if (y == DIRECTORYENTRYCOUNT - 1)
				return;
		}
		else if (dir.entries[y].fileType == IS_DIRECTORY)
		{
			rmDir(dir.entries[y].fileBlockStart);
			setFreeBlock(y);
		}
		y++;
	}
}


void rm(char * path)
{
	int y, z, dirStart, dirSize,blockEntryToRemove,blockToRemove;
	directoryBlock erasedBlock;
	char *destinyBlock,*dirRm;
	destinyBlock = getPathName(path, 1);
	dirRm  = getPathName(path, 2); 
	blockToRemove = findFileBlockInPath(destinyBlock);
	readsector(blockToRemove, (char*)&erasedBlock);
	blockEntryToRemove = findFileInDirectory(erasedBlock, dirRm);
	if (blockEntryToRemove == -1)
	{
		printstring("Directory does not exist\n");
		return;
	}
	if (erasedBlock.entries[blockEntryToRemove].fileType == IS_FILE)
	{
		dirStart = erasedBlock.entries[blockEntryToRemove].fileBlockStart;
		dirSize = erasedBlock.entries[blockEntryToRemove].fileBlockSize;
		z = dirStart;
		while (z < dirSize)
		{
			setFreeBlock(z);
			z++;
		}
	}
	else if (erasedBlock.entries[blockEntryToRemove].fileType == IS_DIRECTORY)
	{
		rmDir(erasedBlock.entries[blockEntryToRemove].fileBlockStart);
		setFreeBlock(erasedBlock.entries[blockEntryToRemove].fileBlockStart);
	}
	y = 0;
	while (y < sizeof(erasedBlock.entries[blockEntryToRemove].fileName))
	{
		erasedBlock.entries[blockEntryToRemove].fileName[y] = '\0';
	}
	erasedBlock.entries[blockEntryToRemove].fileBlockSize = 0;
	erasedBlock.entries[blockEntryToRemove].fileBlockStart = 0;
	writesector(blockToRemove, (char*)&erasedBlock);
}

void mv(char *pathSource, char *destinyPath)
{
	char *copyName =  (char*)mallocFS(mystrlen(getData(pathSource)));
	copyName = getData(getPathName(getData(pathSource), 2));
	cpy(pathSource, destinyPath, copyName);
	rm(pathSource);
}

void wf(fileBlock *file, char Data[], int blockStart)
{
	int module,blocksToWrite,pos,y,z,x;
	char buffer[512];
	blocksToWrite = mystrlen(Data) / BlockSize;
	module = mod(blocksToWrite, BlockSize);
	pos = 481;
	if (module != 0)
	{
		blocksToWrite = blocksToWrite + 1;
	}
	if (blocksToWrite == 0)
	{
		blocksToWrite = 1;
	}
	file->size = blocksToWrite;
	y = 0;
	while (y < blocksToWrite)
	{
		if (y!=0)
		{
			z = pos;
			while (z < pos + 512)
			{
				buffer[x] = Data[z];
				z++;
				x++;
			}
			writesector(blockStart, buffer);
			blockStart++;
			pos = pos + 512;
		}
		else
		{
			writesector(blockStart, (char *)&file);
			blockStart++;
		}
		y++;
	}
}

int IsNewFile(directoryBlock dirBlock, char *fileName)
{
	int y = 0;
	while (y < DIRECTORYENTRYCOUNT)
	{
		if (strCmp(getData(dirBlock.entries[y].fileName), fileName) == 1)
		{
			return -1;
		}
		y++;
	}
	return 1;
}

void mkf(char * path, char* fileName, char acess[], char *data)
{
	int y,isfile;
	directoryBlock destinationDirectory;
	fileBlock file;
	unsigned int newDirBlockNumber, destinationDirBlockNumber;
	
	if (strCmp(getData(path), "/") != 1)
	{
		destinationDirBlockNumber = findFileBlockInPath(getData(path));
	}
	else
	{
		destinationDirBlockNumber = ROOTBLOCK;
	}
	if (destinationDirBlockNumber == -1)
	{
		printstring("Path cannot be found, creating file failed");
		return;
	}
	readsector(destinationDirBlockNumber, (char*)&destinationDirectory);
	isfile = IsNewFile(destinationDirectory, getData(fileName));
	if (isfile == -1)
	{
		printstring("File already exist\n");
		return;
	}
	file.size = 0;
	file.type = IS_FILE;
	file.data[0] = '\0';
	strcpy(getData(fileName), file.fileName);
	strcpy(getData(acess), file.acessRights);
	strcpy(getData(data), file.data);
	newDirBlockNumber = AllocateBlock();
	wf(&file, getData(data), newDirBlockNumber);
	y = 0;
	while( y < DIRECTORYENTRYCOUNT)
	{
		if (destinationDirectory.entries[y].fileBlockStart == 0)
		{
			strcpy(destinationDirectory.entries[y].fileName, file.fileName);
			destinationDirectory.entries[y].fileType = IS_FILE;
			destinationDirectory.entries[y].fileBlockStart = newDirBlockNumber;
			destinationDirectory.entries[y].fileBlockSize = file.size;
			break;
		}
		y++;
	}
	writesector(destinationDirBlockNumber, (char*)&destinationDirectory);
	return;
}

void rmf(char *path, char *fileName)
{
	unsigned int z, blockToRemove;
	int fileStart,fileSize,y;
	directoryBlock destinyBlock;
	unsigned int blockPathNumber; 
	blockPathNumber = findFileBlockInPath(getData(path));
	path = getData(path);
	if (blockPathNumber == -1)
	{
		printstring("Directory does not exist");
		return;
	}
	readsector(blockPathNumber, (char*)&destinyBlock);
	blockToRemove = findFileInDirectory(destinyBlock, getData(fileName));
	if (blockToRemove == -1)
	{
		printstring("File does not exist");
		return;
	}
	if (destinyBlock.entries[blockToRemove].fileType == IS_FILE)
	{
		fileStart = destinyBlock.entries[blockToRemove].fileBlockStart;
		fileSize = destinyBlock.entries[blockToRemove].fileBlockSize;
		while( z < fileSize)
		{
			setFreeBlock(fileStart);
			fileStart++;
			z++;
		}
		y= 0;
		while (y < sizeof(destinyBlock.entries[blockToRemove].fileName))
		{
			destinyBlock.entries[blockToRemove].fileName[y] = '\0';
			y++;
		}
		destinyBlock.entries[blockToRemove].fileBlockSize = 0;
		destinyBlock.entries[blockToRemove].fileBlockStart = 0;
		writesector(blockPathNumber, (char*)&destinyBlock);
	}
}

void lf(char *path, char *fileName)
{
	unsigned int z, fileBlockNumber, fileStart, fileSize;
	char *buffer;
	directoryBlock destinyBlock;
	fileBlock file; 
	unsigned int blockPathNumber = findFileBlockInPath(getData(path));
	readsector(blockPathNumber, (char*)&destinyBlock);
	fileBlockNumber = findFileInDirectory(destinyBlock, getData(fileName));
	z = 0;
	if (blockPathNumber == -1)
	{
		printstring("File %s does not exists", getData(fileName));
	}
	if (destinyBlock.entries[fileBlockNumber].fileType == IS_FILE)
	{
		fileStart = destinyBlock.entries[fileBlockNumber].fileBlockStart;
		fileSize = destinyBlock.entries[fileBlockNumber].fileBlockSize;
		while (z < fileSize)
		{
			if (z != 0)
			{
				readsector(fileStart, buffer);
				printstring(buffer);
			}
			else
			{
				readsector(fileStart, (char *)&file);
				printstring("File Name: ");
				printstring(getData(file.fileName));
				printstring(" \nFile Type: ");
				printstring("%d", file.type);
				printstring(" \nSize: ");
				printstring("%d", file.size);
				printstring(" \nAccess: ");
				printstring(getData(file.acessRights));
				printstring(" \nData: ");
				printstring(getData(file.data));
			}
			fileStart++;
			z++;
		}
	}
	else
		printstring("File %s does not exists", getData(fileName));
}


