#ifndef _SORT_H_
#define _SORT_H_

/******************************************************************************/
/*                             DEFINES                                        */
/******************************************************************************/

#define BLOCKSIZE 512

#define OK 1
#define Sort_EOF -1
#define MEM_ERROR -2
#define OPEN_FILE_ERROR -3
#define CREATE_FILE_ERROR -4
#define CLOSE_FILE_ERROR -5
#define FWRITE_ERROR -6
#define FREAD_ERROR -7
#define REM_FILE -8
#define ATTR_ERROR -9
#define WRONG_BUFFSIZE_ERROR -10

/******************************************************************************/
/*                          SORTING FUNCTIONS                                 */
/******************************************************************************/

void Sort_Init(void);

int Sort_Create_File(
    char *filename,
    char atrrtype1,
    int attrlength1,
    char atrrtype2,
    int attrlength2
);

int Sort_Destroy_File(
    char *fileName
);

int Sort_InsertEntry(
char *filename,        /* όνομα αρχείου */
void *value1,          /* τιμή του πρώτου πεδίου της εγγραφής προς εισαγωγή */
void *value2           /* τιμή του δεύτερου πεδίου της εγγραφής προς εισαγωγή */
);

int Sort_ExtSort(
char *fileName,        /* όνομα αρχείου */
int bufferSize         /* μέγεθος ενδιάμεσης μνήμης σε μπλοκ */
);

int Sort_Print(
    char *fileName     /* όνομα αρχείου */
);

void Sort_PrintError(
    char *errString    /* κείμενο για εκτύπωση */
);

/******************************************************************************/
/*                          GENERAL FUNCTIONS                                 */
/******************************************************************************/

int cmp(const void * a, const void * b);
void swap_internal(char *a, char *b, size_t size);
void qqsort_internal(char *begin, char *end, size_t size1, size_t size2);
void qqsort(void *base, size_t nmemb, size_t size1, size_t size2);

int logbase (float base, float x);

int merge( char *temp_block, int * subfile_distance, int * subfile_size, int N, int buffersize, FILE * fpin, char * buffer);

int total_recs_in_subfile(int subfile_distance, int subfile_size, FILE * fpin, char * buffer);
int find_min(int *current_block_in_subfile, char *buffer, FILE *fpin, int buffersize, int *subfile_distance,
              int * total_records_in_subfile, int N, char *temp_block);

char *blocks_minimum(char * block);

unsigned char BIT_IsSet(
    char ch,               /* Xarakthras pou meletatai */
    char pos               /* 8esh tou bit pou meletatai */
);

int BIT_Flip(
    unsigned char *ch,     /* ari8mos pros allagh */
    char pos               /* 8esh tou bit pou tha enallax8ei */
);

#endif
