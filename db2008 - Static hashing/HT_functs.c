/*
HT_functs.c

std06066 Αναστασίου Βασίλειος-Μάριος
std06101 Καρατζάς Ευστράτιος
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HT.h"

struct file * open_files[MAXOPENFILES];
struct scan * open_scans[MAXSCANS];

struct file_header {
    char attrType1;
    int  attrLength1;
    char attrType2;
    int  attrLength2;
    int  buckets;
    int  total_records;
    int  bitmap_size_in_bytes;
};

struct file {
    FILE   *fpointer;    /* Pointer pou xrhsimopoieitai gia na exoume access sto antistoixo arxeio */
    struct file_header *pfheader;    /* Tha "fortwnei" ton header tou arxeiou */
    char   filename[MAXFILENAMESIZE];
};

struct scan {
    int record;      /* H teleutaia eggrafh opou psaksame */
    int bucket;      /* To teleutaio bucket opou psaksame */
    int op;          /* O telesths sygkrishs (1==equal, 2 == not equal) */
    int fileDesc;    /* H 8esh tou ypo-meleth arxeiou ston pinaka anoixtwn arxeiwn */
    void *key;       /* H timh tou pediou-kleidiou */
    union Value2 *returnval;   /* Tha krataei thn timh tou 2ou pediou */
};

union Value2 {
    int dval;
    float fval;
    char *cval;
};

int HTE_errno;

char error_msg[][128] = {
    "Reached end of file",
    "Memory allocation error",
    "Error while opening file",
    "Error while creating file",
    "Error while closing file",
    "fwrite() didn't write as many bytes as it should",
    "fread() didn't read as many bytes as it should",
    "printf() failed",
    "sprintf() failed",
    "Wrong scanDesc provided",
    "Wrong fileDesc provided",
    "File could not be removed",
    "Wrong key provided",
    "Wrong operator provided"
};

/******************************************************************************/
/*                               FUNCTIONS                                    */
/******************************************************************************/


/**** void HT_Init(void) ****

 Input:
       * None

 Operation:
           * Arxikopoiei ka8olikes domes dedomenwn
 Output:
        * None

 Return Value:
        * None

 Comments:
          Apla arxikopoiei tous 2 global pinakes (twn sarwsewn kai twn arxeiwn)
*/

void HT_Init(void)
{
    int i;

    for (i = 0; i < MAXOPENFILES; i++) {
        open_files[i] = NULL;
    }

    for (i = 0; i < MAXSCANS; i++) {
        open_scans[i] = NULL;
    }
}


/**** int HT_CreateIndex(char*, char, int, char, int, int) ****

 Input:
       * char   *fileName        όνομα αρχείου
       * char   attrType1        τύπος πρώτου πεδίου-κλειδιού: 'c' (σειρά χαρακτήρων), 'i' (ακέραιος), 'f' (πραγματικός)
       * int    attrLength1      μήκος πρώτου πεδίου-κλειδιού: 4 γιά 'i' ή 'f', 1-255 γιά 'c'
       * char   attrType2        τύπος δεύτερου πεδίου: 'c' (σειρά χαρακτήρων), 'i' (ακέραιος), 'f' (πραγματικός)
       * int    attrLength2      μήκος δεύτερου πεδίου: 4 γιά 'i' ή 'f', 1-255 γιά 'c'
       * int    buckets          αριθμός κάδων κατακερματισμού

 Operation:
           * Dhmiourgei to arxeio me onoma fileName
           * O typos kai to mhkos ka8enos ap'ta 2 pedia didetai sta args 2-3, 4-5 antistoixa
           * Kratame kai to plh8os twn kadwn sthn parametro buckets
           * Kai arxikopoiei to proanafer8en arxeio
 Output:
        * Ena adeio hash file (arxeio katakermatismou)

 Return Value:
        * An epityxes: HTE_OK
        * Alliws: antistoixo kwdiko la8ous
*/

int HT_CreateIndex(char *fileName, char attrType1, int attrLength1, char attrType2, int attrLength2, int buckets)
{
    FILE *fp;

    struct file_header *pfileheader;
    int total_records, struct_size, i;
    char * pch;

    if ((fp = fopen(fileName, "wb")) == NULL) {
        HTE_errno = HTE_CREATE_FILE_ERROR;
        return HTE_CREATE_FILE_ERROR;
    }

    /* To pfileheader einai enas pointer se struct pou fylaei tis plhrofories
       tou header tou arxeiou                                                */
    if ((pfileheader = malloc(sizeof(struct file_header))) == NULL) {
        HTE_errno = HTE_MEM_ERROR;
        return HTE_MEM_ERROR;
    }

    /* Fortwma twn plhroforiwn sth mnhmh */
    pfileheader->attrType1 = attrType1;
    pfileheader->attrLength1 = attrLength1;
    pfileheader->attrType2 = attrType2;
    pfileheader->attrLength2 = attrLength2;
    pfileheader->buckets = buckets;

    /*
       * Ap'to synoliko mege8os tou block (BLOCKSIZE) afairoume 4 bytes pou
       xrhsimopoiountai gia thn fylaksh tou "pointer" sto overflow bucket.
       * 8eloume na doume poses eggrafes xwrane se auton ton xwro, kserontas pws
        gia ka8e record prepei na desmeusoume xwro iso me to mhkos twn 2 pediwn
        kai 1/8 tou byte gia thn antistoixh 8esh sto bitmap
    */
    total_records = (BLOCKSIZE - 4) / (float)(attrLength1 + attrLength2 + 0.125);

    /*
       Telika, to plh8os twn bits tou bitmap isoutai me to plh8os twn
       eggrafwn (total_records)
    */
    pfileheader->total_records = total_records;
    pfileheader->bitmap_size_in_bytes = total_records / 8 ;

    /* Edw einai mia manual efarmogh ths "ceiling()" */
    if ((float) total_records / (float) 8  > pfileheader->bitmap_size_in_bytes) {
        pfileheader->bitmap_size_in_bytes++;
    }

    /* Grafoume sto arxeio olo to header */
    struct_size = sizeof(struct file_header);

    if (fwrite(pfileheader, struct_size, 1, fp) != 1) {
        HTE_errno = HTE_FWRITE_ERROR;
        return HTE_FWRITE_ERROR;
    }

    /* Gemizoume ton ypoloipo xwro tou header-block me mhdenika (gia na mhn einai
       uninitialized kai exei skoupidia */
    if ((pch = malloc(BLOCKSIZE - struct_size + 1)) == NULL) {
        HTE_errno = HTE_MEM_ERROR;
        return HTE_MEM_ERROR;
    }

    pch[BLOCKSIZE - struct_size] = '\0';
    memset(pch, 0, BLOCKSIZE - struct_size);

    if (fwrite(pch, 1, BLOCKSIZE - struct_size, fp) != (unsigned int)(BLOCKSIZE - struct_size)) {
        HTE_errno = HTE_FWRITE_ERROR;
        return HTE_FWRITE_ERROR;
    }

    free(pch);

    /* Twra exoyme ftiaksei to header-block tou arxeiou kai prepei na gemisoyme
       me NUL to ypoloipo hash file analoga to plh8os twn buckets */

    if ((pch = malloc(BLOCKSIZE)) == NULL) {
        HTE_errno = HTE_MEM_ERROR;
        return HTE_MEM_ERROR;
    }

    memset(pch, 0, BLOCKSIZE);

    for (i = 0; i < buckets; i++) {
        if (fwrite(pch, 1, BLOCKSIZE, fp) != (unsigned int)BLOCKSIZE) {
            HTE_errno = HTE_FWRITE_ERROR;
            return HTE_FWRITE_ERROR;
        }
    }

    free(pch);

    /* To arxeio arxikopoih8hke */
    if (fclose(fp) == EOF) {
        HTE_errno = HTE_CLOSE_FILE_ERROR;
        return HTE_CLOSE_FILE_ERROR;
    }

    free(pfileheader);
    return HTE_OK;
}


/**** int HT_DestroyIndex(char *fileName) ****

 Input:
       * char *fileName          όνομα αρχείου

 Operation:
           * Katastrefei to arxeio me onoma fileName, svhnontas ola ta dedomena toy.

 Output:
        * None

 Return Value:
        * An epityxes: HTE_OK
        * Alliws: antistoixo kwdiko la8ous
*/

int HT_DestroyIndex(char *fileName)
{
    /* Sbhnetai to arxeio kai ta periexomena tou */
    if (remove(fileName) != 0) {
        HTE_errno = HTE_REM_FILE;
        return HTE_REM_FILE;
    }
    else {
        return HTE_OK;
    }
}


/**** int HT_OpenIndex (char *fileName) ****

 Input:
       * char *fileName          όνομα αρχείου

 Operation:
           * Anoigei to arxeio me onoma filename
 Output:
        * None

 Return Value:
        * An epityxes: Mh-arnhtikos akeraios ( < MAXOPENFILES) poy xrhsimopoieitai gia na anagnwrizetai to arxeio
        * Alliws: antistoixo kwdiko la8ous
*/

int HT_OpenIndex(char *fileName)
{
    FILE *fp;
    int it;

    /* Psaxnoume na broume kenh 8esh ston pinaka anoixtwn arxeiwn */

    for (it = 0 ; it < MAXOPENFILES ; it++) {
        if (open_files[it] == NULL) {
            break;
        }
    }

    /* O iterator kseperase to plh8os twn arxeiwn, opote den brhkame kenh 8esh */
    if (it == MAXOPENFILES) {
        HTE_errno = HTE_WRONG_FILEDESC;
        return HTE_WRONG_FILEDESC; /* THA ALLAX8EI */
    } 
    else
    {
        if ((fp = fopen(fileName, "rb+")) == NULL) {
            HTE_errno = HTE_OPEN_FILE_ERROR;
            return HTE_OPEN_FILE_ERROR;
        } 
        else
        {
            if ((open_files[it] = malloc(sizeof(struct file))) == NULL) {
                HTE_errno = HTE_MEM_ERROR;
                return HTE_MEM_ERROR;
            }

            strcpy(open_files[it]->filename, fileName);

            if ((open_files[it]->pfheader = malloc(sizeof(struct file_header))) == NULL) {
                HTE_errno = HTE_MEM_ERROR;
                return HTE_MEM_ERROR;
            }

            /* Fortwnoume sth mnhmh to header tou antistoixou arxeiou */
            if (fread(open_files[it]->pfheader, sizeof(struct file_header), 1, fp) != 1) {
                HTE_errno = HTE_FREAD_ERROR;
                return HTE_FREAD_ERROR;
            }

            /* Swzoume kai pointer pou deixnei sto sto arxeio gia na exoume access */
            open_files[it]->fpointer = fp;

            return it;
        }
    }
}


/**** int HT_CloseIndex (int fileDesc) ****

 Input:
       * int fileDesc            αριθμός που αντιστοιχεί στο ανοιχτό αρχείο

 Operation:
           * Kleinei to arxeio me onoma fileDesc
           * kai sbhnei thn antistoixh kataxwrhsh ston pinaka anoixtwn arxeiwn

 Output:
        * None

 Return Value:
        * An epityxes: HTE_OK
        * Alliws: antistoixo kwdiko la8ous
*/

int HT_CloseIndex(int fileDesc)
{
    if (open_files[fileDesc] == NULL) {
        HTE_errno = HTE_WRONG_FILEDESC;
        return HTE_WRONG_FILEDESC;
    }

    if (fclose(open_files[fileDesc]->fpointer) == EOF) {
        HTE_errno = HTE_CLOSE_FILE_ERROR;
        return HTE_CLOSE_FILE_ERROR;
    } 
    else
    { /* Sbhsimo tou antistoixou struct sth mnhmh kai diagrafh ap'ton pinaka anoixtwn arxeiwn */
        free(open_files[fileDesc]->pfheader);
        free(open_files[fileDesc]);
        open_files[fileDesc] = NULL;

        return HTE_OK;
    }
}


/**** int HT_InsertEntry(int fileDesc, void *value1, void *value2) ****

 Input:
       *   int  fileDesc         αριθμός που αντιστοιχεί στο ανοιχτό αρχείο
       *   void *value1          τιμή του πεδίου-κλειδιού προς εισαγωγή
       *   void *value2          τιμή του δεύτερου πεδίου της εγγραφής προς εισαγωγή

 Operation:
           * Eisagei to record <value1, value2> sto arxeio fileDesc

 Output:
        * None

 Return Value:
        * An epityxes: HTE_OK
        * Alliws: antistoixo kwdiko la8ous
*/

int HT_InsertEntry(int fileDesc, void *value1, void *value2)
{
    int target_bucket, i, overflow_pointer, tmp;
    unsigned char *bitmap, *pch;
    char overflow_pch[5];

    if (open_files[fileDesc] == NULL) {
        HTE_errno = HTE_WRONG_FILEDESC;
        return HTE_WRONG_FILEDESC;
    }

    /*
       Pername to pedio-kleidi sthn Hashing Function gia na mas epistrepsei se
       poio bucket prepei na steiloume thn eggrafh
    */
    if ((target_bucket = hash_funct(value1, open_files[fileDesc]->pfheader->attrType1, open_files[fileDesc]->pfheader->buckets)) < 0) {
        HTE_errno = HTE_WRONG_KEY;
        return HTE_WRONG_KEY;
    }

    /*
       Prospername to header-block (BLOCKSIZE) kai phgainoume sthn arxh tou
       epilegmenou bucket ( + target_bucket * BLOCKSIZE )
    */
    fseek(open_files[fileDesc]->fpointer, BLOCKSIZE + target_bucket * BLOCKSIZE, SEEK_SET);

    /* Desmeush mnhmhs gia na apo8hkeutei to bitmap */
    if ((bitmap = malloc(open_files[fileDesc]->pfheader->bitmap_size_in_bytes)) == NULL) {
        HTE_errno = HTE_MEM_ERROR;
        return HTE_MEM_ERROR;
    }

    do {
        /* Fortwsh tou anistoixou bitmap sth mnhmh */
        tmp = open_files[fileDesc]->pfheader->bitmap_size_in_bytes;
        
        if (fread(bitmap, 1, tmp, open_files[fileDesc]->fpointer) != (unsigned int)tmp)
        {
            HTE_errno = HTE_FREAD_ERROR;
            return HTE_FREAD_ERROR;
        }

        /* Elegxoume ap'thn arxh ola ta bits tou bitmap gia kenh 8esh */
        for (i = 0; i < open_files[fileDesc]->pfheader->total_records ; i++) {
            /*
                * To i/8 antistoixei sto i-osto byte tou bitmap
                * to i%8 mas dinei to bit ths antistoixhs eggrafhs
            */
            if (!BIT_IsSet(bitmap[i/8], i % 8)) {
                /* An bre8ei anenergh 8esh eggrafhs, thn desmeuoume sto bitmap */
                BIT_Flip(&bitmap[i/8], i % 8);
                break;
            }
        }

        /*
            An de bre8ei kenh 8esh eggrafhs sto bitmap, shmainei pws exoume collision
            sto sygkekrimeno bucket.
        */
        if (i == open_files[fileDesc]->pfheader->total_records) {
            /* Fortwnoume sth mnhmh ton "pointer" poy deixnei sto overflow bucket */
            if (fread(overflow_pch, 1, 4, open_files[fileDesc]->fpointer) != 4) {
                HTE_errno = HTE_FREAD_ERROR;
                return HTE_FREAD_ERROR;
            }

            overflow_pch[4] = '\0';
            overflow_pointer = atoi(overflow_pch);

            /*
                 An o "pointer" exei thn timh mhden, shmainei oti den yparxei
                 overflow bucket, opote to dhmiourgoume.
            */

            if (overflow_pointer == 0) {
                /*
                   O "pointer" tha periexei to plh8os twn bytes apo thn arxh tou
                   twrinou bucket mexri to telos tou arxeiou, opou tha proste8ei
                   o overflow bucket.
                */

                /* Gia na pame sthn arxh tou block, prepei na gyrisoume
                   kata "pointer_size" + "bitmap_size" bytes ap'thn twrinh 8esh */
                fseek(open_files[fileDesc]->fpointer, - (4 + open_files[fileDesc]->pfheader->bitmap_size_in_bytes), SEEK_CUR);

                /* To "i" isoutai me thn apostasth tou twrinou bucket
                   apo thn arxh tou arxeiou   */
                i = ftell(open_files[fileDesc]->fpointer);

                /* Briskoume to mege8os tou arxeiou */
                fseek(open_files[fileDesc]->fpointer, 0, SEEK_END);

                /* H apostash anamesa ston twrino bucket kai ton overflow isoutai
                   me to mege8os tou arxeiou meion thn apostash tou twrinou bucket
                   apo thn arxh. Briskoume epomenws thn metaksy tous apostash */
                overflow_pointer = ftell(open_files[fileDesc]->fpointer) - i;

                /*
                   Epistrefoume sto twrino bucket, outws wste na swsoume thn
                   proanafer8eisa timh ston "pointer" tou twrinou block.
                */
                fseek(open_files[fileDesc]->fpointer, open_files[fileDesc]->pfheader->bitmap_size_in_bytes - overflow_pointer , SEEK_CUR);

                if (sprintf(overflow_pch, "%d", overflow_pointer) < 0) {
                    HTE_errno = HTE_SPRINTF_ERROR;
                    return HTE_SPRINTF_ERROR;
                }

                if (fwrite(overflow_pch, 1, 4, open_files[fileDesc]->fpointer) != 4) {
                    HTE_errno = HTE_FWRITE_ERROR;
                    return HTE_FWRITE_ERROR;
                }

                /*
                   Phgainoume sto telos tou arxeiou wste na dhmiourghsoume
                   ton overflow bucket
                */
                fseek(open_files[fileDesc]->fpointer, 0, SEEK_END);

                /* Arxikopoioume to neo block me NUL */
                if ((pch = malloc(BLOCKSIZE)) == NULL) {
                    HTE_errno = HTE_MEM_ERROR;
                    return HTE_MEM_ERROR;
                }

                memset(pch, 0, BLOCKSIZE);

                /* Desmeuoume xwro gia thn eggrafh sto bitmap tou overflow bucket */
                BIT_Flip(&pch[0], 0);

                /* Apo8hkeuoume thn eggrafh */
                memcpy(pch + open_files[fileDesc]->pfheader->bitmap_size_in_bytes + 4, value1, open_files[fileDesc]->pfheader->attrLength1);
                memcpy(pch + open_files[fileDesc]->pfheader->bitmap_size_in_bytes + 4 + open_files[fileDesc]->pfheader->attrLength1, value2, open_files[fileDesc]->pfheader->attrLength2);

                /* Metaferoume ton overflow bucket ap'thn mnhmh sto arxeio. */

                if (fwrite(pch, 1, BLOCKSIZE, open_files[fileDesc]->fpointer) != BLOCKSIZE) {
                    HTE_errno = HTE_FWRITE_ERROR;
                    return HTE_FWRITE_ERROR;
                }

                free(pch);

                free(bitmap);

                return HTE_OK;
            }

            /*
               An o "pointer" tou bucket exei mh mhdenikh timh, shmainei oti
               yparxei hdh overflow bucker, sto opoion kai phgainoume
            */
            else
            {
                /*
                   Epeidh eimaste kata bitmap_size + "pointer"_size ap'thn arxh tou block,
                   phgainoume kata katallhlo plh8os bytes pisw, kai metaferomaste mprosta
                   oso mas ypodeiknyei o overflow pointer
                */
                fseek(open_files[fileDesc]->fpointer, overflow_pointer - open_files[fileDesc]->pfheader->bitmap_size_in_bytes - 4, SEEK_CUR);
            }
        }

        /* Me epanalhpsh ews otou broume kenh 8esh eggrafhs se kapoio bitmap */
    } while (i == open_files[fileDesc]->pfheader->total_records);

    /* Epistrefoume ton file pointer sthn arxh tou bucket */
    fseek(open_files[fileDesc]->fpointer, -(open_files[fileDesc]->pfheader->bitmap_size_in_bytes), SEEK_CUR);

    /* Kai apo8hkeuoume to neo bitmap sto arxeio */
    if (fwrite(bitmap, 1, open_files[fileDesc]->pfheader->bitmap_size_in_bytes, open_files[fileDesc]->fpointer) != (unsigned int) open_files[fileDesc]->pfheader->bitmap_size_in_bytes) {
        HTE_errno = HTE_FWRITE_ERROR;
        return HTE_FWRITE_ERROR;
    }

    free(bitmap);

    /*
       Prospername ton overflow pointer, kai odhgoume ton fpointer sthn
       kenh 8esh eggrafhs pou brhkame (i-osth 8esh),
       opou tha apo8hkeusoume thn eggrafh mas
    */
    fseek(open_files[fileDesc]->fpointer, 4 + i * (open_files[fileDesc]->pfheader->attrLength1 + open_files[fileDesc]->pfheader->attrLength2), SEEK_CUR);

    /*
       Analoga me ton typo twn pediwn, kanoume antistoixh apo8hkeush,
       elegxontas gia la8os typo dedomenwn
    */

    /* apothikeysh pedioy-kleidioy */
    if (open_files[fileDesc]->pfheader->attrType1 == 'c') {
        if (fwrite(value1, 1, open_files[fileDesc]->pfheader->attrLength1, open_files[fileDesc]->fpointer) != (unsigned int) open_files[fileDesc]->pfheader->attrLength1) {
            HTE_errno = HTE_FWRITE_ERROR;
            return HTE_FWRITE_ERROR;
        }
    } 
    else if (open_files[fileDesc]->pfheader->attrType1 == 'i' || open_files[fileDesc]->pfheader->attrType1 == 'f') {
        if (fwrite(value1, 4, 1, open_files[fileDesc]->fpointer) != 1) {
            HTE_errno = HTE_FWRITE_ERROR;
            return HTE_FWRITE_ERROR;
        }
    } 
    else {
        HTE_errno = HTE_WRONG_KEY;
        return HTE_WRONG_KEY;
    }

    /* apothikeysh 2oy pedioy */
    if (open_files[fileDesc]->pfheader->attrType2 == 'c') {
        if (fwrite(value2, 1, open_files[fileDesc]->pfheader->attrLength2, open_files[fileDesc]->fpointer) != (unsigned int) open_files[fileDesc]->pfheader->attrLength2) {
            HTE_errno = HTE_FWRITE_ERROR;
            return HTE_FWRITE_ERROR;
        }
    } 
    else if (open_files[fileDesc]->pfheader->attrType2 == 'i' || open_files[fileDesc]->pfheader->attrType2 == 'f') {
        if (fwrite(value2, 4, 1, open_files[fileDesc]->fpointer) != 1) {
            HTE_errno = HTE_FWRITE_ERROR;
            return HTE_FWRITE_ERROR;
        }
    } 
    else {
        HTE_errno = HTE_WRONG_KEY;
        return HTE_WRONG_KEY;
    }

    return HTE_OK;

}


/**** int HT_DeleteEntry(int fileDesc, void *value1, void *value2) ****

 Input:
       *   int  fileDesc         αριθμός που αντιστοιχεί στο ανοιχτό αρχείο
       *   void *value1          τιμή του πεδίου-κλειδιού προς απαλοιφή
       *   void *value2          τιμή του δεύτερου πεδίου της εγγραφής προς απαλοιφή

 Operation:
           * Diagrafei to record <value1, value2> apo to arxeio fileDesc

 Output:
        * None

 Return Value:
        * An epityxes: HTE_OK
        * Alliws: antistoixo kwdiko la8ous
*/

int HT_DeleteEntry(int fileDesc, void *value1, void *value2)
{
    int target_bucket, i, overflow_pointer, equal = 0;
    unsigned char *block_offset, block[BLOCKSIZE];
    char overflow_pch[5];

    if (open_files[fileDesc] == NULL) {
        HTE_errno = HTE_WRONG_FILEDESC;
        return HTE_WRONG_FILEDESC;
    }

    /*
       Pername to pedio-kleidi sthn Hashing Function gia na mas epistrepsei se
       poio bucket prepei na steiloume thn eggrafh
    */
    if ((target_bucket = hash_funct(value1, open_files[fileDesc]->pfheader->attrType1, open_files[fileDesc]->pfheader->buckets)) < 0) {
        HTE_errno = HTE_WRONG_KEY;
        return HTE_WRONG_KEY;
    }

    /*
       Prospername to header-block (BLOCKSIZE) kai phgainoume sthn arxh tou
       epilegmenou bucket ( + target_bucket*BLOCKSIZE )
    */
    fseek(open_files[fileDesc]->fpointer, BLOCKSIZE + target_bucket * BLOCKSIZE, SEEK_SET);

    do {
        /* Fortwnoume olo to block sth mnhmh */
        if (fread(block, 1, BLOCKSIZE, open_files[fileDesc]->fpointer) != BLOCKSIZE) {
            HTE_errno = HTE_FREAD_ERROR;
            return HTE_FREAD_ERROR;
        }

        /* Elegxoume ap'thn arxh ola ta bits tou bitmap gia energh 8esh */

        for (i = 0; i < open_files[fileDesc]->pfheader->total_records ; i++) {
            /*
                * To i/8 antistoixei sto i-osto byte tou bitmap
                * to i%8 mas dinei to bit ths antistoixhs eggrafhs
            */
            if (BIT_IsSet(block[i/8], i % 8)) {
                /* 
                   * theloyme ton char pointer block_offset na deixnei sthn arxh ths eggrafhs toy block kathe fora poy 
                    theloyme na eksetasoyme.
                   * h apostash apo tin arxh tou block isoutai me to megethos se bytes tou bitmap kai tou overflow 
                    pointer pou vriskontai stin arxh kathe block kathws kai i fores to standar megethos (twn 2 pediwn) 
                    mias eggrafis.
                */
                block_offset = block +
                    open_files[fileDesc]->pfheader->bitmap_size_in_bytes + 4 +
                    i * (open_files[fileDesc]->pfheader->attrLength1 +
                    open_files[fileDesc]->pfheader->attrLength2);

                /*
                    An bre8ei energh 8esh eggrafhs, elegxoume to kleidi ths
                    Analoga me ton typo twn pediwn, kanoume antistoixh sygkrish,
                */
                equal = 0;

                if (open_files[fileDesc]->pfheader->attrType1 == 'c') {
                    if (strncmp((char*)block_offset, value1, open_files[fileDesc]->pfheader->attrLength1) == 0) {
                        /* Brhkame to kleidi, kratame flag */
                        equal = 1;
                    }
                } 
                else if (open_files[fileDesc]->pfheader->attrType1 == 'i' || open_files[fileDesc]->pfheader->attrType1 == 'f') {

                    /* An broume to antistoixo kleidi, tote equal=1; */
                    if (memcmp(block_offset, value1, 4) == 0) {
                        /* Kratame flag oti bre8hke to kleidi */
                        equal = 1;
                    }
                }

                /* An bre8hke antistoixo kleidi, elegxoume to 2o pedio ths eggrafhs */
                if (equal == 1) {
                    /* An kai to 2o peidio ths eggrafis einai idio tote gemizoyme tis antistoixes theseis sto block me midenika */
                    if (open_files[fileDesc]->pfheader->attrType2 == 'c') {
                        if (strncmp((char*)block_offset + open_files[fileDesc]->pfheader->attrLength1, value2, open_files[fileDesc]->pfheader->attrLength1) == 0) {
                            BIT_Flip(&block[i/8], i % 8);
                            memset(block_offset, 0, open_files[fileDesc]->pfheader->attrLength1 + open_files[fileDesc]->pfheader->attrLength2);
                        }
                    } 
                    else if (open_files[fileDesc]->pfheader->attrType2 == 'i' || open_files[fileDesc]->pfheader->attrType2 == 'f') {                       
                        if (memcmp(block_offset + open_files[fileDesc]->pfheader->attrLength1, value2, 4) == 0) {
                            BIT_Flip(&block[i/8], i % 8);
                            memset(block_offset, 0, open_files[fileDesc]->pfheader->attrLength1 + open_files[fileDesc]->pfheader->attrLength2);
                        }
                    }
                }
            }
        } /* for */

        /* Fortwnoume sth mnhmh ton "pointer" poy deixnei sto overflow bucket */
        strncpy(overflow_pch, (char*)block + open_files[fileDesc]->pfheader->bitmap_size_in_bytes, 4);

        overflow_pch[4] = '\0';

        overflow_pointer = atoi(overflow_pch);

        /* An o "pointer" exei timh diaforh tou mhdenos, tote deixnei se
           overflow bucket, sto opoio tha pame kai tha synexisoume ton elegxo */
        fseek(open_files[fileDesc]->fpointer, -(BLOCKSIZE), SEEK_CUR);

        /* Ksanagrafoyme to allagmeno block poy molis eksetasame sto arxeio */
        fwrite(block, 1, BLOCKSIZE, open_files[fileDesc]->fpointer);

        if (overflow_pointer != 0) {
            fseek(open_files[fileDesc]->fpointer, overflow_pointer - BLOCKSIZE, SEEK_CUR);
        }

    } while (overflow_pointer != 0);

    return HTE_OK;
}


/**** int HT_OpenIndexScan(int fileDesc, int op, void *value) ****

 Input:
       *   int  fileDesc         αριθμός που αντιστοιχεί στο ανοιχτό αρχείο
       *   int  op               τελεστής σύγκρισης
       *   void *value           τιμή του πεδίου-κλειδιού προς σύγκριση

 Operation:
           * Anoigei mia sarwsh sto arxeio fileDesc
           * kai pros8etei mia eggrafh (< MAXSCANS) ston pinaka sarwsewn
           * An op == 1 -> EQUAL
                op == 2 -> NOT EQUAL
 Output:
        * None

 Return Value:
        * An epityxes: Epistrefei thn timh ap'ton pinaka sarwsewn pou antistoixei sthn eggrafh
        * Alliws: antistoixo kwdiko la8ous
 Comment:
         * H entolh
                    open_scans[i]->key = value;
           douleuei, epeidh h eswterikh metablhth value deixnei se dieu8unsh
           metablhths pou yparxei sth main, opote tha yparxei ka8'olh th
           leitourgia tou programmatos!
*/

int HT_OpenIndexScan(int fileDesc, int op, void *value)
{
    int i;
    char key_type;

    /* Psaxnoume na broume kenh 8esh ston pinaka anoixtwn sarwsewn */

    for (i = 0 ; i < MAXSCANS ; i++) {
        if (open_scans[i] == NULL) {
            break;
        }
    }

    /* O iterator kseperase to plh8os twn sarwsewn, opote den brhkame kenh 8esh */
    if (i == MAXSCANS) {
        HTE_errno = HTE_WRONG_SCANDESC;
        return HTE_WRONG_SCANDESC;
    } 
    else {
        if ((open_scans[i] = malloc(sizeof(struct scan))) == NULL) {
            HTE_errno = HTE_MEM_ERROR;
            return HTE_MEM_ERROR;
        }

        /* to apo8hkeuoume proswrina giati xrhsimopoieitai polles fores */
        key_type = open_files[fileDesc]->pfheader->attrType1;

        /*
           Pername to pedio-kleidi sthn Hashing Function gia na mas epistrepsei se
           poio bucket prepei na steiloume thn eggrafh
        */
        if (op == EQUAL) {
            if ((open_scans[i]->bucket = hash_funct(value, key_type, open_files[fileDesc]->pfheader->buckets)) < 0) {
                HTE_errno = HTE_WRONG_KEY;
                return HTE_WRONG_KEY;
            }
        } 
        else if (op == NOT_EQUAL) { /* an o op einai NOT_EQUAL tote tha arxisoyme tin sarwsh apo to 1o bucket toy arxeioy */
            open_scans[i] -> bucket = 0;
        }
        else
        {
            HTE_errno = HTE_WRONG_OP;
            return HTE_WRONG_OP;
        }

        /* Tautoxrona, 8etoume ws arxikh eggrafh thn prwth */
        open_scans[i]->record = 0;

        /* Swzoume tis var "op" kai "fileDesc" ston pinaka anoixtwn sarwsewn */
        open_scans[i]->op = op;
        open_scans[i]->fileDesc = fileDesc;
        open_scans[i]->key = value;

        if ((open_scans[i]->returnval = malloc(sizeof(union Value2))) == NULL) {
            HTE_errno = HTE_MEM_ERROR;
            return HTE_MEM_ERROR;
        }

        open_scans[i]->returnval->cval = NULL;
    }

    return i;
}


/**** void *HT_FindNextEntry(int scanDesc) ****

 Input:
       *   int scanDesc          αριθμός που αντιστοιχεί στην ανοιχτή σάρωση

 Operation:
           * Epistrefei thn timh tou 2ou pediou (value2)
             efoson ikanopoiei thn sun8hkh pou exoume swsei ston
             pinaka sarwsewn, sth 8esh scanDesc

 Output:
        * None

 Return Value:
        * An bre8ei: Thn timh tou 2ou pediou
        * An de bre8ei (kai dhladh "ksemeinoume" apo eggrafes): HTE_EOF
*/

void *HT_FindNextEntry(int scanDesc)
{
    int tmprecord, tmpbucket, tmpfileDesc, overflow_pointer, i , found;
    char overflow_pch[5];
    unsigned char *block_offset, block[BLOCKSIZE];
    found = 0;

    if (open_scans[scanDesc] == NULL) {
        HTE_errno = HTE_WRONG_SCANDESC;
        return (void *)HTE_WRONG_SCANDESC;
    }


    /* Arxikopoihsh twn proswrinwn metablhtwn */
    tmpfileDesc = open_scans[scanDesc]->fileDesc;
    tmprecord = open_scans[scanDesc]->record;
    tmpbucket = open_scans[scanDesc]->bucket;

    if (open_files[tmpfileDesc] == NULL) {
        HTE_errno = HTE_WRONG_FILEDESC;
        return (void *) HTE_WRONG_FILEDESC; 
    }

    /* Fortwma tou arxeiou ston fpointer */
    fseek(open_files[tmpfileDesc]->fpointer, BLOCKSIZE + tmpbucket * BLOCKSIZE, SEEK_SET);

    /* Analoga an o op einai equal h not_equal kanoyme antistoixes sarwseis */
    
    if (open_scans[scanDesc]->op == EQUAL) {
        do {
            /* Fortwnoume olo to block sth mnhmh */
            if (fread(block, 1, BLOCKSIZE, open_files[tmpfileDesc]->fpointer) != BLOCKSIZE) {
                HTE_errno = HTE_FREAD_ERROR;
                return (void*)HTE_FREAD_ERROR;
            }

            /* Elegxoume ap'thn arxh ola ta bits tou bitmap gia energh 8esh */
            
            for (i = tmprecord; i < open_files[tmpfileDesc]->pfheader->total_records; i++) {
                /*
                    * To i/8 antistoixei sto i-osto byte tou bitmap
                    * to i%8 mas dinei to bit ths antistoixhs eggrafhs
                */
                if (BIT_IsSet(block[i/8], i % 8)) {
                /* 
                   * theloyme ton char pointer block_offset na deixnei sthn arxh ths eggrafhs toy block kathe fora poy 
                    theloyme na eksetasoyme.
                   * h apostash apo tin arxh tou block isoutai me to megethos se bytes tou bitmap kai tou overflow 
                    pointer pou vriskontai stin arxh kathe block kathws kai i fores to standar megethos (twn 2 pediwn) 
                    mias eggrafis.
                */
                    block_offset = block + open_files[tmpfileDesc]->pfheader->bitmap_size_in_bytes + 4 +
                                   i * (open_files[tmpfileDesc]->pfheader->attrLength1 + open_files[tmpfileDesc]->pfheader->attrLength2);

                    /* elegxoyme ta kleidia kanontas antistoixh sygkrish analoga ton typo toy kleidioy */
                    if (open_files[tmpfileDesc]->pfheader->attrType1 == 'c') {
                        if (strncmp((char*)block_offset, open_scans[scanDesc]->key, open_files[tmpfileDesc]->pfheader->attrLength1) == 0) {
                            /* An broume to antistoixo kleidi, tote found=1; */
                            found = 1;
                            break; /* Tha bgoume ap'to for, afou brhkame eggrafh */
                        }
                    }

                    else if (open_files[tmpfileDesc]->pfheader->attrType1 == 'i' || open_files[tmpfileDesc]->pfheader->attrType1 == 'f') {

                        /* An broume to antistoixo kleidi, tote found=1; */
                        if (memcmp(block_offset, open_scans[scanDesc]->key, 4) == 0) {
                            found = 1;
                            break; /* Tha bgoume ap'to for, afou brhkame eggrafh */
                        }
                    }

                }
            }

            /* An den exei vrethei oute mia eggrafi s olo to block pou na ikanopoiei tis synthikes mas */
            if (found == 0) {

                /* Fortwnoume sth mnhmh ton "pointer" poy deixnei sto overflow bucket */
                strncpy(overflow_pch, (char*)block + open_files[tmpfileDesc]->pfheader->bitmap_size_in_bytes, 4);
                overflow_pch[4] = '\0';
                overflow_pointer = atoi(overflow_pch);


                if (overflow_pointer != 0) {
                    /*
                       An o "pointer" exei timh diaforh tou mhdenos, tote deixnei se
                       overflow bucket, sto opoio tha pame kai tha synexisoume ekei ton elegxo
                    */
                    fseek(open_files[tmpfileDesc]->fpointer, overflow_pointer - BLOCKSIZE, SEEK_CUR);
                    tmpbucket += (overflow_pointer / BLOCKSIZE);
                    tmprecord = 0;
                } 
                else { 
                    /* alliws den yparxoyn alles eggrafes sto arxeio pou na ikanopoioun ti synthiki mas */ 
                    /* apothikeyoyme ton teleytaio bucket poy eleksame kai thetoyme ws teleytaio record
                       ena mh egkyro-energo record etsi wste an ksanakalestei h synarthsh na termatisei sxedon amesws */
                    open_scans[scanDesc]->record = open_files[tmpfileDesc]->pfheader->total_records;
                    open_scans[scanDesc]->bucket = tmpbucket;
                    HTE_errno = HTE_EOF;
                    return (void *)HTE_EOF;
                }
            }
        } while (found == 0);
    }



    else if (open_scans[scanDesc]->op == NOT_EQUAL) {
        do {
            /* Fortwnoume olo to block sth mnhmh */
            if (fread(block, 1, BLOCKSIZE, open_files[tmpfileDesc]->fpointer) != BLOCKSIZE) {
                if (feof(open_files[tmpfileDesc]->fpointer)) { 
                    /* an ftasame sto telos tou arxeiou tote orizoyme ws teleytaio bucket poy eleksame ton teleytaio bucket tou 
                       arxeiou kai ws teleytaio record ena mh egkyro-energo record etsi wste an ksanakalestei h synarthsh na
                       termatisei sxedon amesws */
                    open_scans[scanDesc]->record = open_files[tmpfileDesc]->pfheader->total_records;
                    open_scans[scanDesc]->bucket = tmpbucket - 1;
                    HTE_errno = HTE_EOF;
                    return (void *)HTE_EOF;
                }

                /* An den oloklhrwse swsta to diabasma h fread tha ektelestei auto to kommati */
                HTE_errno = HTE_FREAD_ERROR;
                return (void*)HTE_FREAD_ERROR;
            }

            /* Elegxoume ap'thn arxh ola ta bits tou bitmap gia energh 8esh */
            
            for (i = tmprecord; i < open_files[tmpfileDesc]->pfheader->total_records; i++) {
                /*
                    * To i/8 antistoixei sto i-osto byte tou bitmap
                    * to i%8 mas dinei to bit ths antistoixhs eggrafhs
                */
                if (BIT_IsSet(block[i/8], i % 8)) {
                /* 
                   * theloyme ton char pointer block_offset na deixnei sthn arxh ths eggrafhs toy block kathe fora poy 
                    theloyme na eksetasoyme.
                   * h apostash apo tin arxh tou block isoutai me to megethos se bytes tou bitmap kai tou overflow 
                    pointer pou vriskontai stin arxh kathe block kathws kai i fores to standar megethos (twn 2 pediwn) 
                    mias eggrafis.
                */
                    block_offset = block + open_files[tmpfileDesc]->pfheader->bitmap_size_in_bytes + 4 + i * (open_files[tmpfileDesc]->pfheader->attrLength1 + open_files[tmpfileDesc]->pfheader->attrLength2);

                    /* elegxoyme ta kleidia kanontas antistoixh sygkrish analoga ton typo toy kleidioy */
                    if (open_files[tmpfileDesc]->pfheader->attrType1 == 'c') {
                        if (strncmp((char*)block_offset, open_scans[scanDesc]->key, open_files[tmpfileDesc]->pfheader->attrLength1) != 0) {
                            /* An broume to antistoixo kleidi, tote found=1; */
                            found = 1;
                            break; /* Tha bgoume ap'to for, afou brhkame eggrafh */
                        }
                    }

                    else if (open_files[tmpfileDesc]->pfheader->attrType1 == 'i' || open_files[tmpfileDesc]->pfheader->attrType1 == 'f') {
                        /* An broume to antistoixo kleidi, tote found=1; */
                        if (memcmp(block_offset, open_scans[scanDesc]->key, 4) != 0) {
                            found = 1;
                            break; /* Tha bgoume ap'to for, afou brhkame eggrafh */
                        }
                    }
                }
            }

            /* An den exei vrethei oute mia eggrafi s olo to block pou na ikanopoiei tis synthikes mas */
            if (found == 0) {
                tmpbucket++; /* synexizoyme sto amesws epomeno bucket tou arxeiou */
                tmprecord = 0;
            }
        } while (found == 0);
    }


    open_scans[scanDesc]->record = i + 1; /* stin epomenh sarwsh th arxisoyme apo tin amesws epomenh eggrafh */
    open_scans[scanDesc]->bucket = tmpbucket; /* kai apo to teleytaio bucket pou eleksame */

    /* apothikeyoyme tin timh toy deyteroy pediou sto struct tis sarwshs (heap memory) gia na mh xathei 
       otan termatistei h synarthsh k etsi na mporei o void * poy epistrefei h synarthsh na deiksei ekei */
    
    /* kanoyme antistoixh apothikeysh kai epistrofh timhs analoga ton typo toy 2oy pedioy */
    if (open_files[tmpfileDesc]->pfheader->attrType2 == 'c') {

        if ((open_scans[scanDesc]->returnval->cval = malloc(open_files[tmpfileDesc]->pfheader->attrLength2)) == NULL) {
            HTE_errno = HTE_MEM_ERROR;
            return (void *)HTE_MEM_ERROR;
        }

        strncpy(open_scans[scanDesc]->returnval->cval, (char *)block_offset + open_files[tmpfileDesc]->pfheader->attrLength1, open_files[tmpfileDesc]->pfheader->attrLength2);
        return open_scans[scanDesc]->returnval->cval;
    } 
    else if (open_files[tmpfileDesc]->pfheader->attrType2 == 'i') {
        memcpy(open_scans[scanDesc]->returnval, (char *)block_offset + open_files[tmpfileDesc]->pfheader->attrLength1, 4);
        return &(open_scans[scanDesc]->returnval->dval);

    } 
    else if (open_files[tmpfileDesc]->pfheader->attrType2 == 'f') {
        memcpy(open_scans[scanDesc]->returnval, (char *)block_offset + open_files[tmpfileDesc]->pfheader->attrLength1, 4);
        return &(open_scans[scanDesc]->returnval->fval);
    } 
    else {
        HTE_errno = HTE_EOF;
        return (void *)HTE_EOF;
    }


}


/**** int HT_CloseIndexScan(int scanDesc) ****

 Input:
       *   int scanDesc        αριθμός που αντιστοιχεί στην ανοιχτή σάρωση


 Operation:
           * Kleinei thn sarwsh scanDesc ap'ton pinaka sarwsewn
           * Sbhnei thn antistoixh eggrafh ap'ton pinaka sarwsewn

 Output:
        * None

 Return Value:
        * An epityxes: HTE_OK
        * Alliws: antistoixo kwdiko la8ous
*/

int HT_CloseIndexScan(int scanDesc)
{
    if (open_scans[scanDesc] != NULL) {
        if (open_files[(open_scans[scanDesc]->fileDesc)]->pfheader->attrType2 == 'c') {
            if (open_scans[scanDesc]->returnval->cval != NULL) { /* an exoyme dynamika desmeysh mnhmh gia kapoio string */
                free(open_scans[scanDesc]->returnval->cval);
            }
        }

        free(open_scans[scanDesc]->returnval);

        free(open_scans[scanDesc]);
        open_scans[scanDesc] = NULL;
    } 
    else {
        HTE_errno = HTE_WRONG_SCANDESC;
        return HTE_WRONG_SCANDESC;
    }

    return HTE_OK;
}


/**** void HT_PrintError(char *errString) ****

 Input:
       * char *errString       κείμενο για εκτύπωση

 Operation:
           * Typwnei to mhnyma ths parametrou errString
             kai meta typwnei to teleutaio mhnyma sfalmatos,
             analoga me thn timh ths global var HT_errno
 Output:
        * Ta antistoixa mhnymata (see "Operation" above)

 Return Value:
        * None
*/

void HT_PrintError(char *errString)
{
    printf("%s: %s\n", errString, error_msg[-(HTE_errno+1)]);
}


/**** int BIT_IsSet(char *ch, char pos) ****

 Input:
       * char *ch               ari8mos pros meleth
       * char pos               8esh tou bit pou elegxoume (se sxesh me to LSB)

 Operation:
           * Elegxei an to "pos"-osto bit tou ari8mou "ch" xrhsimopoieitai
 Output:
        * None

 Return Value:
        * An to "pos"-osto bit tou ari8mou "ch" exei timh 1, epistrefei 1
        * Alliws to bit exei timh 0 kai h sunarthsh epistrefei 0
*/

unsigned char BIT_IsSet(char ch, char pos)
{
    return (ch & (1UL << pos));
}


/**** void BIT_Flip(char *ch, char pos) ****

 Input:
       * char *ch               ari8mos pros allagh
       * char pos               8esh (apo deksia) tou bit pou tha allaksoume

 Operation:
           * Allazei thn timh tou "pos"-ostou bit tou ari8mou "ch"
             sth symplhrwmatikh ths
 Output:
        * None

 Return Value:
        * An h synarthsh leitourghse epityxws, epistrefei 1
        * Alliws, epistrefei 0
*/

int BIT_Flip(unsigned char *ch, char pos)
{
    if ((pos < 8) && (pos >= 0)) {
        *ch ^= (1UL << pos);
        return 1;
    } 
    else
    {
        return 0;
    }
}


/**** int hash_funct(void * key, char type) ****

 Input:
       * char *ch               ari8mos pros allagh
       * char pos               8esh (apo deksia) tou bit pou tha allaksoume

 Operation:
           * Allazei thn timh tou "pos"-ostou bit tou ari8mou "ch"
             sth symplhrwmatikh ths
 Output:
        * None

 Return Value:
        * An h synarthsh leitourghse epityxws, epistrefei 1
        * Alliws, epistrefei 0
*/

int hash_funct(void *key, char type, int buckets)
{
    unsigned long hash;

    if (type == 'i' || type == 'f') {
        /* Credits to Robert Jenkins */
        hash = *(int*)key;
        hash = (hash + 0x7ed55d16) + (hash << 12);
        hash = (hash ^ 0xc761c23c) ^(hash >> 19);
        hash = (hash + 0x165667b1) + (hash << 5);
        hash = (hash + 0xd3a2646c) ^(hash << 9);
        hash = (hash + 0xfd7046c5) + (hash << 3);
        hash = (hash ^ 0xb55a4f09) ^(hash >> 16);
    }

    /* De xrhsimopoioume diaforetikh synarthsh gia tous floats, mias kai de brhkame
       antistoixh hashing function. Mia pi8anh lysh tha htan na efarmosouse mia
       hashing function sto akeraio KAI sto dekadiko meros tou pragmatikou kai
       meta na syndyasoume ta hashes mesw ari8mhtikhs prakshs */

    else if (type == 'c') {
        /* Xrhsh tou DJB Hashing algorithm, credits to Dr. Daniel J. Bernstein */
        hash = 5381;

        /*
           * Afou exoume void*, yparxei eks orismou problhma me ta pointer arithmetics
           Gi'auto, kanoume casting se char*, gia na deixnoume se sygkekrimenh
           dieu8unsh mnhmhs, kai ton auksanoume kata to mege8os enos char,
           epomenos tha deixnei se dieu8unsh kata ena byte deksia.
           * Ousiastika, to *(char*) deixnei enan xarakthra tou string-kleidiou!
        */

        for ( ; *(char*)key != '\0' ; key = (char*)key + sizeof(char))
        {
            hash = ((hash << 5) + hash) + *(char*)key;
        }
    } 
    else {
        HTE_errno = HTE_WRONG_KEY;
        return HTE_WRONG_KEY;
    }

    return (int)(hash % buckets);
}
