/********************************************************************************
*  main1-1.c																	*
*  Το αρχείο αυτό προσομοιώνει τον ρόλο ενός υψηλότερου-επιπέδου αρθρώματος			*
*  ενός ΣΔΒΔ το οποίο καλεί συναρτήσεις του επιπέδου που ασχολείται με  τον			*
*  χειρισμό αρχείων κατακερματισμού(ΑΚ). Η προσοχή του αρχείου επικεντρώνεται			* 
*  στην δημιουργία ΑΚ αρχείων και στην εισαγωγή και αναζήτηση εγγραφών				*
********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include "HT.h"	/* Αυτό το αρχείο μπορεί να περιέχει δηλώσεις που θέλετε να γνωρίζει 
					το πρόγραμμα που καλεί τις συναρτήσεις σας.*/


#define NUMBEROFBUCKETS 5
#define STRING 'c'
#define INTEGER 'i'
#define FLOAT 'f'
#define EQUAL 1
#define NOTEQUAL 2

char empName[40];
char empAge[40];
char empSal[40];
char empDname[40];


void insertEntries(int eNentry, int eAentry, int eSentry, int eDentry, 
                             char* ename, int eage, float esal, char* edname,
int recid)
{
	char errStr[40];

	strcpy(errStr,"Error in HT_InsertEntry");

	if (HT_InsertEntry(eNentry, (void *) ename,    (void *) &recid) != HTE_OK) 
		{
			sprintf(errStr, "Error in HT_InsertEntry called on %s\n",empName);
			HT_PrintError(errStr);
		}
	if (HT_InsertEntry(eAentry, (void *) &eage,    (void *) ename) != HTE_OK) 
		{
			sprintf(errStr, "Error in HT_InsertEntry called on %s \n",empAge);
			HT_PrintError(errStr);
		}
	if (HT_InsertEntry(eSentry, (void *) &esal,   (void *) ename) != HTE_OK) 
		{
			sprintf(errStr, "Error in HT_InsertEntry called on %s \n",empSal);
			HT_PrintError(errStr);
		}
	if (HT_InsertEntry(eDentry, (void *) edname,   (void *) &esal) != HTE_OK) 
		{
			sprintf(errStr, "Error in HT_InsertEntry called on %s \n",empDname);
			HT_PrintError(errStr);
		}

}

int main()
{
	int   scan1;

	int   eNentry;
	int   eAentry;
	int   eSentry;
	int   eDentry;

	char  ename[40];
	int    eage;
	float  esal;
	char  edname[40];

	char deptName[40];
	char retvalue[40];

	int recordid=1;
	int count;

	char errStr[200];
	
	int* ivalue=NULL;
	char* cvalue=NULL;
	float* fvalue=NULL;

/********************************************************************************
*  Αρχικοποίηση των εσωτερικών δομών του λογισμικού των ΑΚ						*
********************************************************************************/
	HT_Init();
	strcpy(empName ,"EMPNAME");
	strcpy(empAge,"EMPAGE");
	strcpy(empSal,"EMPSAL");
	strcpy(empDname,"EMPDNAME");
	strcpy(deptName,"DEPTNAME");

/********************************************************************************
*  Δημιουργία ΑΚ που θα περιέχουν πληροφορίες για υπαλλήλους					*
********************************************************************************/

	if ((scan1=HT_CreateIndex(empName,STRING,sizeof(empName)-1,INTEGER,sizeof(int),NUMBEROFBUCKETS)) != HTE_OK)
	{   
			sprintf(errStr,"Error in HT_CreateIndex called on %s \n",empName);
			HT_PrintError(errStr);
		}

	if (HT_CreateIndex(empAge,INTEGER,sizeof(int),STRING,39,NUMBEROFBUCKETS) != HTE_OK) 
		{
			sprintf(errStr, "Error in HT_CreateIndex called on %s \n",empAge);
			HT_PrintError(errStr);
		}

	if (HT_CreateIndex(empSal,FLOAT,sizeof(float),STRING,39,NUMBEROFBUCKETS) != HTE_OK) 
	{ 
			sprintf(errStr, "Error in HT_CreateIndex called on %s \n",empSal);
			HT_PrintError(errStr);
		}	

	if (HT_CreateIndex(empDname,STRING,sizeof(empDname)-1,FLOAT,sizeof(float),NUMBEROFBUCKETS) != HTE_OK) 
		{
			sprintf(errStr, "Error in HT_CreateIndex called on %s \n",empDname);
			HT_PrintError(errStr);
		}

/********************************************************************************
*  Άνοιγμα των τεσσάρων ΑΚ (με σκοπό την εισαγωγή εγγραφών)						*
********************************************************************************/

	if ((eNentry=HT_OpenIndex(empName)) < 0) 
		{
			sprintf(errStr, "Error in HT_OpenIndex called on %s \n",empName);
			HT_PrintError(errStr);
		}

	if ((eAentry=HT_OpenIndex(empAge)) < 0) 
		{
			sprintf(errStr, "Error in HT_OpenIndex called on %s \n",empAge);
			HT_PrintError(errStr);
		}

	if ((eSentry=HT_OpenIndex(empSal)) < 0) 
		{
			sprintf(errStr, "Error in HT_OpenIndex called on %s \n",empSal);
			HT_PrintError(errStr);
		}

	if ((eDentry=HT_OpenIndex(empDname)) < 0) 
		{
			sprintf(errStr, "Error in HT_OpenIndex called on %s \n",empDname);
			HT_PrintError(errStr);
		}

/********************************************************************************
*  Εισαγωγή των δυάδων (τιμή1, τιμή2) στα ΑΚ. Υπάρχουν 100 συνολικά εισαγωγές	*
*  σε κάθε αρχείο, οι οποίες διαχωρίζονται με παύλες για δική σας ευκολία		*
********************************************************************************/


/*
-----------------------------------------------------------------------------*/
	recordid=0;

	strcpy(ename,"XRHSTOFORIDHS ANTWNHS");      
	eage=25; 
	esal=155.5f; 
	strcpy(edname,"GALATA");
 	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);
	
/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"PARAGWGOY MARIA");
	eage=30;
	esal=208.9f;
	strcpy(edname,"SALAMIA");
 	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"IWAKEIMIDHS ELEYUERIOS");
	eage=49;
	esal=255.1f;
	strcpy(edname,"XYMOI");
 	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"SYRIANOS ARHS");
	eage=59;
	esal=306.9f;
	strcpy(edname,"TYRIA");
 	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);


/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"KYRIAKIDHS GRHGORIOS");
	eage=27;
	esal=448.1f;
	strcpy(edname,"PSWMIA");
 	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"XANTZHKYRIAKOS ANTWNIOS");
	eage=69;
	esal=357.5f;
	strcpy(edname,"KREATA");
 	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"APELPISIADOY IWSHFINA");
	eage=21;
	esal=400.0f;
	strcpy(edname,"FROUTA");
 	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"PANTAZHS LEYTERHS");
	eage=19;
	esal=367.6f;
	strcpy(edname,"TYRIA");
 	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"KOYROPETROGLOY KWSTAS");
	eage=33;
	esal=315.8f;
	strcpy(edname,"XYMOI");
 	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"TSAXPINH EYLAMPIA");
	eage=42;
	esal=267.9f;
	strcpy(edname,"SAPOUNIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"TRIGWNIDHS PYQAGORAS");
	eage=51;
	esal=218.9f;
	strcpy(edname,"SALAMIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"PISIDOY ANTIGONH");
	eage=61;
	esal=169.1f;
	strcpy(edname,"XYMOI");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);
/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"ROYBAS APOSTOLOS");
	eage=25;
	esal=175.2f;
	strcpy(edname,"GLYKA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"APOSBOLHS ANDREAS");
	eage=36;
	esal=227.2f;
	strcpy(edname,"FROUTA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"ASTRONOMOGLOY ISAAK");
	eage=42;
	esal=278.2f;
	strcpy(edname,"KREATA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"MOLOTOFIDHS AYGOYSTHS");
	eage=53;
	esal=328.5f;
	strcpy(edname,"FROUTA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"KYTTAROGLOY RODOYLA");
	eage=65;
	esal=378.5f;
	strcpy(edname,"SAPOUNIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"MPALADOROS PETROS");
	eage=22;
	esal=428.1f;
	strcpy(edname,"XYMOI");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"OIKONOMIDHS ALKHS");
	eage=34;
	esal=478.8f;
	strcpy(edname,"GLYKA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"ARTEMIADH PETROYLA");
	eage=19;
	esal=436.5f;
	strcpy(edname,"TYRIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"PANTELIADOY ELENH");
	eage=44;
	esal=387.1f;
	strcpy(edname,"SALAMIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"PERISTERIADH ANNA");
	eage=52;
	esal=337.1f;
	strcpy(edname,"KREATA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"GEWRGAKOPOYLOS EYQYMIOS");
	eage=64;
	esal=286.7f;
	strcpy(edname,"FROUTA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"PANAGIWTOPOYLOY QEODWRA");
	eage=23;
	esal=238.0f;
	strcpy(edname,"LAXANIKA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"LASKARIDHS SWKRATHS");
	eage=36;
	esal=189.2f;
	strcpy(edname,"PSARIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"ZWGRAFOY ALEXANDRA");
	eage=19;
	esal=198.2f;
	strcpy(edname,"FROUTA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"NIKOLAKOPOYLOS IWANNHS");
	eage=41;
	esal=248.6f;
	strcpy(edname,"SALAMIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"STAMOS IASWNAS");
	eage=56;
	esal=295.5f;
	strcpy(edname,"KREATA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"TERZOPOYLOY ANASTASIA");
	eage=65;
	esal=347.1f;
	strcpy(edname,"FROUTA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"AFENTOYLIDHS AXILLEAS");
	eage=23;
	esal=398.1f;
	strcpy(edname,"XYMOI");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"ANASTASIADHS ALEXANDROS");
	eage=36;
	esal=449.2f;
	strcpy(edname,"SALAMIA");	
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"BELIBASSAKH AIMILIA");
	eage=44;
	esal=458.8f;
	strcpy(edname,"GLYKA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"XRISTODOYLIDOY ARISTEA");
	eage=56;
	esal=406.9f;
	strcpy(edname,"PSWMIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"MARKOY ANTWNIOS");
	eage=69;
	esal=359.9f;
	strcpy(edname,"KREATA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"EYAGGELOY LEWNIDAS");
	eage=70;
	esal=308.9f;
	strcpy(edname,"SALAMIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"SPAQARAKH SOFIA");
	eage=18;
	esal=257.8f;
	strcpy(edname,"TYRIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"STEQANAKHS APOSTOLOS");
	eage=19;
	esal=205.6f;
	strcpy(edname,"TYRIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"PARASKEYOPOYLOS ARGYRHS");
	eage=18;
	esal=157.9f;
	strcpy(edname,"XYMOI");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"ANTWNOPOYLOY ELISABET");      
	eage=22; 
	esal=169.0f; 
	strcpy(edname,"SALAMIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"GEWRGIADOY MARIA");
	eage=33; 
	esal=217.0f; 
	strcpy(edname,"KRASIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"SKLABOY MARQA");              
	eage=45; 
	esal=268.1f; 
	strcpy(edname,"PSARIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"BASLEIADHS ARISTARXOS");      
	eage=56; 
	esal=317.8f; 
	strcpy(edname,"PSARIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);
/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"SEREMETHS ANASTASIOS");       
	eage=63; 
	esal=368.8f; 
	strcpy(edname,"TYRIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"STAMATIADOY ARTEMIS");        
	eage=18; 
	esal=415.1f; 
	strcpy(edname,"SALAMIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"KONTOGIANNIDHS XRHSTOS");     
	eage=27; 
	esal=468.1f; 
	strcpy(edname,"LAXANIKA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"WRAIOPOYLOS KVNSTANTINOS");   
	eage=30; 
	esal=479.5f; 
	strcpy(edname,"TYRIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"ELEYQERIADHS MARKOS");        
	eage=49; 
	esal=428.1f; 
	strcpy(edname,"SALAMIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"ADAMOPOYLOS ANTWNIOS");       
	eage=57; 
	esal=377.0f; 
	strcpy(edname,"FROUTA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"EYSTAQIOY XRISTINA");         
	eage=63; 
	esal=328.1f; 
	strcpy(edname,"KREATA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"MARKOGIANNAKHS EYSTAQIOS");   
	eage=71; 
	esal=279.4f; 
	strcpy(edname,"TYRIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"KOKKINOS ARISTOTELHS");       
	eage=28; 
	esal=228.4f; 
	strcpy(edname,"FROUTA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"ZORMPAS KVNSTANTINOS");       
	eage=33; 
	esal=177.1f; 
	strcpy(edname,"SALAMIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"POLYKANDRIWTHS DHMHTRIOS");   
	eage=41; 
	esal=126.9f; 
	strcpy(edname,"KREATA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"MANWLIKIDHS ZHSHS");          
	eage=58; 
	esal=186.9f; 
	strcpy(edname,"PSWMIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"ZENEMPISHS IAKWBOS");         
	eage=69; 
	esal=237.9f; 
	strcpy(edname,"XYMOI");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"KALOGHROY ALKMHNH");          
	eage=19; 
	esal=288.2f; 
	strcpy(edname,"KREATA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"QEOTOKAS PARASKEYAS");        
	eage=25; 
	esal=337.9f; 
	strcpy(edname,"FROUTA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"KANARH EYQALEIA");            
	eage=38; 
	esal=385.1f; 
	strcpy(edname,"TYRIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"LYRISTHS EYSTAQIOS");         
	eage=18; 
	esal=438.9f; 
	strcpy(edname,"FROUTA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"TREMPELHS ALKHBIADHS");       
	eage=40; 
	esal=486.2f; 
	strcpy(edname,"KREATA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"ARGYRIADHS ANASTASIOS");      
	eage=52; 
	esal=447.9f; 
	strcpy(edname,"KRASIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"QWMADAKHS EMMANOYHL");        
	eage=66; 
	esal=398.1f; 
	strcpy(edname,"XYMOI");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"SIDHROPYLOS XRYSANQOS");      
	eage=21; 
	esal=349.3f; 
	strcpy(edname,"XYMOI");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"STELLAS PETROS");             
	eage=33; 
	esal=298.6f; 
	strcpy(edname,"FROUTA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"DHMHTRIADOY XARALAMPIA");     
	eage=44; 
	esal=245.9f; 
	strcpy(edname,"KREATA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"PALAIOLOGOS QWMAS");          
	eage=50; 
	esal=198.1f; 
	strcpy(edname,"XYMOI");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"TSAKIROGLOY ANASTASIOS");     
	eage=61; 
	esal=147.1f; 
	strcpy(edname,"PSWMIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"STAMATIOY DHMHTRIOS");        
	eage=24; 
	esal=159.6f; 
	strcpy(edname,"XYMOI");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"DEMERTZH AIMILIA");           
	eage=31; 
	esal=207.0f; 
	strcpy(edname,"SAPOUNIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"EYSTAQIOY MARIA");            
	eage=45; 
	esal=258.0f;
	strcpy(edname,"PSWMIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"MARKOGIANNAKH PARASKEYH");    
	eage=53; 
	esal=309.2f; 
	strcpy(edname,"TYRIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"DIAMANTOPOYLOS QEOXARHS");    
	eage=60; 
	esal=356.9f; 
	strcpy(edname,"FROUTA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"LEONTIOS STAMATHS");          
	eage=24; 
	esal=408.7f; 
	strcpy(edname,"LAXANIKA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"PAPPA EYQYMIA");              
	eage=31; 
	esal=457.0f; 
	strcpy(edname,"PSARIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"LAMPRINOYDHS NIKOLAOS");      
	eage=45; 
	esal=508.1f; 
	strcpy(edname,"SAPOUNIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"LOYKOPOYLOY BASILIKH");       
	eage=56; 
	esal=469.1f; 
	strcpy(edname,"FROUTA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"SKLABOS APOSTOLOS");          
	eage=61; 
	esal=418.7f; 
	strcpy(edname,"KREATA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"PANAGOPOYLOY EYANQIA");       
	eage=54; 
	esal=369.2f; 
	strcpy(edname,"KREATA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"MAMALH NIKOLETA");            
	eage=53; 
	esal=315.8f; 
	strcpy(edname,"TYRIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"PANAGIWTOPOYLOS XARALAMPOS"); 
	eage=34; 
	esal=268.2f; 
	strcpy(edname,"FROUTA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"XRYSANQAKOPOYLOS DHMHTRIOS"); 
	eage=25; 
	esal=218.7f; 
	strcpy(edname,"KREATA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"XRISTOOYLOS PANAGIWTHS");     
	eage=32; 
	esal=166.0f; 
	strcpy(edname,"SALAMIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"BASILAKAKH STAMATOYLA");      
	eage=43; 
	esal=175.0f; 
	strcpy(edname,"SAPOUNIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"TSOLAKAS KWNSTANTINOS");      
	eage=52; 
	esal=228.9f; 
	strcpy(edname,"KREATA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"KOLLIA AIKATERINH");          
	eage=65; 
	esal=277.2f; 
	strcpy(edname,"TYRIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"GKOYRHS NIKOLAOS");           
	eage=56; 
	esal=328.2f; 
	strcpy(edname,"TYRIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"KAZAKOS STEFANOS");           
	eage=41; 
	esal=376.8f; 
	strcpy(edname,"TYRIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"APOSTOLOPOYLOS KWNSTANTINOS");
	eage=33; 
	esal=428.6f; 
	strcpy(edname,"FROUTA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"PAPAQANASIOY ELENH");         
	eage=27; 
	esal=479.2f; 
	strcpy(edname,"FROUTA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"SOYTSOS IWANNHS");            
	eage=20; 
	esal=439.9f; 
	strcpy(edname,"KREATA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"MATSARIDHS XARALAMPOS");      
	eage=31; 
	esal=388.9f; 
	strcpy(edname,"LAXANIKA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"STAYROY FILIPPOS");           
	eage=19; 
	esal=339.1f; 
	strcpy(edname,"PSWMIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"TAGKOPOYLOS DHMHTRIOS");      
	eage=44; 
	esal=287.1f; 
	strcpy(edname,"KRASIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"REGKLEITHS STEFANOS");        
	eage=51; 
	esal=235.7f; 
	strcpy(edname,"KRASIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"LASKOS NIKOLAOS");            
	eage=60; 
	esal=188.3f; 
	strcpy(edname,"SAPOUNIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"ZANAKHS MIXAHL");             
	eage=18; 
	esal=198.1f; 
	strcpy(edname,"KREATA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"AMORIANOS KWNSTANTINOS");     
	eage=63; 
	esal=247.4f; 
	strcpy(edname,"SALAMIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"KOYTSOYKOS GEWRGIOS");        
	eage=54; 
	esal=297.9f; 
	strcpy(edname,"TYRIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"PAPASWTHRIOY KYRIAKOS");      
	eage=46; 
	esal=348.6f; 
	strcpy(edname,"TYRIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

	strcpy(ename,"LAMPROPOYLOY XRISTINA");      
	eage=31; 
	esal=399.0f; 
	strcpy(edname,"FROUTA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, 
                             ename, eage, esal, edname, ++recordid);

/*
-----------------------------------------------------------------------------*/

/********************************************************************************
*  Τέλος των εισαγωγών στα τέσσερα αρχεία ΑΚ										*
********************************************************************************/

	if (HT_CloseIndex(eNentry) != HTE_OK) 
		{
			sprintf(errStr, "Error in HT_CloseIndex called on %s \n",empName);
			HT_PrintError(errStr);
		}

	if (HT_CloseIndex(eAentry) != HTE_OK) 
		{
			sprintf(errStr, "Error in HT_CloseIndex called on %s \n",empAge);
			HT_PrintError(errStr);
		}

	if (HT_CloseIndex(eSentry) != HTE_OK) 
		{
			sprintf(errStr, "Error in HT_CloseIndex called on %s \n",empSal);
			HT_PrintError(errStr);
		}

	if (HT_CloseIndex(eDentry) != HTE_OK) 
		{
			sprintf(errStr, "Error in HT_CloseIndex called on %s \n",empDname);
			HT_PrintError(errStr);
		}

/********************************************************************************
*      Επερώτηση #1																*
*  Έυρεση της εγγραφής "LAMPROPOYLOY XRISTINA", γνωρίζοντας ότι το όνομα είναι		*
*  μοναδικό. Θα ψάξουμε και δεύτερη φορά και πρέπει να μας επιστρέψει λάθος			*
********************************************************************************/

	strcpy(ename,"LAMPROPOYLOY XRISTINA");
	
	printf("******************\n");
	printf("RESULT OF QUERY #1\n");
	printf("******************\n\n");

	if ((eNentry=HT_OpenIndex(empName)) < 0) 
		{
			sprintf(errStr, "Error in HT_OpenIndex called on %s \n",empName);
			HT_PrintError(errStr);
		}

/********************************************************************************
*  Άνοιγμα της αναζήτησης															*
********************************************************************************/
	if ((scan1=HT_OpenIndexScan(eNentry, EQUAL, (void *) ename)) < 0) 
		{
			sprintf(errStr, "Error in HT_OpenIndexScan called on %s \n",empName);
			HT_PrintError(errStr);
		}

/********************************************************************************
*  Εύρεση της πρώτης εγγραφής με όνομα "LAMPROPOYLOY XRISTINA"					*
********************************************************************************/
	printf("Searching for the first record with name 'LAMPROPOYLOY XRISTINA'\n");
	
	if ((ivalue=(int*)HT_FindNextEntry(scan1)) != (int *)HTE_EOF) 
				printf("%d \n\n",*ivalue);
	else 
		{
			sprintf(errStr, "Error in HT_FindNextEntry called on scan %d\n\n",scan1);
			HT_PrintError(errStr);
		}

/********************************************************************************
*  Ψάχνουμε και για δεύτερη εγγραφή η οποία δεν υπάρχει και θα πρέπει να μας			*	
*  επιστραφεί ανάλογο μήνυμα														*
********************************************************************************/
	printf("Searching for another record with name 'LAMPROPOYLOY XRISTINA'\n");

	if ((ivalue=(int*)HT_FindNextEntry(scan1)) != (int *)HTE_EOF) 
				printf("%d \n\n",*ivalue);
	else 
		{
			sprintf(errStr, "Error in HT_FindNextEntry called on scan %d",scan1);
			HT_PrintError(errStr);
		}

/********************************************************************************
*  Κλείσιμο της αναζήτησης και του αρχείου ΑΚ									*
********************************************************************************/
	if (HT_CloseIndexScan(scan1) != HTE_OK) 
		{
			sprintf(errStr, "Error in HT_CloseIndexScan called on scan %d\n",scan1);
			HT_PrintError(errStr);
		}

	if (HT_CloseIndex(eNentry) != HTE_OK) 
		{
			sprintf(errStr, "Error in HT_CloseIndex called on %s \n",empName);
			HT_PrintError(errStr);
		}

/********************************************************************************
*      Επερώτηση #2																*
*  Εύρεση των ονομάτων των υπαλλήλων με μισθό 315.8								*
********************************************************************************/

	esal = 315.8f;

	printf("\n\n******************\n");
	printf("RESULT OF QUERY #2\n");
	printf("******************\n\n");

	if ((eSentry=HT_OpenIndex(empSal)) < 0) 
		{
			sprintf(errStr, "Error in HT_OpenIndex called on %s \n",empSal);
			HT_PrintError(errStr);
		}

/********************************************************************************
*  Άνοιγμα της αναζήτησης															*
********************************************************************************/
	if ((scan1=HT_OpenIndexScan(eSentry, EQUAL, (void *) &esal)) < 0) 
		{
			sprintf(errStr, "Error in HT_OpenIndexScan called on %s\n",empSal);
			HT_PrintError(errStr);
		}

/********************************************************************************
*  Σάρωση των σχετικών εγγραφών και εκτύπωση των αντίστοιχων ονομάτων				*
********************************************************************************/
	count=0;
	while ((cvalue=(char*)HT_FindNextEntry(scan1)) != (char *)HTE_EOF) 
		{
			strcpy(retvalue,cvalue);
			printf("'%s'\n",retvalue);
			count++;
		}
	printf("\n%d records found\n",count);

/********************************************************************************
*  Κλείσιμο της αναζήτησης και του αρχείου ΑΚ										*
********************************************************************************/
	if (HT_CloseIndexScan(scan1) != HTE_OK) 
		{
			sprintf(errStr, "Error in HT_CloseIndexScan called on scan %d\n",scan1);
			HT_PrintError(errStr);
		}

	if (HT_CloseIndex(eSentry) != HTE_OK) 
		{
			sprintf(errStr, "Error in HT_CloseIndex called on %s \n",empSal);
			HT_PrintError(errStr);
		}

/********************************************************************************
*      Επερώτηση #3																*
*  Αναζήτηση στο αρχείο για τα ονόματα αυτών που δεν είναι 18 χρονών				*
********************************************************************************/

	eage = 18;
	
	printf("\n\n******************\n");
	printf("RESULT OF QUERY #3\n");
	printf("******************\n\n");

	if ((eAentry=HT_OpenIndex(empAge)) < 0) 
		{
			sprintf(errStr, "Error in HT_OpenIndex called on %s \n",empAge);
			HT_PrintError(errStr);
		}

/********************************************************************************
*  Άνοιγμα της αναζήτησης															*
********************************************************************************/
	if ((scan1=HT_OpenIndexScan(eAentry, NOTEQUAL, (void *) &eage)) < 0) 
		{
			sprintf(errStr, "Error in HT_OpenIndexScan called on %s\n",empAge);
			HT_PrintError(errStr);
		}

/********************************************************************************
*  Σάρωση των σχετικών εγγραφών και εκτύπωση των αντίστοιχων ονομάτων				*
********************************************************************************/
	count=0;
	while ((cvalue=(char*)HT_FindNextEntry(scan1)) != (char *)HTE_EOF) 
		{
			strcpy(retvalue,cvalue);
			printf("'%s'\n",retvalue);
			count++;
		}

	printf("\n%d records found\n",count);
/********************************************************************************
*  Κλείσιμο της αναζήτησης και του αρχείου ΑΚ										*
********************************************************************************/
	if (HT_CloseIndexScan(scan1) != HTE_OK) 
		{
			sprintf(errStr, "Error in HT_CloseIndexScan called on scan %d\n",scan1);
			HT_PrintError(errStr);
		}

	if (HT_CloseIndex(eAentry) != HTE_OK) 
		{
			sprintf(errStr, "Error in HT_CloseIndex called on %s \n",empAge);
			HT_PrintError(errStr);
		}

/********************************************************************************
*      Επερώτηση #4																*
*  Εύρεση των μισθών των υπαλλήλων στο τμήμα "Τυριά"								*
********************************************************************************/

	
	strcpy(edname,"TYRIA");
	
	printf("\n\n******************\n");
	printf("RESULT OF QUERY #4\n");
	printf("******************\n\n");

	if ((eDentry=HT_OpenIndex(empDname)) < 0) 
		{
			sprintf(errStr, "Error in HT_OpenIndex called on %s \n",empDname);
			HT_PrintError(errStr);
		}
	
/********************************************************************************
*  Άνοιγμα της αναζήτησης															*
********************************************************************************/
	if ((scan1=HT_OpenIndexScan(eDentry, EQUAL, (void *) edname)) < 0) 
		{
			sprintf(errStr, "Error in HT_OpenIndexScan called on %s\n",empDname);
			HT_PrintError(errStr);
		}

/********************************************************************************
*  Σάρωση των σχετικών εγγραφών και εκτύπωση των αντίστοιχων ονομάτων				*
********************************************************************************/
	count=0;
	while ((fvalue=(float*)HT_FindNextEntry(scan1)) != (float *)HTE_EOF) 
		{
			count++;
			printf("%.2f \n",*fvalue);
		}
	printf("\n%d records found\n",count);

/********************************************************************************
*  Κλείσιμο της αναζήτησης και του αρχείου ΑΚ										*
********************************************************************************/
	if (HT_CloseIndexScan(scan1) != HTE_OK) 
		{
			sprintf(errStr, "Error in HT_CloseIndexScan called on scan %d\n",scan1);
			HT_PrintError(errStr);
		}

	if (HT_CloseIndex(eDentry) != HTE_OK) 
		{
			sprintf(errStr, "Error in HT_CloseIndex called on %s \n",empSal);
			HT_PrintError(errStr);
		}

/********************************************************************************
*  Eπερώτηση #5																    *
*  Αναζήτηση στο enames για να βρεθεί το αναγνωριστικό της εγγραφής					*
*  XANTZHKYRIAKH NANTIA															*
********************************************************************************/

	strcpy(ename,"XANTZHKYRIAKH NANTIA");
	
	printf("\n\n******************\n");
	printf("RESULT OF QUERY #5\n");
	printf("******************\n");
	
	if ((eNentry=HT_OpenIndex(empName)) < 0) 
		{
			sprintf(errStr, "Error in HT_OpenIndex called on %s \n",empName);
			HT_PrintError(errStr);
		}

/********************************************************************************
*  Άνοιγμα της αναζήτησης															*
********************************************************************************/
	if ((scan1=HT_OpenIndexScan(eNentry, EQUAL, (void *) ename)) < 0) 
		{
			sprintf(errStr, "Error in HT_OpenIndexScan called on %s\n",empName);
			HT_PrintError(errStr);
		}

/********************************************************************************
*  Eύρεση της εγγραφής XANTZHKYRIAKH NANTIA.										*
********************************************************************************/
	if ((ivalue=(int*)HT_FindNextEntry(scan1)) != (int *)HTE_EOF) 
				printf("%d \n\n",*ivalue);
	else 
		{
			sprintf(errStr, "XANTZHKYRIAKH NANTIA was not found... (Note: This is the correct behaviour!)");
			HT_PrintError(errStr);
		}

/********************************************************************************
*  Κλείσιμο της αναζήτησης και του αρχείου ΑΚ										*
********************************************************************************/
	if (HT_CloseIndexScan(scan1) != HTE_OK) 
		{
			sprintf(errStr, "Error in HT_CloseIndexScan called on scan %d\n",scan1);
			HT_PrintError(errStr);
		}

	if (HT_CloseIndex(eNentry) != HTE_OK) 
		{
			sprintf(errStr, "Error in HT_CloseIndex called on %s \n",empName);
			HT_PrintError(errStr);
		}

/********************************************************************************
*  Τέλος του δοκιμαστικού προγράμματος για εισαγωγές/ διαγραφές/αναζητήσεις			*
********************************************************************************/


	return 0;
}
