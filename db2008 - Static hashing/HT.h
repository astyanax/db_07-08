/*
HT.h

std06066 Αναστασίου Βασίλειος-Μάριος
std06101 Καρατζάς Ευστράτιος
*/

#ifndef _HT_H_
#define _HT_H_

/******************************************************************************/
/*                             DEFINES                                        */
/******************************************************************************/

#define BLOCKSIZE 512
#define MAXOPENFILES 20
#define MAXSCANS 20

#define HTE_OK 1
#define HTE_EOF -1
#define HTE_MEM_ERROR -2
#define HTE_OPEN_FILE_ERROR -3
#define HTE_CREATE_FILE_ERROR -4
#define HTE_CLOSE_FILE_ERROR -5
#define HTE_FWRITE_ERROR -6
#define HTE_FREAD_ERROR -7
#define HTE_PRINTF_ERROR -8
#define HTE_SPRINTF_ERROR -9
#define HTE_WRONG_SCANDESC -10
#define HTE_WRONG_FILEDESC -11
#define HTE_REM_FILE -12
#define HTE_WRONG_KEY -13
#define HTE_WRONG_OP -14


#define MAXFILENAMESIZE 50

#define EQUAL 1
#define NOT_EQUAL 2

/******************************************************************************/
/*                          HASHING FUNCTIONS                                 */
/******************************************************************************/

void HT_Init(void);

int HT_CreateIndex(
    char   *fileName,      /* όνομα αρχείου */
    char   attrType1,      /* τύπος πρώτου πεδίου: 'c' (σειρά χαρακτήρων), 'i' (ακέραιος), 'f' (πραγματικός) */
    int    attrLength1,    /* μήκος πρώτου πεδίου: 4 γιά 'i' ή 'f', 1-255 γιά 'c' */
    char   attrType2,      /* τύπος δεύτερου πεδίου: 'c' (σειρά χαρακτήρων), 'i' (ακέραιος), 'f' (πραγματικός) */
    int    attrLength2,    /* μήκος δεύτερου πεδίου: 4 γιά 'i' ή 'f', 1-255 γιά 'c' */
    int    buckets         /* αριθμός κάδων κατακερματισμού */
);

int HT_DestroyIndex(
    char   *fileName       /* όνομα αρχείου */
);

int HT_OpenIndex(
    char   *fileName       /* όνομα αρχείου */
);

int HT_CloseIndex(
    int    fileDesc        /* αριθμός που αντιστοιχεί στο ανοιχτό αρχείο */
);

int HT_InsertEntry(
    int    fileDesc,       /* αριθμός που αντιστοιχεί στο ανοιχτό αρχείο */
    void   *value1,        /* τιμή του πεδίου-κλειδιού προς εισαγωγή */
    void   *value2         /* τιμή του δεύτερου πεδίου της εγγραφής προς εισαγωγή */
);

int HT_DeleteEntry(
    int    fileDesc,       /* αριθμός που αντιστοιχεί στο ανοιχτό αρχείο */
    void   *value1,        /* τιμή του πεδίου-κλειδιού προς απαλοιφή */
    void   *value2         /* τιμή του δεύτερου πεδίου της εγγραφής προς απαλοιφή */
);

int HT_OpenIndexScan(
    int    fileDesc,       /* αριθμός που αντιστοιχεί στο ανοιχτό αρχείο */
    int    op,             /* τελεστής σύγκρισης */
    void   *value          /* τιμή του πεδίου-κλειδιού προς σύγκριση */
);

void *HT_FindNextEntry(
    int    scanDesc        /* αριθμός που αντιστοιχεί στην ανοιχτή σάρωση */
);

int HT_CloseIndexScan(
    int    scanDesc        /* αριθμός που αντιστοιχεί στην ανοιχτή σάρωση */
);

void HT_PrintError(
    char    *errString     /* κείμενο για εκτύπωση */
);

int hash_funct(
    void * key,
    char type,
    int buckets
);

void load_bitmap(
    unsigned char *bitmap,
    int bitmap_size,
    FILE * fp
);

/******************************************************************************/
/*                          GENERAL FUNCTIONS                                 */
/******************************************************************************/

unsigned char BIT_IsSet(
    char ch,               /* Xarakthras pou meletatai */
    char pos               /* 8esh tou bit pou meletatai */
);

int BIT_Flip(
    unsigned char *ch,              /* ari8mos pros allagh */
    char pos               /* 8esh tou bit pou tha enallax8ei */
);

#endif
