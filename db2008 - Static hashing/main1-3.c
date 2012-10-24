/********************************************************************************
* main1-3.c																		*
* Αυτό το πρόγραμμα παίζει τον ρόλο ενός αρθρώματος υψηλού επιπέλου ενος ΣΔΒΔ	*
* το οποίο διαγράφει όλα τα αρχεία κατακερματισμού λόγο της διαγραφής όλης		*
* της βάσης.																	*
********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "HT.h"     /* Αυτό το αρχείο μπορεί να περιέχει δηλώσεις που θέλετε να γνωρίζει 
						το πρόγραμμα που καλεί τις συναρτήσεις σας.*/

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
*  Αρχικοποίηση των εσωτερικών δομών του λογισμικού των ΑΚ						*
********************************************************************************/
	HT_Init();

/********************************************************************************
*  Διαγραφή των αρχείων κατακερματισμού, για παράδειγμα λόγω της διαγραφής		*
*  ολόκληρης της βάσης															*
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

