/**
*@file pagerank.c
* @brief A program that implements the algorythm PageRank
*
* This program includes many operations that implement the general 
* operation of a pageRank via an algorythm.It uses arrays as matrixes and sets
* in order to connect the hosts with their links and general information about
* the pagerankss of each host ,the the number of links of each  host and
* other things.
*
*  @version 1.0
*  @author Valentinos Pariza
*  @bug No know bugs.
*
*/



#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
/*Th constants and macros*/
#define SIZE_OF_WEB_HOSTS 10
#define NULL_POINTER_ERROR -1
#define FILE_ERROR -2
#define MEMORY_ALLOCATION_ERROR -3
#define NEGATIVE_SIZE_ERROR 2
#define MAX_CHARS_PER_LINE 100
#define MAX_CHARS_FOR_NAME 40
#define NOT_VALID_INFORMATION_FOR_HOSTS 1
#define D 0.85
#define FRACTION (1.0/10.0)*(1.0/10.0)


// Function Prototypes

float calculatePageRank(float* pageRanks, int** links,int* 
numberOfWebServers,int size,int hostID);

int readHostgraph(char* fileName,int*** links,int* size);

int readHostNames(char* fileName,char*** hostNames,int* size);

void printStructureOij( int** links,int size);

void printStructureOj( int* sumHyperlinksFromHosts,int size);

void printStructureIij( int** links,int size);

void calculatePageRanksForHosts(float* pageRanks,float* previousPageRanks,
 int** links,int* numberOfWebServers,int size);

void calculateSumsHyperlinksFromHost( int** links,int* 
sumHyperlinksFromHosts,int size);

float euklideiaDistance(float* const pageRanks,float* const previousPageRanks,
int size);

float calculateNormPageRank(float* const pageRanks,int hostID,int size);

void printPageRanks( char** hostNames,float* pageRanks,int size);

int modifySize(int*** a,int size);

int checkForValidityOfHostNames(char** const hostNames,int size);

int checkForValidityOfLinskOfHosts(int** const links,int size);




/**
*@brief Checks for validity of the hosts' names that were read from 
*       a file.
*
*This method checks for validity of the hosts' names that were read from 
* a file.Valid host names in general are the host names that have ids that
* all together can be placed in an array with a consecutive order 
* based on their id(For example 0,1,2,3..n is valid but hosts that are 
* 0,1,4,6,..n are not because the ids 2 and 3 do no exist).For this program we
* suppose that the file should give (in every order) only ids that as numbers
* can be placed in an ascending order where each of them differs from previous
* and next (if they exist)by one .
*
*@param hostNames a pointer to a place in memory where a pointer(char**) of a 
*       a sequence of char* pointers exists.These char* pointers are pointers
* 
*@param  size the number of the hosts
*
*@return 0 if the host names as a unit are valid(for their ids.To be 
* consecutive)
*
*/
int checkForValidityOfHostNames(char** const hostNames,int size)
{

   // for this purpose we check if there is any element which is NULL
   // if is NULL then it means that for that ith element-pointer that is
   // NULL we can not accept that format of sequence of pointers,because
   // the ith id does not exist
   for(int i=0;i<size;i++)
      if(hostNames[i]==NULL)
        return NOT_VALID_INFORMATION_FOR_HOSTS;

   return 0;
}


/**
*@brief Checks for validity of the hosts' links that were read from 
*       a file.
*
*This method checks for validity of the hosts' names that were read from 
* a file.Valid host names in general are the host names that have ids that
* all together can be placed in an array with a consecutive order 
* based on their id(For example 0,1,2,3..n is valid but hosts that are 
* 0,1,4,6,..n are not because the ids 2 and 3 do no exist).For this program we
* suppose that the file should give (in every order) only ids that as numbers
* can be placed in an ascending order where each of them differs from previous
* and next (if they exist)by one .
*
*@param hostNames a pointer to a place in memory where a pointer(char**) of a 
*       a sequence of char* pointers exists.These char* pointers are pointers
* 
*@param  size the number of the hosts
*
*@return 0 if the host names as a unit are valid(for their ids.To be 
* consecutive)
*
*/
int checkForValidityOfLinskOfHosts(int** const links,int size)
{

   // for this purpose we check if there is any element which is NULL
   // if is NULL then it means that for that ith element-pointer that is
   // NULL we can not accept that format of sequence of pointers,because
   // the ith id does not exist
   for(int i=0;i<size;i++)
      if(links[i]==NULL)
        return NOT_VALID_INFORMATION_FOR_HOSTS;

   return 0;
}



/**
* 
* @brief This method modifies the size of a dynamic sequence of sequences of 
*        integers 
*
*  This method takes a pointer to variable type of of int** and modifies the 
*  sequences,that the pointer points to,so that the sequences have the size 
*  that indicated by the parameter size.
*
* @param a A pointer that points to a place in memory ,which that place points
* to a place in memory where a sequence of pointers to sequences of integers 
*  exists
*
* @param size The size of the sequence (which is the number of the hosts)
*
* @return  0 if the method was excecuted correctly,NULL_POINTER_ERROR if 
*         a    was NULL,NEGATIVE_SIZE_ERROR if the size is a negative number
*         or MEMORY_ALLOCATION_ERROR for problems in allocating dynamically 
*         memory from heap.
*
*/
int modifySize(int*** a,int size)
{
   if(a==NULL)
      return NULL_POINTER_ERROR;

      if(size<0)
         return NEGATIVE_SIZE_ERROR;

    { // local subroutine for reallocating the sequence of int* pointers
      // and checking whether is created or not
   
      int** temp=(int**)realloc(*a,sizeof(int*)*size);

     if(temp==NULL)
        return MEMORY_ALLOCATION_ERROR;

   }

     int* temp=NULL;
   for(int i=0;i<size;i++)
    {
   
      // reallocating the size of the sequence of integers numbers for each 
      // int* pointer and checking whether is created or not
        if((*a)[i]!=NULL)
         {

            temp=(int*)realloc((*a)[i],sizeof(int)*size);
      
             if(temp==NULL)
                return MEMORY_ALLOCATION_ERROR; 
      
          
             (*a)[i]=temp; 
         }
              

    }

     return 0;

}


/**
*@brief This method reads the hostogrph from a file
*
* This method reads the hosts from a file and their links to other hosts
* and places these information and the informations read from the file in a
* two dimensional array.
* 
* @param fileName a pointer to a sequence of characters which indicate the name
*         of the file
*
*@param links A pointer to a sequence of pointers which each pointer
*points to a sequence of integers.This data structure represents the number of 
*links of each host.
*
* @return  0 if the method was excecuted correctly, @see NULL_POINTER_ERROR if 
*        one of the parameters is NULL, @see FILE_ERROR if the file could not 
*        be opened or found for reading or @see MEMORY_ALLOCATION_ERROR for
*        problems in allocating dynamically memory from heap.
*
*/
int readHostgraph(char* fileName,int*** links,int* size)
{

   if(fileName==NULL)
      return NULL_POINTER_ERROR;

   if(links==NULL)
      return NULL_POINTER_ERROR;

   FILE* filePointer=NULL;
   filePointer=fopen(fileName,"r");

   if(filePointer==NULL)
     return FILE_ERROR;

   // each line of the file that will be read,it will be placed (in each 
   // iteration) in this array size of MAX_CHARS_PER_LINE
   char line[MAX_CHARS_PER_LINE];
   
   int src=0;   

   // the initial number of permitted web hosts
   int maxNumberOfHyperlinks=SIZE_OF_WEB_HOSTS;

   // a temporary two dimensional array-sequence of integer(using pointer
   // to pointers of integers) 
   int** someHyperlinks=(int**)malloc(sizeof(int*)*maxNumberOfHyperlinks);   

   if(someHyperlinks==NULL)
      return MEMORY_ALLOCATION_ERROR;
   
   int status=0;
   
   while(fscanf(filePointer,"%d",&src)!=EOF)
   {
         // if the number of hosts is greater than maxNumberOfHyperlinks
         // or the host that has been read is a  host that has id
         // greater than the maxNumberOfHyperlinks,which forces the program to
         // allocate more space in order to include the id of that host which 
         // is greater than maxNumberOfHyperlinks
        while((*size)>=maxNumberOfHyperlinks || src>=maxNumberOfHyperlinks) 
         {

         // check if there is a host id that is greater than the current
         // number of hosts
         if(src>maxNumberOfHyperlinks)
            *size=src;   
         // in the *size would be the new number of different hosts that
         // exist-1.But later the *size will be incremented so *size will
         // have the size that the array should have

             status=modifySize(&someHyperlinks,maxNumberOfHyperlinks*=2);
      
            if(status!=0)
               return status;
      
         }              
            

      someHyperlinks[src]=(int*)calloc(maxNumberOfHyperlinks,sizeof(int));

      if(someHyperlinks[src]==NULL)
         return MEMORY_ALLOCATION_ERROR;

         
      fscanf(filePointer,"%s",line); // ignore the symbol ->


         // read the rest line that remains
       if(fgets(line,sizeof(line)/sizeof(char),filePointer)!=NULL)
       {
               // use some escape characters that might appear as delimeters
            char* token=strtok(line," \n\t\f\b\r");
            
            int dest=0;
            int numLinks=0;         

   
            // tokenize the line to the tokens that follow the predefined
            // format
            while(token!=NULL)
            {
   
              sscanf(token,"%d:%d",&dest,&numLinks); 

             someHyperlinks[src][dest]=numLinks;
         
              token=strtok(NULL," \n");     
   
            }

       }
   
      
      (*size)++;
   }

   fclose(filePointer);

   // shrink the to the number of hosts the two dimensionl dynamic array
   status=modifySize(&someHyperlinks,*size);

   if(status!=0)
      return status;

   // update the content of the set of memory cells with the array of 
   // links in the memory
   *links=someHyperlinks;
      
   return checkForValidityOfLinskOfHosts(*links,*size);
}



/**
* @brief This method reads the names of hosts from a file
*
* This method reads the names of hosts from a file which is indicated by the 
*  name fileName, updates the number of hosts in a variable and returns whether
* the operation has been completed succesfully or not.
* 
* @param fileName a pointer to a sequence of characters which indicates the name
*         of the file to be opened for reading
*
*@param hostNames a pointer to a place in memory where a pointer(char**) of a 
*       a sequence of char* pointers exists.These char* pointers are pointers
*         to the characters of the names of the hosts.
*
*@param size a pointer to a set of memory cells where the number of the web
*        hosts is stored
*
* @return  0 if the method was excecuted correctly, @see NULL_POINTER_ERROR if 
*        one of the parameters is NULL, @see FILE_ERROR if the file could not 
*        be opened or found for reading or @see MEMORY_ALLOCATION_ERROR for
*        problems in allocating dynamically memory from heap.
*
*/
int readHostNames(char* fileName,char*** hostNames,int* size)
{

   if(hostNames==NULL)
      return NULL_POINTER_ERROR;

   if(fileName==NULL || size==NULL)
      return NULL_POINTER_ERROR;

   FILE* filePointer=NULL;
   filePointer=fopen(fileName,"r");
      
   if(fileName==NULL)
      return FILE_ERROR;

   char** names=NULL;

   names=(char**)malloc(SIZE_OF_WEB_HOSTS*sizeof(char*));


    if(names==NULL)
      return MEMORY_ALLOCATION_ERROR;

   int sizeOfList=SIZE_OF_WEB_HOSTS;
   int host=0;

   *size=0;

   char** temp=NULL;
   char* name=NULL;

   while(fscanf(filePointer,"%d",&host)!=EOF)
   {   
   
      // while the number of hosts read is less than the defined size of
      // two dimensional dynamic array-sequence and the id of the host
      // read
     while((*size)>=sizeOfList || host>=sizeOfList) 
       {
   
         // check if there is a host id that is greater than the current
         // number of hosts
         if(host>sizeOfList)
            *size=host;
         // in the *size would be the new number of different hosts that
         // exist-1.But later the *size will be incremented so *size will
         // have the size that the array should have

            sizeOfList*=2;
             temp=(char**)realloc(names,sizeOfList*sizeof(char*));

            if(temp==NULL)
               return MEMORY_ALLOCATION_ERROR;
            else
               names=temp;
       }  
   
       name=(char*)malloc(MAX_CHARS_FOR_NAME*sizeof(char));
   
      if(name==NULL)
         return MEMORY_ALLOCATION_ERROR;

      fscanf(filePointer,"%s",name);
      names[host]=name;
      (*size)++;
        
   }
      
   fclose(filePointer);

      // shrink the to the number of hosts the two dimensionl dynamic array
      if(sizeOfList!=*size)
        {
         
         temp=(char**)realloc(names,(*size)*sizeof(char*));

          if(temp==NULL)
              return MEMORY_ALLOCATION_ERROR;
            
            names=temp;
         }
          
         *hostNames=names;
   

      return checkForValidityOfHostNames(*hostNames,*size);
}


/**
*
*@brief Prints the data structure Oij 
*
* This method prints the structure 0ij of the hosts.More simplier it prints the
* id of the hosts and the number of links that link from that host at each host 
* in a formatted way.
*
*
*@param links A pointer to a sequence of pointers which each pointer
*points to a sequence of integers.This data structure represents the number of 
*links of each host.
*
*
*@param  size the number of the hosts
*
*@return void
*
*/
void printStructureOij( int** links,int size)
{
      printf("+++ Start of printing structure Oij +++\n"); 
      printf("The structure of each line is:\n");
      printf("src -> dest1:nlinks1 dest2:nlinks2 ... destk:nlinksk\n\n");
   
      for(int i=0;i<size;i++)
      {
         
         printf("%d -> ",i);      
      
         for(int k=0;k<size;k++)
          {
               if(links[i][k]!=0)
                  printf("%d:%d ",k,links[i][k]);               
          }

         printf("\n");
         
      }

      printf("\n--- End of printing structure Oij ---\n");
}


/**
* @brief Prints the structure Oj
*
* This method prints the structure Oj.The structure Oj is an array which 
* holds the sum of links for each host to other hosts.
*
*@param sumHyperlinksFromHosts a pointer to t integers where integers represent
*        the number of 
*
* @param size the number of the hosts
*
* @return void
*/
void printStructureOj( int* sumHyperlinksFromHosts,int size)
{

      printf("+++ Start of printing structure Oj +++\n"); 
      printf("The structure of each line is:\n");
      printf("src ==> links_total\n\n");
   
      for(int i=0;i<size;i++)
      {
         
         printf("%d -> %d\n",i,sumHyperlinksFromHosts[i]);      
      
      }

      printf("\n--- End of printing structure Oj ---\n");

}


/**
*@brief Prints the structure Iij
*
*This method prints the structure Iij.Which means that this method prints the
* number of links that link to each host and te ids of the hosts that point 
* to them.
*
*
*
*@param links A pointer to a sequence of pointers which each pointer
*points to a sequence of integers.This data structure represents the number of 
*links of each host.
*
*@param  size the number of the hosts
*
* @return void
*
*/
void printStructureIij(int** links,int size)
{

      printf("+++ Start of printing structure Iij +++\n"); 
      printf("The structure of each line is:\n");
      printf("dest <== hosts_total hostid1 hostid2 ... hostidk\n\n");
   
      for(int k=0;k<size;k++)
      {

         int sum=0;

         for(int i=0;i<size;i++)
         {
            if(links[i][k])
              {
                 sum++;
              }
         }
   
         printf("%d <== %d ",k,sum);         
   
         for(int i=0;i<size;i++)
         {
            if(links[i][k])
              {
                 printf("%d ",i);
              }
         }     

         printf("\n");
      
      }

      printf("\n--- End of printing structure Iij ---\n");

}

/**
*
*@brief Prints the page ranks and the normalized page ranks of the hosts 
*
*
* This method prints the pageRanks and the normalized pageRanks of each host
* in a formatted way .
* 
*@param hostNames a pointer to a place in memory where a pointer(char**) of a 
*       a sequence of char* pointers exists.These char* pointers are pointers
*       to the characters of the names of the hosts.
*
*
* @param pageRanks a pointer to the page ranks of each host  
*
*
*@param  size the number of the hosts
*
*@return void
*
*/
void printPageRanks(char** hostNames,float* pageRanks,int size)
{

      printf("+++ Start of printing PR +++\n\n"); 
     
         
      printf("%-20s %-20s %-20s %-20s\n","host_id","host_name","host_rank",
      "norm_host_rank");


      for(int i=0;i<size;i++)
      {
        
        printf("%-20d %-20s %-20.3f %-20.2f\n",i,hostNames[i],pageRanks[i],
         calculateNormPageRank(pageRanks,i,size));  
       
      }

      printf("\n--- End of printing PR ---\n");

}


/**
*
*@brief Calculates the new page ranks of the hosts based on the previous 
*        pageRanks.
*
* This method calculates all the page ranks of the hosts by using the previous
* page ranks of the samse hosts and updates the array of the pageRanks passed
* as an argument.
*
*
* @param pageRanks a pointer to the page ranks of each host 
*
* @param previousPageRanks a pointer to the previous page ranks of each host 
*
*@param links A pointer to a sequence of pointers which each pointer
*points to a sequence of integers.This data structure represents the number of 
*links of each host.
*
*
*@param sumHyperlinksFromHosts a pointer to integers which integers indicate
*        the number of links from each host.This sequence of integers is 
*        one -to- one corresponding array with the ids of the hosts,which means
*        that id i is the ith element of the sequence which sequence is pointed
*        by the pointer sumHyperlinksFromHosts
*
*
*@return void
*
*/
void calculatePageRanksForHosts(float* pageRanks,float* previousPageRanks,
 int** links,int* sumHyperlinksFromHosts,int size)
{
   for(int i=0;i<size;i++)
      pageRanks[i]=calculatePageRank( previousPageRanks,links,
      sumHyperlinksFromHosts, size, i);
   
}



/**
*
* @brief Calculates the number of links from each host
*
*This method calculates the number of links from each host that exist
*
*
*@param links A pointer to a sequence of pointers which each pointer
*points to a sequence of integers.This data structure represents the number of 
*links of each host.
*
*
*@param sumHyperlinksFromHosts a pointer to integers which integers indicate
*        the number of links from each host.This sequence of integers is 
*        one -to- one corresponding array with the ids of the hosts,which means
*        that id i is the ith element of the sequence which sequence is pointed
*        by the pointer sumHyperlinksFromHosts
*        
*
*@param  size the number of the hosts 
*
*@return void
*/
void calculateSumsHyperlinksFromHost( int** links,int*
 sumHyperlinksFromHosts,int size)
{
   int sum=0;

   
   for(int i=0;i<size;i++)
   {  
      sum=0;
     
      for(int j=0;j<size;j++)
         {
          sum+=links[i][j];
         }     
       
      sumHyperlinksFromHosts[i]=sum;
   }


}




/**
*@brief This method calculates the page rank of a host
*
*This method calculates the page rank of a host specified by hostID.I
*
*@param pageRanks a pointer to the page ranks of each host
*
*@param links A pointer to a sequence of pointers which each pointer
*points to a sequence of integers.This data structure represents the number of 
*links of each host.
*
*@param sumHyperlinksFromHosts a pointer to integers which integers indicate
*        the number of links from each host.This sequence of integers is 
*        one -to- one corresponding array with the ids of the hosts,which means
*        that id i is the ith element of the sequence which sequence is pointed
*        by the pointer sumHyperlinksFromHosts
*
*@param  size the number of the hosts
*
*
*@param hostID the id of the host of which the page rank will be calculated
*
*@return  the page rank for the specified host(indicated by hostID)
*
*/
float calculatePageRank(float* pageRanks, int** links,int* 
sumHyperlinksFromHosts,int size,int hostID)
{
   float fraction=(1-D)/size;

   float sum=0;

   for(int i=0;i<size;i++)
     {
         if(sumHyperlinksFromHosts[i]!=0)
         sum+=(pageRanks[i]*links[i][hostID])/ sumHyperlinksFromHosts[i];
              
     }

   
   return fraction+D*sum;
}


/**
*@brief calculates the euklideia Distance between pageRanks
*
*This method calculates the euklideia Distance between two vectors of  
* page ranks of the hosts.
*
*@param pageRanks a pointer to the page ranks of each host 
*
* @param previousPageRanks a pointer to the previous page ranks of each host 
*
*@param  size the number of the hosts
*
*@return thr euklideia distance
*/
float euklideiaDistance(float* const pageRanks,float* const previousPageRanks,
int size)
{
   
   float sum=0;

   for(int i=0;i<size;i++)
     sum+=(previousPageRanks[i]-pageRanks[i])*(previousPageRanks[i]-
      pageRanks[i]);
      
          
   return sqrt(sum);
}

/**
*@brief Calculates the page ranks of the hosts
*
*
* This method calculates the normalized page rank of the host specified by the  
* argument hostID.
*
* @param pageRanks a pointer to the page ranks of each host 
*
* @param hostID the id of the host that  
*
* @return the page rank of a host based on the pageRanks of the 
*         the rest hosts
*/
float calculateNormPageRank(float* const pageRanks,int hostID,int size)
{

   float sum=0;

   for(int i=0;i<size;i++)
      sum+=pageRanks[i];      
  

   return pageRanks[hostID]/sum;
}



/**
* @This is the main method of the program where the program starts its ececution
*
* From this method the program is controlled .The arrays which are used in this
* program are created dynamically in this method and are passed as parameters
* to each method in order to complete a set of operations at the elements of
* thr arrays(like reading the host names and placing them in a sequence of 
* of pointers which point to characters and many more operations that appeal
* and affect the data on the arrays).After the creation and the initialization
* of the arrays with the correct data the arrays are used for calculating sets
* and at the end for calculating the page ranks of each web host. 
*
*
*
* @param argc the number of arguments given in command line(the name of the 
* file which is excecuted,is included in the command line arguments)
*
* @param argv an array of pointers type of char*--> an array of pointers to 
*        characters which these characters represent the arguments
*        passed in the program by the coomand line(the name of the 
*        file which is excecuted,is included in the command line arguments)
*
* @return 0 if the program ran without any problem else any number that 
*        indicates a problem
*/
int main(int argc,char* argv[])
{
   

     if(argc!=3)
      {
         printf("Error : Not correct number of arguments.\n");
         
         return 0;
      }

      
     //Declaration of the arrays
   char** hostNames=NULL;
   int** links=NULL;
   int size=0; 

  ;
   int status=readHostNames( argv[1], &hostNames,&size);   
  
   if(status==FILE_ERROR)
      {
       printf("Error : Problems in reading or opening the file %s.\n",argv[1]);
       return 0;   
      }
      

   if(status==MEMORY_ALLOCATION_ERROR)
     {
   
      printf("Error : Problems in allocating memory.Maybe not enough space"
       "in heap.\n");
           
        return 0;
      }

      if(status==NOT_VALID_INFORMATION_FOR_HOSTS)
      {
         printf("Error : Not valid ids for the hosts.\n");
         return 0;

      }

     {
         
        int size2=0;
        status=readHostgraph(argv[2],&links,&size2);
        
       if(status==FILE_ERROR)
        {
           printf("Error : Problems in reading or opening the file %s.\n"
          ,argv[2]);
           return 0;   
        }

         if(status==MEMORY_ALLOCATION_ERROR)
         {
   
         printf("Error : Problems in allocating memory.Maybe not enough space"
         "in heap.\n");
            return 0;
          }
   
      if(status==NOT_VALID_INFORMATION_FOR_HOSTS)
      {
         printf("Error : Not valid ids for the hosts.\n");
         return 0;

      }

      // check if the two files had both the same number of max id host
      // or the same number of ids
         if(size2!=size)
         {
            printf("Error : Not good association between the files.\n");
            printf("The number of web-hosts indicated by file(%s)-(size=%d) "
             "is notthe same as from file(%s)-(size=%d).\n"
               ,argv[1],size,argv[2],size2);   

            return 0;
         } 

  

     }

      
   int* sumHyperlinksFromHosts=NULL;
   float* pageRanks=NULL;
   float* previousPageRanks=NULL;

   // checks for the errors in allocation of memory for the last
   // arrays that are going to be used in the program
   sumHyperlinksFromHosts=(int*)calloc(size,sizeof(int));
   status=(sumHyperlinksFromHosts==NULL)? MEMORY_ALLOCATION_ERROR : 0;
   
   pageRanks=(float*)calloc(size,sizeof(float));
   status=(pageRanks==NULL)? MEMORY_ALLOCATION_ERROR : status;   

   previousPageRanks=(float*)calloc(size,sizeof(float));
   status=(previousPageRanks==NULL)? MEMORY_ALLOCATION_ERROR : status;
   
   if(status==MEMORY_ALLOCATION_ERROR)
     {
   
      printf("Error : Problems in allocating memory.Maybe not enough space"
       "in heap.\n");
        return 0;
     }

   for(int i=0;i<size;i++)
      {
         pageRanks[i]=1;
      }

   

   // Calculate structure O(hj)
   calculateSumsHyperlinksFromHost(links,sumHyperlinksFromHosts,size);


   do
   {
      // copy the current page Ranks to the array of the previous page Ranks
      memcpy(previousPageRanks,pageRanks,size*sizeof(float));
      
      calculatePageRanksForHosts( pageRanks, previousPageRanks,links, 
      sumHyperlinksFromHosts, size);

      
   }while(euklideiaDistance( pageRanks, previousPageRanks, size)>=FRACTION);
   // while the euklideia distance between the previous page ranks and the 
   // current page ranks of the hosts is greater than FRACTION
   
 
   printf("\n");
   printStructureOij( links, size);

   printf("\n");
   printStructureOj( sumHyperlinksFromHosts,size);

   printf("\n");
   printStructureIij( links, size);

   printf("\n");
   printPageRanks(hostNames,previousPageRanks,size);

   return 0;

}
