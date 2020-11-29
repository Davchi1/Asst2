#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <dirent.h>
#include <libgen.h>
#include <string.h>
#include <ctype.h>

void* checkDir(void* arg);
void* checkFile(void* arg);
//Thread stuff
//--------------------------------------------------------------------------------
typedef struct threadPasser{
  char* ptPath;
  struct dirent* ptDent;
}tPasser;

//Linked list that consists of a thread and a pointer to the next node
typedef struct node{
  pthread_t id;
  char* dirName;
  struct node* next;
}node_t;
//Function to create a new thread node
node_t* create_new_node(pthread_t ids, char* directoryName){
  node_t* res = malloc(sizeof(node_t));
  res ->dirName = directoryName;

  res -> id = ids;
  res->next=NULL;
  return res;
}
//Function to join and print out all the threads created for files and directories
void printlist(node_t *head){
  node_t* pTemp = head;
  while(pTemp!=NULL){
   // printf("Directory Name: %s \n",pTemp->dirName);
  
   // printf("thread id = %ld\n",pthread_self());
    pthread_join(pTemp->id,NULL);
    pTemp=pTemp->next;
  }
}
//----------------------------------------------------------------------------------

static pthread_mutex_t lock;
static pthread_mutex_t nlock;
static pthread_mutex_t mutexDirectories = PTHREAD_MUTEX_INITIALIZER;
//Traverse all entries in the directory.... Look for other directories or files
//IF a directory we want to create a new directory thread... if it is a File we want to create a new file thread... Otherwise we ignore the entry 
void* checkDir(void* arg){
  //arg = (struct dirent*)arg;
  //printf("hello im a directory\n");
  tPasser *pPasser = (tPasser*)arg;
  DIR *pDir;
  pthread_mutex_lock(&lock);
  struct dirent *dent = pPasser->ptDent;
  char* path = pPasser->ptPath;
  printf("NAMED: %s\n",path);
  char fName[100] = "/";
  char dName[100] = "";
  strcpy(dName,dent->d_name);
  strcat(fName,dName);
  
  char* fullPath = strcat(path,fName); 
  memset(fName,0,100);
  memset(dName,0,100);
  
  printf("NAMED: %s\n",fullPath);
  pthread_mutex_unlock(&lock);
 // pDir = opendir(dent);
 //// dent = readdir(pDir);
node_t* pHead;
//node_t* pTemp;
while((dent = readdir(pDir)) != NULL){
  
  //If we find a file we want to make a new thread with the file function 
  if(dent->d_type == 4){
    //pthread_t id = (int*)malloc(sizeof(pthread_t));
   
  }
  //If we find a file we want to make a new thread with the dir functionw
  if(dent->d_type==8){
   
  }
}
//Joining the threads created for files and directories 
printlist(pHead);

  pthread_exit(NULL);
  
}
/*Tokenize every file. The program has no way of determining whether a file contains text or not, so it must assume every file contains text.*/
//
//File Linked List

//(File Name, Total Words, tokenList<>) -> (File Name, Total Words, tokenList<>) -> (File Name, Total Words, tokenList<>)

//wordTokens Linked List

//(Token name,wordFrequency) -> (Token name, wordFrequency) -> (Token name, wordFrequency)
//File will be our shared data structure
//DIR
  //./DIR/DIR2
      //DIR/DIR2/DIR3

typedef struct File{
char* fileName;
int totalWords;
struct File* next;
struct Tokens* tokenList;
}file;
//A Doubly linked list for the tokens... for alphabetical insertion

typedef struct Tokens{
char* tokenName;
float wordFrequency;
struct Tokens* next;
struct Tokens* prev;
}token;

//Head for Fie list
file* fHead;
void* checkFile(void* arg){
  //dereference void*

  tPasser *pPasser = (tPasser*)arg;
  
  struct dirent *pDent = pPasser->ptDent;
  char* path = pPasser->ptPath;
pthread_mutex_lock(&nlock);
  char fName[100] = "/";
  char dName[100] = "";
  strcpy(dName,pDent->d_name);
  strcat(fName,dName);
 // memset(fName,0,100);
  //memset(dName,0,100);
  char* fullPath = strcat(path,fName); 
  printf("File Name: %s\n",fullPath);
  pthread_mutex_unlock(&nlock);
  //When we see a file we want to create an entry in the shared data strcture - File
   printf("Checkpoint3 \n");
  pthread_mutex_lock(&lock);
 
  //This block of code makes the file name lowercase
  /*
  char fTempName[strlen(pDent->d_name)];
  strcpy(fTempName,pDent->d_name);
  for(int x = 0 ; fTempName[x];x++){
    fTempName[x] = tolower(fTempName[x]);
  }
  */
  file* fTemp = malloc(sizeof(file));
  strcpy(fullPath,fTemp->fileName);
  fTemp->next = fHead; 
  file* tempFile = fTemp;
  fHead=fTemp;
  pthread_mutex_unlock(&lock);
  //Make a token List for the file
  token* tHead = malloc(sizeof(token));
  tempFile -> tokenList = tHead;

  //FILE* fpointer = fopen(tempFile->fileName,"r");

  //After we have created the file entry we want to know perform the tokenization of the file

  //If the token is alphabetically less than the head then we want to insert it before...
  
  //if(strcmp(fTemp->fileName,fHead->fileName)<0){
    
  //}
  //Ohterwise it is greater or equal then insert it as the next token
  //else{

  //}



  

  // Dog cat horse Dog
  
  

  
pthread_exit(NULL);

}

int main(int argc, char** argv){
DIR *pDir;
struct dirent *dent;
pDir = opendir(argv[1]);


//Checks if the command line passes in a directory, otherwise exits the process.
//
if(pDir == NULL){
  printf("You have to pass in a directory in the command linee\n");
  exit(1);
}

dent = readdir(pDir);

if(dent->d_type != 4){
  printf("You have to pass in a directory in the command line\n");
  exit(1);
}

dent = readdir(pDir);
node_t* pHead;
node_t* pTemp;
while((dent = readdir(pDir)) != NULL){
  
  //If we find a file we want to make a new thread with the file function 
  if(dent->d_type == 4){
    //pthread_t id = (int*)malloc(sizeof(pthread_t));
    char startDir[100] = "./";
strcat(startDir,argv[1]);
tPasser* starter = malloc(sizeof(tPasser));
starter -> ptPath = startDir;
starter -> ptDent = dent;
    pthread_t id;
    pthread_create(&id, NULL,checkDir, (void*)starter);
    pTemp = create_new_node(id,dent->d_name);
    pTemp->next = pHead;
    pHead = pTemp;
  }
  //If we find a file we want to make a new thread with the dir functionw
  if(dent->d_type==8){
       char startDir[100] = "./";
strcat(startDir,argv[1]);
tPasser* starter = malloc(sizeof(tPasser));
starter -> ptPath = startDir;
starter -> ptDent = dent;
    pthread_t id2;
    pthread_create(&id2, NULL , checkFile,(void*)starter);
  }
}
//Joining the threads created for files and directories 
printlist(pHead);




//If so we can pass it into the dir function

//  NAME ,THREAD
// (Dir,Thread,Pointer) -> (Dir,Thread,Pointer) -> (Dir,Thread,Pointer)-> (Dir,Thread,Pointer)

// (File Name, Total Words, wordsTokens<>) -> (File Name, Total Words, wordsTokens<>) -> (File Name, Total Words, wordsTokens<>)

return 0;
}

//In order to read a file we need the directory path
//The directory path will be the directory the file is in concat with the file name
//The name of the directory of the file will be the directory concat with the directory that called its thread