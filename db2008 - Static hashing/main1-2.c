/********************************************************************************
*  main1-2.c																	*
*  �� ��������� ���� ������ ��� ���� ���� ���������� ������ �������� ���� ����	*
*  �� ����� ����� ����������� ��� �� ������� ��� ���������� �� �� ������		*
*  ���������������. ���� ����������� ��� ����� ���� �� ���������, �������		*
*  ��������� ������� �� ��������� ��� �����������								*
********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "HT.h"	/* ���� �� ������ ������ �� �������� �������� ��� ������ �� �������� 
						�� ��������� ��� ����� ��� ����������� ���.*/

#define STRING 'c'
#define INTEGER 'i'
#define EQUAL 1

int main()
{
	int   scan1;
	int   scan2;

	char empName[40];
	char empAge[40];
	char empSal[40];
	char empDname[40];
	char deptName[40];

	int   eNentry=0;
	int   eAentry=0;
	int   eSentry=0;
	int   eDentry=0;
	int   dNentry=0;

	char  ename[40];
	int    eage;
	float  esal;
	char  edname[30];

	int   count;

	char errStr[200];
	
	int* ivalue=NULL;
	float* fvalue=NULL;
	char* cvalue=NULL;
	int recordid=0;
	char retvalue[40];
	
	strcpy(empName ,"EMPNAME");
	strcpy(empAge, "EMPAGE");
	strcpy(empSal, "EMPSAL");
	strcpy(empDname, "EMPDNAME");
	strcpy(deptName, "DEPTNAME");

/********************************************************************************
*  ������������ ��� ���������� ����� ��� ���������� ��� ��.						*
********************************************************************************/
	HT_Init();

/********************************************************************************
*  ������� ��� �������� �� �� ����� ��� �������� ���� �������� ��� ���� ������	*
********************************************************************************/

	if ((eNentry=HT_OpenIndex(empName)) < 0) {
  	 sprintf(errStr,"Error in HT_OpenIndex called on %s \n",empName);
   	HT_PrintError(errStr);
	}

	if ((eAentry=HT_OpenIndex(empAge)) < 0) {
  	 sprintf(errStr,"Error in HT_OpenIndex called on %s \n",empAge);
	   HT_PrintError(errStr);
	}

	if ((eSentry=HT_OpenIndex(empSal)) < 0) {
  	 sprintf(errStr,"Error in HT_OpenIndex called on %s \n",empSal);
	   HT_PrintError(errStr);
	}

	if ((eDentry=HT_OpenIndex(empDname)) < 0) {
  	 sprintf(errStr,"Error in HT_OpenIndex called on %s \n",empDname);
	   HT_PrintError(errStr);
	}

/********************************************************************************
*      �������� #1																*
*  �������� ��� ������� ����� (����1,����2) ��� �� ���������� ��.				*
********************************************************************************/

	strcpy(ename,"PARASKEYOPOYLOS ARGYRHS");
	eage=18;
	esal=157.9f;
	strcpy(edname,"XYMOI");
	printf("\nRESULT OF DELETION #1\n\n");
	recordid=38;
	ivalue=&recordid;

	if (HT_DeleteEntry(eNentry, (void *) ename,  (void *) ivalue) != HTE_OK) {
         sprintf(errStr,"Error in HT_DeleteEntry called on %s\n",empName);
           HT_PrintError(errStr);
	}

	if (HT_DeleteEntry(eAentry, (void *) &eage,   (void *) ename) != HTE_OK) {
  	 sprintf(errStr,"Error in HT_DeleteEntry called on %s \n",empAge);
	   HT_PrintError(errStr);
	}

	if (HT_DeleteEntry(eSentry, (void *) &esal,  (void *) ename) != HTE_OK) {
  	 sprintf(errStr,"Error in HT_DeleteEntry called on %s \n",empSal);
	   HT_PrintError(errStr);
	}

	if (HT_DeleteEntry(eDentry, (void *) edname,  (void *) &esal) != HTE_OK) {
  	 sprintf(errStr,"Error in HT_DeleteEntry called on %s\n",empDname);
	   HT_PrintError(errStr);
	}

	if (HT_CloseIndex(eNentry) != HTE_OK) {
  	 sprintf(errStr,"Error in HT_CloseIndex called on %s \n",empName);
	   HT_PrintError(errStr);
	}

	if (HT_CloseIndex(eAentry) != HTE_OK) {
  	 sprintf(errStr,"Error in HT_CloseIndex called on %s \n",empAge);
	   HT_PrintError(errStr);
	}

	if (HT_CloseIndex(eSentry) != HTE_OK) {
  	 sprintf(errStr,"Error in HT_CloseIndex called on %s \n",empSal);
	   HT_PrintError(errStr);
	}

	if (HT_CloseIndex(eDentry) != HTE_OK) {
  	 sprintf(errStr,"Error in HT_CloseIndex called on %s\n",empDname);
	   HT_PrintError(errStr);
	}

/********************************************************************************
*      �������� #2																*
*  �������� �� ���� ��� ������� : ������ = 18. ������ �� ��������� 4 ��������.	*
*  ���������� ��� ���� ��� PARASKEYOPOYLOS ARGYRHS ��� ����� ��� ������ �� ����	*
*  - ��������� ����																*
********************************************************************************/

	eage = 18;
	printf("\nRESULT OF DELETION #2\n\n");

	if ((eAentry=HT_OpenIndex(empAge)) < 0) {
  	 sprintf(errStr,"Error in HT_OpenIndex called on %s \n",empAge);
	   HT_PrintError(errStr);
	}

/********************************************************************************
*  ������� ��� �������															*
********************************************************************************/
	if ((scan1=HT_OpenIndexScan(eAentry, EQUAL, (void *) &eage)) < 0) {
  	 sprintf(errStr,"Error in HT_OpenIndexScan called on %s\n",empAge);
	   HT_PrintError(errStr);
	}

/********************************************************************************
*  ������ ��� �������� �������� ��� ���������� �������� ����					*
********************************************************************************/
	count=0;
	while ((cvalue=(char*)HT_FindNextEntry(scan1)) != (char *)HTE_EOF) {
		strcpy(retvalue,cvalue);
		printf("Deleting %s\n", retvalue);
    if (HT_DeleteEntry(eAentry, (void *) &eage, retvalue) != HTE_OK) {
    	sprintf(errStr,"Error in HT_DeleteEntry called on %s\n",empAge);
      HT_PrintError(errStr);
	  }
  	count++;
	}
	printf("%d records deleted\n",count);

/********************************************************************************
*  �������� ��� ������� ��� ��� ����������										*
********************************************************************************/
	if (HT_CloseIndexScan(scan1) != HTE_OK) {
  	 sprintf(errStr,"Error in HT_CloseIndexScan called on scan %d\n",scan1);
	   HT_PrintError(errStr);
	}

	if (HT_CloseIndex(eAentry) != HTE_OK) {
  	 sprintf(errStr,"Error in HT_CloseIndex called on %s \n",empAge);
	   HT_PrintError(errStr);
	}

/********************************************************************************
*      ��������� #1																*
*  ������ ��� �������� ���  ��������� ��� �������� 287.1 �����.					*
********************************************************************************/

    printf("\n\n******************\n");
	printf("RESULT OF QUERY #1\n");
	printf("******************\n\n");

	esal = 287.1f;

	if ((eSentry=HT_OpenIndex(empSal)) < 0) {
  	 sprintf(errStr,"Error in HT_OpenIndex called on %s \n",empSal);
	   HT_PrintError(errStr);
	}

/********************************************************************************
*  ������� ��� �������															*
********************************************************************************/
	if ((scan1=HT_OpenIndexScan(eSentry, EQUAL, (void *) &esal)) < 0) {
  	 sprintf(errStr,"Error in HT_OpenIndexScan called on %s\n",empSal);
	   HT_PrintError(errStr);
	}

/********************************************************************************
*  ��������� ��� �������� ��������.												*
********************************************************************************/
	while ((cvalue=(char *)HT_FindNextEntry(scan1)) != (char *)HTE_EOF) {
  		strcpy(retvalue,cvalue);
		printf("'%s' \n",retvalue);
	}

/********************************************************************************
*  �������� ��� ������� ��� ��� ����������										*
********************************************************************************/
	if (HT_CloseIndexScan(scan1) != HTE_OK) {
  	 sprintf(errStr,"Error in HT_CloseIndexScan called on scan %d\n",scan1);
	   HT_PrintError(errStr);
	}

	if (HT_CloseIndex(eSentry) != HTE_OK) {
  	 sprintf(errStr,"Error in HT_CloseIndex called on %s \n",empSal);
	   HT_PrintError(errStr);
	}

/********************************************************************************
*      ��������� #2																*
*  ������ ��� �������� ��� ��������� ��� ����� 61 ������.						*
********************************************************************************/

	printf("\n\n******************\n");
	printf("RESULT OF QUERY #2\n");
	printf("******************\n\n");

	eage = 61;

	if ((eAentry=HT_OpenIndex(empAge)) < 0) {
  	 sprintf(errStr,"Error in HT_OpenIndex called on %s \n",empAge);
	   HT_PrintError(errStr);
	}	
	
/********************************************************************************
*  ������� ��� �������															*
********************************************************************************/
	if ((scan1=HT_OpenIndexScan(eAentry, EQUAL, (void *) &eage)) < 0) {
  	 sprintf(errStr,"Error in HT_OpenIndexScan called on %s\n",empAge);
	   HT_PrintError(errStr);
	}

/********************************************************************************
*  ������ ��� �������� ��������													*
********************************************************************************/
	while ((cvalue=(char *)HT_FindNextEntry(scan1)) != (char *)HTE_EOF) {
		strcpy(retvalue,cvalue);
		printf("'%s' \n",retvalue);
	}

/********************************************************************************
*  �������� ��� ������� ��� ��� ����������										*
********************************************************************************/
	if (HT_CloseIndexScan(scan1) != HTE_OK) {
  	 sprintf(errStr,"Error in HT_CloseIndexScan called on scan %d\n",scan1);
	   HT_PrintError(errStr);
	}

	if (HT_CloseIndex(eAentry) != HTE_OK) {
  	 sprintf(errStr,"Error in HT_CloseIndex called on %s \n",empAge);
	   HT_PrintError(errStr);
	}

/********************************************************************************
*  ���������� ���� �� ��� �� �������� ����������� ��� �������					*
********************************************************************************/
	if (HT_CreateIndex(deptName,STRING,20,INTEGER,sizeof(int),2) != HTE_OK) {
  	 sprintf(errStr,"Error in HT_CreateIndex called on %s\n",deptName);
	   HT_PrintError(errStr);
	}

/********************************************************************************
*  ������� ��� ���� �� �� ����� ��� �������� ��������							*
********************************************************************************/

	if ((dNentry=HT_OpenIndex(deptName)) < 0) {
  	 sprintf(errStr,"Error in HT_OpenIndex called on %s \n",deptName);
	   HT_PrintError(errStr);
	}

/********************************************************************************
*  �������� ��� ������ (����1,����2) ��� ��� ������								*
********************************************************************************/
	recordid=200;
	ivalue=&recordid;

	strcpy(edname,"GALATA");
	++recordid;
	
	if (HT_InsertEntry(dNentry, (void *) edname, ivalue) != HTE_OK) {
   sprintf(errStr,"Error in HT_InsertEntry called on %s\n",deptName);
   HT_PrintError(errStr);
	}

	strcpy(edname,"SALAMIA");
	++recordid;
	
	if (HT_InsertEntry(dNentry, (void *) edname, ivalue) != HTE_OK) {
   sprintf(errStr,"Error in HT_InsertEntry called on %s\n",deptName);
   HT_PrintError(errStr);
	}

	strcpy(edname,"TYRIA");
	++recordid;
	
	if (HT_InsertEntry(dNentry, (void *) edname, ivalue) != HTE_OK) {
   sprintf(errStr,"Error in HT_InsertEntry called on %s\n",deptName);
   HT_PrintError(errStr);
	}

	strcpy(edname,"FROUTA");
	++recordid;

	if (HT_InsertEntry(dNentry, (void *) edname, ivalue) != HTE_OK) {
   sprintf(errStr,"Error in HT_InsertEntry called on %s\n",deptName);
   HT_PrintError(errStr);
	}

	strcpy(edname,"XYMOI");
	++recordid;

	if (HT_InsertEntry(dNentry, (void *) edname, ivalue) != HTE_OK) {
   sprintf(errStr,"Error in HT_InsertEntry called on %s\n",deptName);
   HT_PrintError(errStr);
	}

	strcpy(edname,"KRASIA");
	++recordid;

	if (HT_InsertEntry(dNentry, (void *) edname, ivalue) != HTE_OK) {
   sprintf(errStr,"Error in HT_InsertEntry called on %s\n",deptName);
   HT_PrintError(errStr);
	}

	strcpy(edname,"PSWMIA");
	++recordid;

	if (HT_InsertEntry(dNentry, (void *) edname, ivalue) != HTE_OK) {
   sprintf(errStr,"Error in HT_InsertEntry called on %s\n",deptName);
   HT_PrintError(errStr);
	}

	strcpy(edname,"KREATA");
	++recordid;

	if (HT_InsertEntry(dNentry, (void *) edname, ivalue) != HTE_OK) {
   sprintf(errStr,"Error in HT_InsertEntry called on %s\n",deptName);
   HT_PrintError(errStr);
	}

	strcpy(edname,"GLYKA");
	++recordid;

	if (HT_InsertEntry(dNentry, (void *) edname, ivalue) != HTE_OK) {
   sprintf(errStr,"Error in HT_InsertEntry called on %s\n",deptName);
   HT_PrintError(errStr);
	}

	strcpy(edname,"SAPOUNIA");
	++recordid;

	if (HT_InsertEntry(dNentry, (void *) edname, ivalue) != HTE_OK) {
   sprintf(errStr,"Error in HT_InsertEntry called on %s\n",deptName);
   HT_PrintError(errStr);
	}

	strcpy(edname,"LAXANIKA");
	++recordid;

	if (HT_InsertEntry(dNentry, (void *) edname, ivalue) != HTE_OK) {
   sprintf(errStr,"Error in HT_InsertEntry called on %s\n",deptName);
   HT_PrintError(errStr);
	}

	strcpy(edname,"PSARIA");
	++recordid;

	if (HT_InsertEntry(dNentry, (void *) edname, ivalue) != HTE_OK) {
   sprintf(errStr,"Error in HT_InsertEntry called on %s\n",deptName);
   HT_PrintError(errStr);
	}

/********************************************************************************
*  ����� ��� ���������															*
********************************************************************************/

	if (HT_CloseIndex(dNentry) != HTE_OK) {
  	 sprintf(errStr,"Error in HT_CloseIndex called on %s\n",deptName);
	   HT_PrintError(errStr);
	}

/********************************************************************************
*  E�������� #3																    *
*  ��������� ��� enames ��� �� ������ �� ������������� ��� ��������				*
*  ZYMARIKA															*
********************************************************************************/

	strcpy(edname,"ZYMARIKA");
	
	printf("\n\n******************\n");
	printf("RESULT OF QUERY #3\n");
	printf("******************\n");
	
	if ((dNentry=HT_OpenIndex(deptName)) < 0) 
		{
			sprintf(errStr, "Error in HT_OpenIndex called on %s \n",deptName);
			HT_PrintError(errStr);
		}

/********************************************************************************
*  ������� ��� ����������														*
********************************************************************************/
	if ((scan1=HT_OpenIndexScan(dNentry, EQUAL, (void *) edname)) < 0) 
		{
			sprintf(errStr, "Error in HT_OpenIndexScan called on %s\n",empName);
			HT_PrintError(errStr);
		}

/********************************************************************************
*  E����� ��� �������� XANTZHKYRIAKH NANTIA.                                    *
********************************************************************************/
	if ((ivalue=(int*)HT_FindNextEntry(scan1)) != (int *)HTE_EOF) 
				printf("%d \n\n",*ivalue);
	else 
		{
			sprintf(errStr, "ZYMARIKA was not found... (Note: This is the correct behaviour!)");
			HT_PrintError(errStr);
		}

/********************************************************************************
*  �������� ��� ���������� ��� ��� ������� ��									*
********************************************************************************/
	if (HT_CloseIndexScan(scan1) != HTE_OK) 
		{
			sprintf(errStr, "Error in HT_CloseIndexScan called on scan %d\n",scan1);
			HT_PrintError(errStr);
		}

	if (HT_CloseIndex(dNentry) != HTE_OK) 
		{
			sprintf(errStr, "Error in HT_CloseIndex called on %s \n",deptName);
			HT_PrintError(errStr);
		}

/********************************************************************************
*      E�������� # 4															*
*  ������ �������� �������� �� ��� �� ����������. � ��������� ���� �����������	*
*  ��� ��� (���������) ��������� � ����� �������� ��������� �������������		*
*  �����������.																	*
********************************************************************************/

	strcpy(edname,"FROUTA");
	
	printf("\n\n******************\n");
	printf("RESULT OF QUERY #4\n");
	printf("******************\n\n");

	if ((eDentry=HT_OpenIndex(empDname)) < 0) {
  	 sprintf(errStr,"Error in HT_OpenIndex called on %s \n",empDname);
	   HT_PrintError(errStr);
	}

	if ((dNentry=HT_OpenIndex(deptName)) < 0) {
  	 sprintf(errStr,"Error in HT_OpenIndex called on %s \n",deptName);
	   HT_PrintError(errStr);
	}

/********************************************************************************
*  ��������� ��� ��������� ������												*
********************************************************************************/
	if ((scan1=HT_OpenIndexScan(eDentry, EQUAL, (void *) edname)) < 0) {
  	 sprintf(errStr,"Error in HT_OpenIndexScan called on %s\n",deptName);
   	HT_PrintError(errStr);
	}

/********************************************************************************
*  ������ ��� �������� �������� ��� ��������� ������ ��� ��� ����				*
*  ������� ��� ���������, ������ ���� ������������� ����������					*
********************************************************************************/
	while ((fvalue=(float*)HT_FindNextEntry(scan1)) != (float *)HTE_EOF) {
	  printf("salary: %.2f \n",*fvalue);

   /*************************************************************************
    *  ������� ��� ������������� ����������                                 *
    *************************************************************************/
    if ((scan2=HT_OpenIndexScan(dNentry, EQUAL, (void *) edname)) < 0) {
       sprintf(errStr,"Error in HT_OpenIndexScan called on %s\n",empDname);
       HT_PrintError(errStr);
	    }
	   
   /*************************************************************************
    *  ������������ ��������� ��� �������� ��� �������� ��������            *
    *************************************************************************/
  	printf("Department id:  ");
   	while ((ivalue=(int *)HT_FindNextEntry(scan2)) != (int *)HTE_EOF) {
       printf("%d \n",*ivalue);
    }
    printf("\n");

   /*************************************************************************
    *  �������� ��� ������������� ����������                                *
    *************************************************************************/
		if (HT_CloseIndexScan(scan2) != HTE_OK) {
     sprintf(errStr,"Error in HT_CloseIndexScan called on scan %d\n",scan2);
     HT_PrintError(errStr);
	   }
	}

/********************************************************************************
*  �������� ��� ���������� ���������� ��� ��� ��� �������						*
********************************************************************************/
	if (HT_CloseIndexScan(scan1) != HTE_OK) {
	   sprintf(errStr,"Error in HT_CloseIndexScan called on scan %d\n",scan1);
  	 HT_PrintError(errStr);
	}

	if (HT_CloseIndex(eDentry) != HTE_OK) {
	   sprintf(errStr,"Error in HT_CloseIndex called on %s\n",empDname);
  	 HT_PrintError(errStr);
	}

	if (HT_CloseIndex(dNentry) != HTE_OK) {
	   sprintf(errStr,"Error in HT_CloseIndex called on %s\n",deptName);
  	 HT_PrintError(errStr);
	}

/********************************************************************************
*      E�������� # 5															*
*  ������ �������� �������� �� ��� �� ����������. � ��������� ���� �����������	*
*  ��� ��� (���������) ��������� � ����� �������� ������������ ���������		*
*  ���� ��� ��������.															*
********************************************************************************/

	printf("\n\n******************\n");
	printf("RESULT OF QUERY #5\n");
	printf("******************\n\n");

	strcpy(edname,"KREATA");

	if ((eDentry=HT_OpenIndex(empDname)) < 0) {
  	 sprintf(errStr,"Error in HT_OpenIndex called on %s \n",empDname);
	   HT_PrintError(errStr);
	}

	if ((dNentry=HT_OpenIndex(deptName)) < 0) {
  	 sprintf(errStr,"Error in HT_OpenIndex called on %s \n",deptName);
	   HT_PrintError(errStr);
	}

/********************************************************************************
*  ��������� ��� ��������� ������												*
********************************************************************************/
	if ((scan1=HT_OpenIndexScan(dNentry, EQUAL, (void *) edname)) < 0) {
	   sprintf(errStr,"Error in HT_OpenIndexScan called on %s\n",empDname);
	   HT_PrintError(errStr);
	}
		   

/********************************************************************************
*  ������ ��� �������� �������� ��� ��������� ������ ��� ��� ����				*
*  ������� ��� ���������, ������ ���� ������������� ����������					*
********************************************************************************/
	while ((ivalue=(int*)HT_FindNextEntry(scan1)) != (int *)HTE_EOF) {
		
		printf("Department id: %d \n",*ivalue);

	   /*************************************************************************
		*  ������� ��� ������������� ����������                                 *
		*************************************************************************/
		if ((scan2=HT_OpenIndexScan(eDentry, EQUAL, (void *) edname)) < 0) {
  			sprintf(errStr,"Error in HT_OpenIndexScan called on %s\n",deptName);
	   		HT_PrintError(errStr);
		}
		   
	   /*************************************************************************
		*  ������������ ���������, �������� ���� ��� �������� ��� ��������		*
		*  ��������																*
		*************************************************************************/
   		while ((fvalue=(float*)HT_FindNextEntry(scan2)) != (float *)HTE_EOF) {
			printf("salary: %.2f \n",*fvalue);

			if (HT_DeleteEntry(eDentry, (void *) edname, fvalue) != HTE_OK) {
				sprintf(errStr,"Error in HT_DeleteEntry called on %s\n",empAge);
				HT_PrintError(errStr);
			}
		}
		printf("\n");


	   /*************************************************************************
		*  �������� ��� ������������� ����������                                *
		*************************************************************************/
		if (HT_CloseIndexScan(scan2) != HTE_OK) {
		 sprintf(errStr,"Error in HT_CloseIndexScan called on scan %d\n",scan2);
		 HT_PrintError(errStr);
		}
	}

/********************************************************************************
*  �������� ��� ���������� ����������											*
********************************************************************************/
	if (HT_CloseIndexScan(scan1) != HTE_OK) {
	   sprintf(errStr,"Error in HT_CloseIndexScan called on scan %d\n",scan1);
  	 HT_PrintError(errStr);
	}

	if (HT_CloseIndex(eDentry) != HTE_OK) {
	   sprintf(errStr,"Error in HT_CloseIndex called on %s\n",empDname);
  	 HT_PrintError(errStr);
	}

	if (HT_CloseIndex(dNentry) != HTE_OK) {
	   sprintf(errStr,"Error in HT_CloseIndex called on %s\n",deptName);
  	 HT_PrintError(errStr);
	}


/********************************************************************************
*      E�������� # 6															*
*  ����� �� ��������. �� ��� ����� ���� ���� ��������� 4 ���� ��� �� ���������� *
*  ������ �����																	*
********************************************************************************/

	printf("\n\n******************\n");
	printf("RESULT OF QUERY #6\n");
	printf("******************\n\n");

	strcpy(edname,"KREATA");

	if ((eDentry=HT_OpenIndex(empDname)) < 0) {
  	 sprintf(errStr,"Error in HT_OpenIndex called on %s \n",empDname);
	   HT_PrintError(errStr);
	}

	if ((dNentry=HT_OpenIndex(deptName)) < 0) {
  	 sprintf(errStr,"Error in HT_OpenIndex called on %s \n",deptName);
	   HT_PrintError(errStr);
	}

/********************************************************************************
*  ��������� ��� ��������� ������												*
********************************************************************************/
	if ((scan1=HT_OpenIndexScan(dNentry, EQUAL, (void *) edname)) < 0) {
	   sprintf(errStr,"Error in HT_OpenIndexScan called on %s\n",empDname);
	   HT_PrintError(errStr);
	}
		   

/********************************************************************************
*  ������ ��� �������� �������� ��� ��������� ������ ��� ��� ����				*
*  ������� ��� ���������, ������ ���� ������������� ����������					*
********************************************************************************/
	while ((ivalue=(int*)HT_FindNextEntry(scan1)) != (int *)HTE_EOF) {
		
		printf("Department id: %d \n",*ivalue);

	   /*************************************************************************
		*  ������� ��� ������������� ����������                                 *
		*************************************************************************/
		if ((scan2=HT_OpenIndexScan(eDentry, EQUAL, (void *) edname)) < 0) {
  			sprintf(errStr,"Error in HT_OpenIndexScan called on %s\n",deptName);
	   		HT_PrintError(errStr);
		}
		   
	   /*************************************************************************
		*  ������������ ���������, �������� ���� ��� �������� ��� ��������		*
		*  ��������																*
		*************************************************************************/
   		while ((fvalue=(float*)HT_FindNextEntry(scan2)) != (float *)HTE_EOF) {
			printf("salary: %.2f \n",*fvalue);

			if (HT_DeleteEntry(eDentry, (void *) edname, fvalue) != HTE_OK) {
				sprintf(errStr,"Error in HT_DeleteEntry called on %s\n",empAge);
				HT_PrintError(errStr);
			}
		}
		printf("\n");


	   /*************************************************************************
		*  �������� ��� ������������� ����������                                *
		*************************************************************************/
		if (HT_CloseIndexScan(scan2) != HTE_OK) {
		 sprintf(errStr,"Error in HT_CloseIndexScan called on scan %d\n",scan2);
		 HT_PrintError(errStr);
		}
	}

/********************************************************************************
*  �������� ��� ���������� ����������											*
********************************************************************************/
	if (HT_CloseIndexScan(scan1) != HTE_OK) {
	   sprintf(errStr,"Error in HT_CloseIndexScan called on scan %d\n",scan1);
  	 HT_PrintError(errStr);
	}

	if (HT_CloseIndex(eDentry) != HTE_OK) {
	   sprintf(errStr,"Error in HT_CloseIndex called on %s\n",empDname);
  	 HT_PrintError(errStr);
	}

	if (HT_CloseIndex(dNentry) != HTE_OK) {
	   sprintf(errStr,"Error in HT_CloseIndex called on %s\n",deptName);
  	 HT_PrintError(errStr);
	}

	
	
/********************************************************************************
*  ����� ��� ������������														*
********************************************************************************/

	return 0;
}

 
