/********************************************************************************
* main1-3.c																		*
* ���� �� ��������� ������ ��� ���� ���� ���������� ������ �������� ���� ����	*
* �� ����� ��������� ��� �� ������ ��������������� ���� ��� ��������� ����		*
* ��� �����.																	*
********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "HT.h"     /* ���� �� ������ ������ �� �������� �������� ��� ������ �� �������� 
						�� ��������� ��� ����� ��� ����������� ���.*/

int main()
{
	char empName[40];
	char empAge[40];
	char empSal[40];
	char empDname[40];
	char deptName[40];

	char errStr[200];
	
	
	strcpy(empName,"EMPNAME");
	strcpy(empAge,"EMPAGE");
	strcpy(empSal,"EMPSAL");
	strcpy(empDname,"EMPDNAME");
	strcpy(deptName,"DEPTNAME");

/********************************************************************************
*  ������������ ��� ���������� ����� ��� ���������� ��� ��						*
********************************************************************************/
	HT_Init();

/********************************************************************************
*  �������� ��� ������� ���������������, ��� ���������� ���� ��� ���������		*
*  ��������� ��� �����															*
********************************************************************************/
	if (HT_DestroyIndex(empName) != HTE_OK) {
  	 sprintf(errStr,"Error in BT_DestroyIndex called on %s\n",empName);
	   HT_PrintError(errStr);
	}
	else printf("%s file destroyed\n",empName);

	if (HT_DestroyIndex(empAge) != HTE_OK) {
  	 sprintf(errStr,"Error in HT_DestroyIndex called on %s\n",empAge);
	   HT_PrintError(errStr);
	}
	else printf("%s file destroyed\n",empAge);

	if (HT_DestroyIndex(empSal) != HTE_OK) {
  	 sprintf(errStr,"Error in HT_DestroyIndex called on %s\n",empSal);
	   HT_PrintError(errStr);
	}
	else printf("%s file destroyed\n",empSal);

	if (HT_DestroyIndex(empDname) != HTE_OK) {
  	 sprintf(errStr,"Error in HT_DestroyIndex called on %s\n",empDname);
   	HT_PrintError(errStr);
	}
	else printf("%s file destroyed\n",empDname);

	if (HT_DestroyIndex(deptName) != HTE_OK) {
  	 sprintf(errStr,"Error in HT_DestroyIndex called on %s\n",deptName);
	   HT_PrintError(errStr);
	}
	else printf("%s file destroyed\n",deptName);
	
	return 0;
}

