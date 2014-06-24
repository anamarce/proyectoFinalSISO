void readsector(int, char*);
void writesector(int, char*);
void putchar(char);
char getchar();
void printstring(char*);
void printnumber(int);
void readstring(char*);
void readfile(char*, char*);
void writefile(char*, char*, int);
void deletefile(char*);
void exit();
void executeprogram(char*, int,char*);
void allow_preemption();
int mod(int,int);
int mydiv(int,int);
void setvideo(int);
void setpixel(int,int,int);
void clearscreen();
void setcursor(int,int);
void setchar(char,char,int,int);
void setstring(char*,char,int,int);
void getnumberstring(char*,int);

//funciones del FS
void ps();

void *mallocFS(int sz);
int mod(int a, int b);
char *strcpy(char* , char*);
int strCmp (char * ,char *);
int stlen(char * cadena, int start, char delim);
int qtytoken(char * token, char delim);
char * strtok(char * str, char delim, int pos);
int mystrlen(char *cadena);
char * mistrcat(char * destino,char * fuente);
int caracterValido(char caracter);
char *mystrtok(char * str, char delim, int pos);
char *getData(char *cadena);
char * getPathName(char * pathName, int type);

#define BlockSize 512
#define FsStart 34
#define magic 1837083 //valor decimal de 'fs'
#define totalBlocksDeviceSize 2880
#define totalBlocksFS 34
#define DIRECTORYENTRYCOUNT 16
#define IS_DIRECTORY 1
#define IS_FILE 2
#define ROOTBLOCK 35
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
	directoryEntry entries[DIRECTORYENTRYCOUNT]; 
}directoryBlock;

 typedef struct 
{ 
	unsigned int nextFreeBlock; 
	char wastedSpace[508]; 
}unusedBlock;

typedef struct 
{
	char fileName[20];
	unsigned int type;
	unsigned int size;//
	char acessRights[3];//xwr
	char data [481];
}fileBlock;

	int findFileInDirectory(directoryBlock directory,char * fileName);
	unsigned int findFileBlockInPath(char * fileName);
	unsigned int  GetFreeSpace();
	unsigned int AllocateBlock();
	int mkdir(char * filePath,char * fileName);
	void ls(char * filePath);
	void mkdirFS(char* name);
	void rn(char *dirPath, char *newDirName);
	void setFreeBlock(unsigned int blockNumber);
	void rmDir(int block);
	void rm(char * path);
	void cpyDir(int blockSource, int blockDestino, char *pathDestino);
	void cpy(char *pathSource, char *pathDestino,char *nameOfCopy);
	void mv(char *pathSource, char *pathDestino);
	void lf(char *path, char *fileName);
	int IsNewFile(directoryBlock dirBlock, char *fileName);
	void wf(fileBlock *file, char fileData[], int blockStart);
	void rmf(char *path, char *fileName);
	void mkf(char * path, char* fileName, char acess[], char *data);
	