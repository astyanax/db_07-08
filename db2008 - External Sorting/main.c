//main.c
#include <stdio.h>
#include <stdlib.h>
#include "Sort.h"

int main(void)
{
   int numOfRecs, errno;
   char val1[8];
   char val2[8];
   char val3[15];
   int val4;
   FILE *input, *output;

   ////////////////////////////////////////////////////////////// input-data1//////////////////////////////////////////////////////////

   //Initialize structures
   Sort_Init();

   //Create File input-data1 -> to be sorted
   if ((errno = Sort_Create_File("input-data1", 'c', 8, 'c', 15)) != OK)
	   Sort_PrintError("Problem in Sort_Create_File for file input-data1\n");
 

   if ((input = fopen("file_1.txt","r")) == NULL) {
	   fprintf(stderr,"You must have file_1.txt to run this program! \n");
	   exit(1);
   }

   //Insert records of file file_1.txt to file input-data1
   do {
      fscanf(input, "%s %s\n", val1, val3);
      if ((errno = Sort_InsertEntry("input-data1", val1, val3)) != OK)
	      Sort_PrintError("Problem in insertion of record in file input-data1\n");

   } while(!feof(input) );
     
   fclose(input);

   //External Sorting of file input-data1 - error
   if ((errno = Sort_ExtSort("input-data1", 2)) != OK)
	   Sort_PrintError("Not enough buffer size for the external sorting of file input-data1\n");


   numOfRecs = Sort_Print("input-data1");

   if (numOfRecs > 0)
	   printf("numOfRecs for file input-data1 is %d\n", numOfRecs);
   else
	   Sort_PrintError("Problem in Sort_Print for file input-data1\n");


   //External Sorting of file input-data1
   if ((errno = Sort_ExtSort("input-data1", 5)) != OK)
	   Sort_PrintError("Problem in Sort_ExtSort for file input-data1\n");

   numOfRecs = Sort_Print("input-data1");

   if (numOfRecs > 0)
	   printf("numOfRecs for file input-data1 is %d\n", numOfRecs);
   else
	   Sort_PrintError("Problem in Sort_Print for file input-data1\n");


   numOfRecs = Sort_Destroy_File("input-data1");

   if (numOfRecs > 0)
	   printf("numOfRecs for file input-data1 is %d\n", numOfRecs);
   else
	   Sort_PrintError("Problem in Sort_Destroy_File for file input-data1\n");



   ////////////////////////////////////////////////////////////// input-data2//////////////////////////////////////////////////////////

   //Initialize structures
   Sort_Init();

   //Create File input-data2 -> to be sorted
   if ((errno = Sort_Create_File("input-data2",'c', 15, 'i', 4)) != OK)
	   Sort_PrintError("Problem in Sort_Create_File for file input-data2\n");

   if ((input = fopen("file_2.txt","r")) == NULL) {
	    fprintf(stderr,"You must have file_2.txt to run this program! \n");
	    exit(1);
   }

   //Insert records of file file_2.txt to file input-data2
   do{
      fscanf(input, "%s %d\n", val3, &val4);

      if ((errno = Sort_InsertEntry("input-data2", val3, &val4)) != OK)
	      Sort_PrintError("Problem in insertion of record in file input-data2\n");

     } while(!feof(input) );

   fclose(input);
   
   
   numOfRecs = Sort_Print("input-data2");

   if (numOfRecs > 0)
	   printf("numOfRecs for file input-data2 is %d\n", numOfRecs);
   else
	   Sort_PrintError("Problem in Sort_Print for file input-data2\n");


   //External Sorting of file input-data2
   if ((errno = Sort_ExtSort("input-data2", 4)) != OK)
	   Sort_PrintError("Problem in Sort_ExtSort for file input-data2\n");

   numOfRecs = Sort_Print("input-data2");

   if (numOfRecs > 0)
	   printf("numOfRecs for file input-data2 is %d\n", numOfRecs);
   else
	   Sort_PrintError("Problem in Sort_Print for file input-data2\n");


   numOfRecs = Sort_Destroy_File("input-data2");

   if (numOfRecs > 0)
	   printf("numOfRecs for file input-data2 is %d\n", numOfRecs);
   else
	   Sort_PrintError("Problem in Sort_Destroy_File for file input-data2\n");


   ////////////////////////////////////////////////////////////// input-data3//////////////////////////////////////////////////////////

   //Initialize structures
   Sort_Init();

   //Create File input-data3 -> to be sorted
   if ((errno = Sort_Create_File("input-data3",'c', 8, 'c', 8)) != OK)
	   Sort_PrintError("Problem in Sort_Create_File for file input-data3\n");

   if ((input = fopen("file_3.txt","r")) == NULL) {
	    fprintf(stderr,"You must have file_3.txt to run this program! \n");
	    exit(1);
   }

   //Insert records of file file_3.txt to file input-data3
   do{
      fscanf(input, "%s %s\n", val1, val2);

      if ((errno = Sort_InsertEntry("input-data3", val1, val2)) != OK)
	      Sort_PrintError("Problem in insertion of record in file input-data3\n");

     } while(!feof(input) );

   fclose(input);
   
   
   numOfRecs = Sort_Print("input-data3");

   if (numOfRecs > 0)
	   printf("numOfRecs for file input-data3 is %d\n", numOfRecs);
   else
	   Sort_PrintError("Problem in Sort_Print for file input-data3\n");


   //External Sorting of file input-data3
   if ((errno = Sort_ExtSort("input-data3", 9)) != OK)
	   Sort_PrintError("Problem in Sort_ExtSort for file input-data3\n");


   numOfRecs = Sort_Print("input-data3");

   if (numOfRecs > 0)
	   printf("numOfRecs for file input-data3 is %d\n", numOfRecs);
   else
	   Sort_PrintError("Problem in Sort_Print for file input-data3\n");


   numOfRecs = Sort_Destroy_File("input-data3");

   if (numOfRecs > 0)
	   printf("numOfRecs for file input-data3 is %d\n", numOfRecs);
   else
	   Sort_PrintError("Problem in Sort_Destroy_File for file input-data3\n");


   
   ////////////////////////////////////////////////////////////// input-data4//////////////////////////////////////////////////////////

   //Initialize structures
   Sort_Init();

   //Create File input-data4 -> to be sorted
   if ((errno = Sort_Create_File("input-data4",'i', 4, 'c', 8)) != OK)
	   Sort_PrintError("Problem in Sort_Create_File for file input-data4\n");

   if ((input = fopen("file_4.txt","r")) == NULL) {
	    fprintf(stderr,"You must have file_4.txt to run this program! \n");
	    exit(1);
   }

   //Insert records of file file_4.txt to file input-data4
   do{
      fscanf(input, "%d %s\n", &val4, val1);

      if ((errno = Sort_InsertEntry("input-data4", &val4, val1)) != OK)
	      Sort_PrintError("Problem in insertion of record in file input-data3\n");

     } while(!feof(input) );

   fclose(input);
   
   numOfRecs = Sort_Print("input-data4");

   if (numOfRecs > 0)
	   printf("numOfRecs for file input-data4 is %d\n", numOfRecs);
   else
	   Sort_PrintError("Problem in Sort_Print for file input-data4\n");


   //External Sorting of file input-data4
   if ((errno = Sort_ExtSort("input-data4", 6)) != OK)
	   Sort_PrintError("Problem in Sort_ExtSort for file input-data4\n");


   numOfRecs = Sort_Print("input-data4");

   if (numOfRecs > 0)
	   printf("numOfRecs for file input-data4 is %d\n", numOfRecs);
   else
	   Sort_PrintError("Problem in Sort_Print for file input-data4\n");


   numOfRecs = Sort_Destroy_File("input-data4");

   if (numOfRecs > 0)
	   printf("numOfRecs for file input-data4 is %d\n", numOfRecs);
   else
	   Sort_PrintError("Problem in Sort_Destroy_File for file input-data4\n");

   return 0;

}
