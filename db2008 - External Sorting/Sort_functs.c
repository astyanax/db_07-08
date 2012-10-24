/*
  std06066 Anastasiou Basileios-Marios
  std06101 Karatzas Eystratios
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Sort.h"
                 
struct file_header * header_data;

struct file_header {
    char attrType1;
    int  attrLength1;
    char attrType2;
    int  attrLength2;
    int  total_records_per_block;
    int  total_records_in_file;
    int  bitmap_size_in_bytes;
};

int Sort_errno;

char error_msg[][128] = {
    "Reached end of file",
    "Memory allocation error",
    "Error while opening file",
    "Error while creating file",
    "Error while closing file",
    "fwrite() didn't write as many bytes as it should",
    "fread() didn't read as many bytes as it should",
    "Error while removing file",
    "Wrong cell attributes provided",
    "Wrong buffsize error",
};

/******************************************************************************/
/*                               FUNCTIONS                                    */
/******************************************************************************/


/**** void Sort_Init(void) ****

 Input:
       * None

 Operation:
           * Arxikopoiei tis ka8olikes domes dedomenwn
 Output:
        * None

 Return Value:
        * None

 Comments:
          ---
*/

void Sort_Init(void)
{
    header_data = NULL;
}


/**** int Sort_Create_File(char*, char, int, char, int) ****

 Input:
       * char   *fileName        όνομα αρχείου που θα δημιουργηθεί
       * char   attrType1        τύπος πρώτου πεδίου-κλειδιού: 'c' (σειρά χαρακτήρων), 'i' (ακέραιος), 'f' (πραγματικός)
       * int    attrLength1      μήκος πρώτου πεδίου-κλειδιού: 4 γιά 'i' ή 'f', 1-255 γιά 'c'
       * char   attrType2        τύπος δεύτερου πεδίου: 'c' (σειρά χαρακτήρων), 'i' (ακέραιος), 'f' (πραγματικός)
       * int    attrLength2      μήκος δεύτερου πεδίου: 4 γιά 'i' ή 'f', 1-255 γιά 'c'

 Operation:
           * Dhmiourgei to arxeio me onoma fileName
           * O typos kai to mhkos ka8enos ap'ta 2 pedia didetai sta args 2-3, 4-5 antistoixa
           * Arxikopoiei to antistoixo struct me tis plhrofories toy arxeioy
           * Kai arxikopoiei to arxeio prosthetontas ena arxiko header-block me tis aparaitites plhrofories
 Output:
        * Ena arxikopoimeno file

 Return Value:
        * An epityxes: OK
        * Alliws: antistoixo kwdiko la8ous
*/


int Sort_Create_File(char *filename, char attrtype1, int attrlength1, char attrtype2, int attrlength2)
{
    FILE * fp;
    struct file_header *fhptr;
    char buffer[BLOCKSIZE];
    
    if ( (fp = fopen(filename, "wb")) == NULL) {
       Sort_errno = CREATE_FILE_ERROR;
       return CREATE_FILE_ERROR;
    }

    if ( attrtype1 != 'c' && attrtype1 != 'f' && attrtype1 != 'i'){
       Sort_errno = ATTR_ERROR;
       return ATTR_ERROR;
    }

    if ( attrtype2 != 'c' && attrtype2 != 'f' && attrtype2 != 'i'){
       Sort_errno = ATTR_ERROR;
       return ATTR_ERROR;
    }

    if ( (fhptr = malloc(sizeof(struct file_header))) == NULL){
       Sort_errno = MEM_ERROR;
       return MEM_ERROR;
    }

    fhptr->attrType1 = attrtype1;
    fhptr->attrType2 = attrtype2;
    fhptr->attrLength1 = attrlength1;
    fhptr->attrLength2 = attrlength2;
    fhptr->total_records_per_block =  (float) BLOCKSIZE  / (float)(0.125 + attrlength1 + attrlength2);
    fhptr->bitmap_size_in_bytes = fhptr->total_records_per_block / 8;
    fhptr->total_records_in_file=0;

    /* Idia leitourgia me thn ceiling() */
    if ((float) fhptr->total_records_per_block / (float) 8  > fhptr->bitmap_size_in_bytes) {
        fhptr->bitmap_size_in_bytes++;
    }

    /* Arxikopoihsh tou buffer kai antigrafh tou file_header */
    memset(buffer, 0 , BLOCKSIZE);
    memcpy(buffer, fhptr, sizeof(struct file_header));
    
    /* grafoyme to header sto arxeio */
    if (fwrite(buffer, 1, BLOCKSIZE, fp) != BLOCKSIZE){
       return FWRITE_ERROR;
    }

    free(fhptr);
    if ( fclose(fp) == EOF) {
       Sort_errno = CLOSE_FILE_ERROR;
       return CLOSE_FILE_ERROR;
    }
    
    return OK;
}

/**** int Sort_Destroy_File(char *fileName) ****

 Input:
       * char *fileName          όνομα αρχείου

 Operation:
           * Katastrefei to arxeio me onoma fileName, svhnontas ola ta dedomena toy.

 Output:
        * None

 Return Value:
        * An epityxes: Epistrefei to plh8os twn eggrafwn tou arxeiou
        * Alliws: antistoixo kwdiko la8ous
*/

int Sort_Destroy_File(char *fileName)
{
    FILE * fp;
    struct file_header * fhptr;
    int recs;
    
    if ( (fp = fopen(fileName, "rb")) == NULL) {
       Sort_errno = OPEN_FILE_ERROR;
       return OPEN_FILE_ERROR;
    }
    
    if ( (fhptr = malloc(sizeof(struct file_header)) ) == NULL){
       Sort_errno = MEM_ERROR;
       return MEM_ERROR;
    }
    
    /* fortwnoyme to header block toy arxeioy sth mnhmh */
    if (fread(fhptr , sizeof(struct file_header), 1, fp) != 1){
       Sort_errno = FREAD_ERROR;
       return FREAD_ERROR;
    }
    
    /* oi synolikes energes eggrafes toy arxeioy katagrafontai sto header */
    
    recs = fhptr->total_records_in_file;
    free(fhptr);
    
    if ( fclose(fp) == EOF) {
       Sort_errno = CLOSE_FILE_ERROR;
       return CLOSE_FILE_ERROR;
    }
    
    /* diagrafh toy arxeioy */
    if (remove(fileName) != 0) {
        Sort_errno = REM_FILE;
        return REM_FILE;
    }
    else {
        return recs;
    }
}

/**** int Sort_Print(char *fileName) ****

 Input:
       * char *fileName      αρχείο του οποίου τις εγγραφές θα τυπώσουμε

 Operation:
           * Typwnei tis pleiades tou arxeiou fileName, mia ana grammh
             
 Output:
        * Tis pleiades, mia ana grammh

 Return Value:
        * An epityxei, to plh8os twn eggrafwn sto arxeio
        * Alliws, kwdiko la8ous
*/
int Sort_Print(char *fileName)
{
    FILE * fp;
    struct file_header *fhptr;
    int recs, i, dvalue;
    float fvalue;
    char buffer[BLOCKSIZE], *buffer_offset;
    
    if ( (fp = fopen(fileName, "rb")) == NULL) {
       Sort_errno = OPEN_FILE_ERROR;
       return OPEN_FILE_ERROR;
    }
    
    if ( (fhptr = malloc(sizeof(struct file_header)) ) == NULL){
       Sort_errno = MEM_ERROR;
       return MEM_ERROR;
    }
    
    /* fortwnoyme to header-block sth mnhmh */
    if (fread(fhptr , sizeof(struct file_header), 1, fp) != 1){
       Sort_errno = FREAD_ERROR;
       return FREAD_ERROR;
    }
    
    recs = fhptr->total_records_in_file;

    /* Pame sto 1o block me eggrafes... */
    fseek(fp, BLOCKSIZE, SEEK_SET);

    /* ...kai to diabazoume */
    
    if ( fread(buffer, 1, BLOCKSIZE, fp) != BLOCKSIZE ){
       Sort_errno = FREAD_ERROR;
       return FREAD_ERROR;
    }

    while (!feof(fp))
    {
          for (i=0; i < fhptr->total_records_per_block; i++)
          {
              /* An bre8ei energh 8esh sto bitmap, ektypwnoume thn eggrafh */
              if (BIT_IsSet(buffer[i/8], i % 8))
              {
                 /* Prwta pame sto prwto pedio kai to ektypwnoume */
                 buffer_offset = buffer + fhptr->bitmap_size_in_bytes + i * (fhptr->attrLength1 + fhptr->attrLength2);
                 
                 if (fhptr->attrType1 == 'c'){
                    printf("%s", buffer_offset);
                 }
                 else if (fhptr->attrType1 == 'i'){
                    memcpy(&dvalue, buffer_offset, sizeof(int));
                    printf("%d", dvalue);
                 }
                 else if (fhptr->attrType1 == 'f'){
                    memcpy(&fvalue, buffer_offset, sizeof(float));
                    printf("%f", fvalue);
                 }
                 else{
                    Sort_errno = ATTR_ERROR;
                    return ATTR_ERROR;
                 }

                 /* Kai meta sto 2o */
                 buffer_offset += fhptr->attrLength1;
                 
                 if (fhptr->attrType2 == 'c'){
                    printf(" %s", buffer_offset);
                 }
                 else if (fhptr->attrType2 == 'i'){
                    memcpy(&dvalue, buffer_offset, sizeof(int));
                    printf(" %d", dvalue);
                 }
                 else if (fhptr->attrType2 == 'f'){
                    memcpy(&fvalue, buffer_offset, sizeof(float));
                    printf(" %f", fvalue);
                 }
                 else{
                    Sort_errno = ATTR_ERROR;
                    return ATTR_ERROR;
                 }
                 
                 printf("\n");
              }
          }
         
          /* Diabazoume to epomeno block */
          if ( fread(buffer, 1, BLOCKSIZE, fp) != BLOCKSIZE && !feof(fp)){
             Sort_errno = FREAD_ERROR;
             return FREAD_ERROR;
          }
    }
    
    free(fhptr);
    if ( fclose(fp) == EOF) {
       Sort_errno = CLOSE_FILE_ERROR;
       return CLOSE_FILE_ERROR;
    }
    
    return recs;
}


/**** int Sort_InsertEntry(char *filename, void *value1, void *value2) ****

 Input:
       * char *filename         To onoma tou arxeiou ap'to opoio tha diabasoume
       * void *value1           timh pediou-kleidiou pou eisagoume
       * void *value2           timh tou antistoixou 2ou pediou

 Operation:
           * Anoigoume to arxeio kai eisagoume sth prwth kenh 8esh thn
             pleiada (value1, value2)
           * An den yparxei kenh 8esh, pros8etoume ena block sto arxeio
             kai grafoume.
             
 Output:
        * None

 Return Value:
        * An petyxei, epistrefei OK
        * Alliws, kwdiko la8ous
*/

int Sort_InsertEntry(char *filename, void *value1, void *value2)
{
    FILE * fp;
    struct file_header *fhptr;
    int i;
    unsigned char buffer[BLOCKSIZE], *buffer_offset;

    if ( (fp = fopen(filename, "rb+")) == NULL) {
       Sort_errno = OPEN_FILE_ERROR;
       return OPEN_FILE_ERROR;
    }

    if ( (fhptr = malloc(sizeof(struct file_header)) ) == NULL){
       Sort_errno = MEM_ERROR;
       return MEM_ERROR;
    }

    /* fortwnoyme to header-block sth mnhmh */
    if ( fread(fhptr , sizeof(struct file_header), 1, fp) != 1 ){
       Sort_errno = FREAD_ERROR;
       return FREAD_ERROR;
    }
  
    fhptr->total_records_in_file ++; /* topothetoyme mia nea eggrafh sto block k ananewnoyme ton antistoixo metrhth */
    rewind(fp);
    
    /* Eisagoume to ananewmeno header block sto arxeio */
    {
       char block[BLOCKSIZE];
       memset(block, 0, BLOCKSIZE);
       memcpy(block, fhptr, sizeof(struct file_header));
       if (fwrite(block, 1, BLOCKSIZE, fp) != BLOCKSIZE) {
          Sort_errno = FWRITE_ERROR;
          return FWRITE_ERROR;
       }
    }

    /* Pame sto 1o block me eggrafes */
    fseek(fp, BLOCKSIZE, SEEK_SET);
    
    if (fread(buffer, 1, BLOCKSIZE, fp) != BLOCKSIZE && !feof(fp)) {
       Sort_errno = FREAD_ERROR;
       return FREAD_ERROR;
    }
    
    while (!feof(fp))
    {
          for (i=0; i < fhptr->total_records_per_block; i++)
          {
              /* Psaxnoume gia kenh 8esh sto bitmap */
              if (!BIT_IsSet((char)buffer[i/8], i % 8))
              {
                 buffer_offset = buffer + fhptr->bitmap_size_in_bytes + i * (fhptr->attrLength1 + fhptr->attrLength2);
                 
                 /* Desmeuoume 8esh sto bitmap */
                 BIT_Flip(&buffer[i/8], i % 8);
                 
                 /* An bre8ei energh 8esh sto bitmap, eisagoume thn eggrafh */
                 if (fhptr->attrType1 == 'c'){
                    strncpy((char*)buffer_offset, (char*)value1, fhptr->attrLength1 );
                 }
                 else if ( (fhptr->attrType1 == 'i') || (fhptr->attrType1 == 'f')){
                    memcpy(buffer_offset, value1, fhptr->attrLength1);
                 }
                 else{
                    Sort_errno = ATTR_ERROR;
                    return ATTR_ERROR;
                 }

                 buffer_offset += fhptr->attrLength1;

                 if (fhptr->attrType2 == 'c'){
                   strncpy((char*)buffer_offset, (char*)value2, fhptr->attrLength2 );
                 }
                 else if ( (fhptr->attrType2 == 'i') || (fhptr->attrType2 == 'f')){
                    memcpy(buffer_offset, value2, fhptr->attrLength2);
                 }
                 else{
                    Sort_errno = ATTR_ERROR;
                    return ATTR_ERROR;
                 }

                 /* Metaferoume to ananewmeno block ap'th mnhmh sto arxeio */
                 fseek(fp, -BLOCKSIZE, SEEK_CUR);
                 if (fwrite(buffer, BLOCKSIZE, 1, fp) != 1) {
                    Sort_errno = FWRITE_ERROR;
                    return FWRITE_ERROR;
                 }

                 free(fhptr);
                 if ( fclose(fp) == EOF) {
                    Sort_errno = CLOSE_FILE_ERROR;
                    return CLOSE_FILE_ERROR;
                 }

                 return OK;
              }
          }
          
          /* De bre8hke kenh 8esh se auto to block, fortwnoyme to epomeno */
          if ( fread(buffer, BLOCKSIZE, 1, fp) != 1 && !feof(fp)){
             Sort_errno = FREAD_ERROR;
             return FREAD_ERROR;
          }
    }

    /*
        De bre8hke kenh 8esh sta yparxonta blocks, opote dhmiourgoume neo block to opoio tha to valoyme
        sto telos toy arxeioy kai eisagoume thn eggrafh ekei.
    */

    /* Arxikopoioume to block */
    memset(buffer, 0, BLOCKSIZE);

    /* Grafoume sth prwth 8esh tou block enhmerwnontas kai to antistoixo bitmap */
    BIT_Flip(&buffer[0], 0);
    buffer_offset = buffer + fhptr->bitmap_size_in_bytes;

    if (fhptr->attrType1 == 'c'){
       strncpy((char*)buffer_offset, (char*)value1, fhptr->attrLength1 );
    }
    else if ( (fhptr->attrType1 == 'i') || (fhptr->attrType1 == 'f')){
       memcpy(buffer_offset, value1, fhptr->attrLength1);
    }
    else{
       Sort_errno = ATTR_ERROR;
       return ATTR_ERROR;
    }

    buffer_offset += fhptr->attrLength1;

    if (fhptr->attrType2 == 'c'){
       strncpy((char*)buffer_offset, (char*)value2, fhptr->attrLength2 );
    }
    else if ( (fhptr->attrType2 == 'i') || (fhptr->attrType2 == 'f')){
       memcpy(buffer_offset, value2, fhptr->attrLength2);
    }
    else{
       Sort_errno = ATTR_ERROR;
       return ATTR_ERROR;
    }

    /* grafome to neo block sto arxeio */
    if (fwrite(buffer, BLOCKSIZE, 1, fp) != 1) {
       Sort_errno = FWRITE_ERROR;
       return FWRITE_ERROR;
    }

    free(fhptr);
    if ( fclose(fp) == EOF) {
       Sort_errno = CLOSE_FILE_ERROR;
       return CLOSE_FILE_ERROR;
    }

    return OK;
}

/**** int Sort_ExtSort(char *fileName,int bufferSize) ****

 Input:
       * char *fileName     arxeio gia external Sorting
       * int bufferSize     megethos tou buffer se blocks

 Operation:
         * Taksinomei to arxeio fileName me ton algorithmo ekswterikhs taksinomhshs

 Output:
        * To arxeio "fileName" me eggrafes taksinomimenes kata ayksoysa seira

 Return Value:
        * OK, ean oloklhrw8ei epityxws
        * Alliws, antistoixo kwdiko lathoys
*/


int Sort_ExtSort(char *fileName, int bufferSize)
{
    FILE *fpin, *fpout;
    char *buffer, *dest, *source, *temp_block, ch;
    int *subfile_distance, *subfile_size;
    int i, n, j, temp_q, t, l, k, p, new_subfile_size, old_subfile_size, q, iterations, total_blocks, 
        total_subfiles_before_merge, records_to_sort, skoypidia, sum, subfiles_left, current_subfile, tmp_error;
    size_t record_data;
    long int size;

    /* o algorithmos apaitei toylaxiston 3 blocks diathesima sth mnhmh gia tin ektelesh toy */
    if (bufferSize < 3) {
        Sort_errno = WRONG_BUFFSIZE_ERROR;
        return WRONG_BUFFSIZE_ERROR;
    }

    if ((fpin = fopen(fileName, "rb+")) == NULL) {
        Sort_errno = OPEN_FILE_ERROR;
        return OPEN_FILE_ERROR;
    }

    /* to temp einai to proswrino arxeio sto opoio tha eisagoyme ola ta sorted subfiles poy tha dhmioyrgithoyn sthn prwth fash
       toy algorithmoy. */
    if ((fpout = fopen("temp", "wb")) == NULL) {
        Sort_errno = OPEN_FILE_ERROR;
        return OPEN_FILE_ERROR;
    }

    if ((buffer = malloc(bufferSize * BLOCKSIZE)) == NULL) {
        Sort_errno = MEM_ERROR;
        return MEM_ERROR;
    }

    if ((header_data = malloc(sizeof(struct file_header))) == NULL) {
        Sort_errno = MEM_ERROR;
        return MEM_ERROR;
    }

    /* Euresh tou plh8ous twn blocks sto arxeio */
    fseek(fpin, 0, SEEK_END);
    size = ftell(fpin);
    total_blocks = size / BLOCKSIZE;
    total_blocks--; /* -1 block logw toy arxikoy header-block */
    rewind(fpin);

    /* Fortwsh tou header */
    if (fread(header_data, sizeof(struct file_header), 1, fpin) != 1) {
        Sort_errno = FREAD_ERROR;
        return FREAD_ERROR;
    }

/*
*******************************************************************************
*     Telos fashs arxikopoihshs / Enarksh dhmiourgias twn prwtwn subfiles     *
*******************************************************************************
                                                                              */

    /* Pame sto prwto block */
    fseek(fpin, BLOCKSIZE , SEEK_SET);
    iterations = ceil((float)(total_blocks / (float)bufferSize));
    
    /*
         Briskoume to plh8os twn axrhsimopoihtwn bytes sto telos ka8e block
         An ap'to block afairesoume to bitmap kai oles tis eggrafes, to ypoloipo
         einai axrhsth plhroforia. Fysika, to plh8os bytes pou xrhsimopoioun oi
         eggrafes einai oso to block meion ta skoupidia kai to bitmap!
    */
    skoypidia = BLOCKSIZE - header_data->bitmap_size_in_bytes
                - (header_data->total_records_per_block * (header_data->attrLength1 + header_data->attrLength2));

    record_data = BLOCKSIZE - header_data->bitmap_size_in_bytes - skoypidia;

    /* Dhmiourgia subfiles pou apotelountai apo "bufferSize"-plh8ous blocks */
    for (i = 0; i < iterations; i++)
    {
        /*
             Fortwnoyme ta "bufferSize" blocks sth mnhmh kai afairoume
             ta bitmaps tous kathws kai oti axrhsimopoihtos xwros yparxei
             sto telos kathe block wste na perasoume ton buffer sthn qqsort()
             kai na apoteleitai apo synexomenes eggrafes
        */
        source = buffer + header_data->bitmap_size_in_bytes;
        dest = buffer;

        for (j = 0; j < bufferSize; j++) {

            if ((q = fread(buffer + (j * BLOCKSIZE), 1, BLOCKSIZE, fpin)) != BLOCKSIZE && !feof(fpin))
            {
                Sort_errno = FREAD_ERROR;
                return FREAD_ERROR;
            }
            
            if (feof(fpin)) { /* an ftasame sto telos toy arxeioy den exoyme kati allo gia na fortwsoyme */
               break;
            }            

            /*
               Tha metaferoume ta blocks kata "bitmap_size_in_bytes" bytes pros ta aristera,
               kai tha grapsoume panw sta bitmaps
            */
            memcpy(dest, source, record_data);

            /*
               Tha paroume twra tis eggrafes tou epomenou block,
               oi opoies tha mpoun amesws meta ap'autes pou molis "organwsame"
            */
            dest += record_data;
            source += BLOCKSIZE;
        }

        /* Plh8os eggrafwn, an ola ta blocks htan gemata */
        records_to_sort = j * header_data->total_records_per_block;

        /* To teleutaio block mporei na mhn einai gemato me eggrafes... */
        if ((i == iterations - 1)) {
            records_to_sort = records_to_sort - 
                              ((total_blocks * header_data->total_records_per_block) - header_data->total_records_in_file);
        }

        /* Exoume enan buffer pou exei mono eggrafes, autes tha taksinomhsoume */
        qqsort(buffer, records_to_sort, header_data->attrLength1, header_data->attrLength2);

        /*
             Afou o buffer exei twra sorted eggrafes
             ksanaftiaxnoume ta bitmaps, phgainontas to ka8e "block"
             sth swsth tou 8esh (pros8etoume dhladh pali ta bitmaps)
        */

        /* Arxizoyme apo to teleytaio block poy mpike sto buffer */
        swap_internal( (void*)&source, (void*)&dest, sizeof(char*));

        for (k = j - 1; k >= 0 ; k--)
        {
            source -= record_data;
            dest -= BLOCKSIZE;
            memcpy(dest, source, record_data);

            /* Initialize to bitmap me 0 */
            memset(dest - header_data->bitmap_size_in_bytes, 0, header_data->bitmap_size_in_bytes);

            /* Kai twra vale tosous assous oso to plh8os twn eggrafwn*/

            /* To teleytaio block omws mporei na mhn einai gemato! */
            if ((i == iterations - 1) && (k == j - 1))
            {
               /*
                  Ypologizoume poses eggrafes tha eixame an ola ta blocks einai gemata.
                    Ap'auth th posothta, afairoume tis twrines eggrafes.
                  Mas menoun loipon poses eggrafes einai kenes sto teleutaio block.
                  An auto to afairesoume ap'to plh8os twn eggrafwn ana block,
                    mas menoun oi eggrafes tou teleutaiou block!
               */
                for (q = 0; q < header_data->total_records_per_block - ((total_blocks * header_data->total_records_per_block) - header_data->total_records_in_file); q++) {
                    BIT_Flip((unsigned char*)(buffer + (k * BLOCKSIZE) + q / 8), q % 8);
                }
            }
            else {
                for (q = 0; q < header_data->total_records_per_block; q++) {
                    BIT_Flip((unsigned char*)(buffer + (k * BLOCKSIZE) + q / 8), q % 8);
                }
            }
        }

        if ((q = fwrite(buffer, 1, j * BLOCKSIZE, fpout)) != (j * BLOCKSIZE)) {
            Sort_errno = FWRITE_ERROR;
            return FWRITE_ERROR;
        }
    }

/*
 ******************************************************************************
 *                      Ektelesh tou merging merous                           *
 ******************************************************************************
                                                                              */
    if ( fclose(fpin) == EOF) {
       Sort_errno = CLOSE_FILE_ERROR;
       return CLOSE_FILE_ERROR;
    }
    
    if ( fclose(fpout) == EOF) {
       Sort_errno = CLOSE_FILE_ERROR;
       return CLOSE_FILE_ERROR;
    }

    if ( (fpin=fopen("temp", "rb+")) == NULL) {
       Sort_errno = OPEN_FILE_ERROR;
       return OPEN_FILE_ERROR;
    }
    
    /* O temp_block krataei ta blocks pou tha ginoun merged */
    temp_block = buffer + ( (bufferSize-1) * BLOCKSIZE);
    if ( (subfile_distance = malloc (iterations * sizeof(int))) == NULL){
       Sort_errno = MEM_ERROR;
       return MEM_ERROR;
    }
    
    if ( (subfile_size = malloc (iterations * sizeof(int))) == NULL){
       Sort_errno = MEM_ERROR;
       return MEM_ERROR;
    }

    /*
       Kathe subfile (ektos isws ap to teleytaio) periexei bufferize blocks,
       apo ta opoia to kathe block exei megethos BLOCKSIZE
    */

     /* O subfile_distance einai o pinakas poy periexei posh apostash (se bytes)
        apexei apo tin arxh toy arxeioy to antistoixo subfile */
    for (i=0; i < iterations; i++){
        subfile_distance[i] = i * bufferSize * BLOCKSIZE;
    }

    /* O subfile_size einai o pinakas poy periexei poso megethos (se blocks) exei to kathe subfile */
    sum = 0;
    for (i=0; i < iterations - 1; i++){
        subfile_size[i] = bufferSize;
        sum += subfile_size[i];
    }

    fseek(fpin, 0, SEEK_END);
    size=ftell(fpin);
    subfile_size[i] = (size / BLOCKSIZE) - sum;
    rewind(fpin);


    /* Ypen8umish: iterations = total_blocks_in_file / bufferSize */
    i = 1;
    p = logbase( (float)(bufferSize-1), (float) iterations);
    j = iterations;
    total_subfiles_before_merge = iterations;


    while (i <= p)
    {          
          n = 1;
          q = (ceil((float) j / (float)(bufferSize - 1)));

          /* Krataei se poio subfile meiname kathe fora etsi wste h merge()
             na diavasei apo kei kai pera ta epomena "bufferSize-1" subfiles */
          current_subfile = 0; 
          
          /* Krataei posa subfiles mas menoun sto twrino perasma gia merging */
          subfiles_left = total_subfiles_before_merge; 

          /* Theloume na "ksemeinei" ena subfile mono tou sto telos kai na mh kalestei
              kai gi'auto h merge(), gi'auto yparxei h metablhth temp_q */
          if ((total_subfiles_before_merge) % (bufferSize-1) == 1)
          {
             temp_q = q - 1;
          }
          else{
             temp_q = q;
          }
          

          while (n <= temp_q)
          {
                /*
                   An mas exoyn meinei ligotera subfiles ap'to megethos tou diathesimoy buffer
                   tote prepei na kalesoume thn merge me auto to meiwmeno plh8os subfiles
                */
                if (subfiles_left < bufferSize-1){
                     if ( (tmp_error = merge(temp_block, subfile_distance, subfile_size, current_subfile, subfiles_left, fpin, buffer)) != OK){
                        return tmp_error;
                     }
                }
                else {
                     if ( (tmp_error = merge(temp_block, subfile_distance, subfile_size, current_subfile, bufferSize - 1, fpin, buffer)) != OK){
                        return tmp_error;
                     }
                }

                if ( n == temp_q ){
                   /* kanontas break sto shmeio ayto tha diatirithei sth metavlith subfiles_left to plithos 
                      twn subfiles pou xrhsimopoiithikan sto teleytaio merge, to opoio xrhsimopoieitai parakatw */
                   break;
                }

                n++;
                current_subfile += (bufferSize - 1);
                subfiles_left -= (bufferSize-1);
          }


/***********************************************************************************
 * Twra prepei na allaksoyme toys pinakes poy kratane tis plhrofories gia ta subfiles
 * ta subfiles poy dimioyrgithikan isoyntai me to plithos twn klhsewn tis merge (temp_q) poy molis eginan
 * edw tha enimerwsoyme toys pinakes gia ola ta merges poy ginane ektos toy teleytaioy poy apotelei eidikh
   periptwsh poy tha eksetastei parakatw
************************************************************************************/


          /* to megethos(se blocks) toy neoy subfile (an ayto einai gemato) tha einai
             oso tou paliou epi to plh8os twn subfiles pou eishx8hsan sto neo subfile (bufferSize - 1 )*/
          old_subfile_size = subfile_size[0]; /* to palio size (se blocks) enos subfile an ayto htan gemato */
          new_subfile_size = old_subfile_size * (bufferSize-1);

          for (t=0; t < temp_q - 1; t++)
          {
              /* H apostash toy t-ostoy subfile apo tin arxh toy arxeioy tha einai t fores to new_subfile_size * BLOCKSIZE */
              subfile_size[t] = new_subfile_size;
              subfile_distance[t] = t * new_subfile_size * BLOCKSIZE;
          }

/*******************************************************************************
 ayto poy mas apasxolei einai oti to teleytaio subfile mporei na
 mhn htan plhrws gemato me blocks gi ayto eksetazoyme tis ekshs 2 periptwseis:
********************************************************************************/

          /*
           1) An to plithos twn subfiles prin apo to twrino perasma MOD to megethos tou buffer == 1, yparxei ena subfile
              to opoio de xrhsimopoiithike sto teleytaio merging kai prepei na eksetastei ksexwrista.
          */
          
          if ((total_subfiles_before_merge) % (bufferSize-1) == 1)
          {
             /* Enhmerwsh twn pinakwn symfwna me to teleutaio merging */
             subfile_size[t] = new_subfile_size;
             subfile_distance[t] = t * new_subfile_size * BLOCKSIZE;

             /* Kai twra enhmerwsh gia to teleutaio subfile */
             t++;

             /* H apostash toy einai oso h apostash toy teleytaio kainoyrgioy subfile + to new_subfile_size * BLOCKSIZE */
             subfile_distance[t] = subfile_distance[t-1] + (new_subfile_size * BLOCKSIZE);
             /* To mege8os tou omws emeine to idio! */
             subfile_size[t] = subfile_size[total_subfiles_before_merge - 1]; 

             /* Ypologismos tou kainourgiou plh8ous twn subfiles */
             total_subfiles_before_merge = temp_q + 1;
          }

          /*
             2) Sth deuterh periptwsh, xrhsimopoih8hke kai to teleutaio subfile,
                opote eksetazoume apokleistika to teleutaio merge kai enhmerwnoume tous pinakes
          */
          else{
              /* Ypologismos toy kainoyrgioy arithmoy twn subfiles */
              total_subfiles_before_merge = temp_q;

              sum = 0;
              /* Ypologizoyme to megethos (se blocks) olwn twn subfiles poy xrhsimopoiithikan sto teleytaio merging */
              for (l=0; l < subfiles_left; l++) {
                  sum += subfile_size[((t * (bufferSize-1)) + l)];
              }
              /* H apostash toy einai oso h apostash toy teleytaio kainoyrgioy subfile + new_subfile_size*BLOCKSIZE */
              subfile_distance[t] = subfile_distance[t-1] + (new_subfile_size * BLOCKSIZE);
              subfile_size[t] = sum;
          }

          j = q;
          i++;
    }

    if ( fclose(fpin) == EOF) {
       Sort_errno = CLOSE_FILE_ERROR;
       return CLOSE_FILE_ERROR;
    }
    
    if ( (fpout=fopen(fileName, "wb")) == NULL){
       Sort_errno = CREATE_FILE_ERROR;
       return CREATE_FILE_ERROR;
    }
    
    {
        char block[BLOCKSIZE];
        memset(block, 0, BLOCKSIZE);
        memcpy(block, header_data, sizeof(struct file_header));
        if (fwrite(block, 1, BLOCKSIZE, fpout) != BLOCKSIZE) {
           Sort_errno = FWRITE_ERROR;
           return FWRITE_ERROR;           
        }
    }

    if ( (fpin=fopen("temp", "rb")) == NULL) {
       Sort_errno = OPEN_FILE_ERROR;
       return OPEN_FILE_ERROR;
    }

    ch = getc(fpin);   
    while(!feof(fpin))
    {     
       putc(ch, fpout);
       ch = getc(fpin);
    }

    if ( fclose(fpin) == EOF) {
       Sort_errno = CLOSE_FILE_ERROR;
       return CLOSE_FILE_ERROR;
    }
    if ( fclose(fpout) == EOF) {
       Sort_errno = CLOSE_FILE_ERROR;
       return CLOSE_FILE_ERROR;
    }

    free(header_data);
    remove("temp");

    return OK;
}



/**** void Sort_PrintError(char *errString) ****

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

void Sort_PrintError(char *errString)
{
    printf("%s--> %s\n", errString, error_msg[-(Sort_errno+1)]);
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


/**** int cmp(const void * a, const void * b) ****

 Input:
       * const void * a     Prwth posothta pros sygkrish
       * const void * b     Deuterh posothta pros sygkrish

 Operation:
           * Analoga me ton typo twn dedomenwn, ektelei sygkrish twn 2
             posothtwn stis antistoixes 8eseis mnhmhs
 Output:
        * None

 Return Value:
        * An epistrepsei 8etiko ari8mo, h posothta a einai megalyterh
        * An epistrepsei arnhtiko ari8mo, h posothta b einai megalyterh
        * An epistrepsei mhden, oi posothtes einai ises
*/

int cmp(const void * a, const void * b)
{
    if (header_data->attrType1 == 'c') {
        return (strcmp(a, b));
    } 
    else if (header_data->attrType1 == 'i') {
        return (*(int*) a - *(int*) b);
    } 
    else if (header_data->attrType1 == 'f') {
        return (*(float*) a - *(float*) b);
    }
    
    return OK;
}

/**** void swap_internal(char *a, char *b, size_t size) ****

 Input:
       * char *a        Prwth posothta pou tha antallax8ei
       * char *b        Deuterh posothta pou tha antallax8ei
       * size_t size    To mege8os twn posothtwn pou tha antallax8oun

 Operation:
           * Antallasei "size" bytes anamesa stis 2 dieu8unseis mnhmhs "a" kai "b"
 Output:
        * None

 Return Value:
        * None
*/

void swap_internal(char *a, char *b, size_t size)
{
    if (a != b) {
        char t;

        while (size--) {
            t = *a;
            *a++ = *b;
            *b++ = t;
        }
    }
}

/* Dikia mas ylopoihsh ths qsort */
void qqsort_internal(char *begin, char *end, size_t size1, size_t size2)
{
    if (end > begin) {
        char *pivot = begin;
        char *l = begin + size1 + size2, *r = end;

        while (l < r) {
            if (cmp(l, pivot) <= 0) {
                l += size1 + size2;
            } else {
                r -= (size1 + size2);
                swap_internal(l, r, size1 + size2);
            }
        }

        l -= (size1 + size2);

        swap_internal(begin, l, size1 + size2);
        qqsort_internal(begin, l, size1, size2);
        qqsort_internal(r, end, size1, size2);
    }
}

void qqsort(void *base, size_t nmemb, size_t size1, size_t size2)
{
    qqsort_internal((char *)base, (char *)base + nmemb*(size1 + size2), size1, size2);
}


/**** int logbase (float base, float x) ****

 Input:
       * float base     Bash tou logari8mou
       * float x        Logari8mizomenos ari8mos

 Operation:
           * Ypologizei kai epistrefei ton logari8mo me bash "base" tou ari8mou "x"
 Output:
        * None

 Return Value:
        * Des "Operation"
*/

int logbase (float base, float x)
{
      return ceil( log(x) / log(base) );
}


/**** int merge( char *temp_block, int * subfile_distance, int * subfile_size, int N, int buffersize, FILE * fpin, char * buffer) ****

 Input:
       - char * temp_block         Proswrino block sto opoio eisagoume tis mikroteres eggrafes
       - int * subfile_distance    Enas pinakas stoy opoioy to iosto stoixeio leei posa bytes apexei to
                                      antistoixo iosto subfile apo tin arxi toy arxeiou
       - int * subfile_size        Pinakas stoy opoiy to iosto stoixeio exei to megethos toy i-ostoy subfile
                                      apo tin arxh toy arxeioy, se blocks
       - int N                     Apo to N-osto subfile kai katw tha ginei to merge
       - int buffersize            To synolo twn subfiles poy tha enwsoume kai tha exoyme ap'oloys 1 block sti mnimi
                                      PROSOXH: Kaleitai me buffsize-1 ap'thn Sort_ExtSort()!!
       - FILE * fpin               Pointer sto arxeio poy periexei ola ta subfiles
       - char * buffer             Pointer sthn arxh toy idioy tou buffer poy tha xrhsimopoiithei gia tin merge

 Operation:
           * Enwnei ta "buffersize"-plh8ous subfiles, opoy to prwto einai to
             N-osto apo thn arxh toy arxeioy
 Output:
        * Eksagei ta sorted subfiles se ena arxeio (fpout)

 Return Value:
        * OK, an oloklhrw8ei epityxws
        * Alliws, antistoixo mhnyma la8ous
*/


int merge( char *temp_block, int * subfile_distance, int * subfile_size, int N, int buffersize, FILE * fpin, char * buffer)
{
    FILE * fpout;
    int i, sum, min, records_in_temp_block, tmp_error;
    int *total_records_in_subfile, *current_block_in_subfile;
    char ch;

    sum = min = records_in_temp_block = 0;
    
    if (buffersize < 2){
       Sort_errno = WRONG_BUFFSIZE_ERROR;
       return WRONG_BUFFSIZE_ERROR;
    }

    /*
       Dhmiourgoume ena proswrino arxeio sto opoio tha apo8hkeuoume to apotelesma ths merge().
       Otan oloklhrw8ei h diadikasia, tha to antigrapsoume sto arxeio me ta subfiles
    */
    if ( (fpout = tmpfile()) == NULL) {
        Sort_errno = OPEN_FILE_ERROR;
        return OPEN_FILE_ERROR;
    }

    /*
       O total_records_in_subfile[] einai enas pinakas ston opoio kratame se ka8e epanalhpsh
       to plh8os twn eggrafwn pou apomenoun sto ka8e subfile. Otan ola ta stoixeia tou ginoun
       mhden, tote exoume xrhsimopoihsei oles tis eggrafes!
    */
    if ((total_records_in_subfile = malloc(buffersize * sizeof(int)) ) == NULL) {
        Sort_errno = MEM_ERROR;
        return MEM_ERROR;
    }

    sum = 0;
    for (i=0; i < buffersize; i++)
    {
        /* Briskoume to plh8os twn eggrafwn se ka8e subfile, opws kai to synoliko plh8os eggrafwn */
        if ( (total_records_in_subfile[i] = total_recs_in_subfile(subfile_distance[N+i], subfile_size[N+i], fpin, buffer) ) < 0 ){
           return total_records_in_subfile[i]; /* Epestrepse la8os h total_recs_in_subfile() */
        }
        sum += total_records_in_subfile[i];
    }

    /*
       O current_block_in_subfile[] einai enas pinakas ston opoio kratame kathe fora
       poio block toy kathe subfile exoyme sth mnhmh. Epeidh ta subfiles einai sorted apo mona toys,
       arxizoyme apo ta arxika tous blocks (pou tha periexoun tis mikroteres eggrafes).
       Ka8ws afairoume eggrafes, tha proxwrame pros ta panw
    */
    if ( (current_block_in_subfile = malloc (buffersize * sizeof(int))) == NULL) {
        Sort_errno = MEM_ERROR;
        return MEM_ERROR;
    }

    /* Arxikopoioume to current block me 0 (arxiko block) */
    for ( i=0 ; i<buffersize ; i++ ){
        current_block_in_subfile[i] = 0;
    }

    /*
         Edw tha fortwsoyme ta prwta blocks apo kathe subfile ston buffer pinaka.
         To teleutaio block tou buffer (to buffersize + 1) to arxikopoioume me 0.
    */

    for (i=0 ; i < buffersize ; i++)
    {
        fseek(fpin, subfile_distance[N+i], SEEK_SET);
        if ( fread(buffer + (i * BLOCKSIZE), 1, BLOCKSIZE, fpin) != BLOCKSIZE) {
            Sort_errno = FREAD_ERROR;
            return FREAD_ERROR;
        }
    }

   /* Arxikopoihsh tou temporary block */
    memset(temp_block, 0, BLOCKSIZE);
    rewind(fpout);

    /* Oso mas menoyn eggrafes */
    while( sum )
    {
          /* Briskoume thn elaxisth, thn afairoume kai th metaferoume sto proswrino block */
          if ( (tmp_error = find_min(current_block_in_subfile, buffer, fpin, buffersize, subfile_distance, total_records_in_subfile, N, temp_block) ) < 0 ){
             return tmp_error; /* Epestrepse la8os h find_min() */
          }
          sum--;
          records_in_temp_block++;

          /*
             An gemhse to proswrino block 'H xrhsimopoihsame oles tis eggrafes,
             grafoume to proswrino block sto arxeio
          */
          if ( (records_in_temp_block == header_data->total_records_per_block) || (sum == 0) )
          {
             if (fwrite(temp_block, 1, BLOCKSIZE, fpout) != BLOCKSIZE) {
                Sort_errno = FWRITE_ERROR;
                return FWRITE_ERROR;                 
             }
             records_in_temp_block = 0;
             memset(temp_block, 0, BLOCKSIZE);
          }
    }
    
   /*
      Tha antikatasthsoume ta buffersize subfiles me to proswrino mas arxeio (pou tha einai to neo subfile)
      kai tha katastrepsoume to proswrino arxeio, pou einai pia axrhsto
   */

   fseek(fpin, subfile_distance[N], SEEK_SET);
   rewind(fpout);

   ch = getc(fpout);   
   while(!feof(fpout))
   {     
       putc(ch, fpin);
       ch = getc(fpout);
   }

   if ( fclose(fpout) == EOF) {
       Sort_errno = CLOSE_FILE_ERROR;
       return CLOSE_FILE_ERROR;
   }

   return OK;
}


/**** int total_recs_in_subfile(int subfile_distance, int subfile_size, FILE * fpin, char * buffer) ****

 Input:
       - int subfile_distance     H apostash toy sygkekrimenoy subfile apo tin arxh toy arxeioy me ta subfiles (se bytes)
       - int subfile_size         To megethos toy sugkekrimenoy subfile se blocks
       - FILE * fpin              Pointer sto arxeio poy periexei ola ta subfiles
       - char * buffer            Pointer sthn arxh toy idioy tou buffer poy tha xrhsimopoiithei gia tin merge

 Operation:
           * Briskei poses eggrafes yparxoun sto subfile pou apexei
             apostash "subfile_distance" ap'thn arxh tou arxeiou, kai epistrefei
             to plh8os sto onoma ths
 Output:
        * None

 Return Value:
        * An oloklhrw8hke epityxws, to plh8os twn eggrafwn sto arxeio fpin
        * Alliws, antistoixo mhnyma la8ous
*/

int total_recs_in_subfile(int subfile_distance, int subfile_size, FILE * fpin, char * buffer){
    int i, j, sum=0;

    /* Pame sthn arxh tou subfile */
    fseek(fpin, subfile_distance, SEEK_SET);

    /* Eimaste sto bitmap tou prwtou block kai tha kanoume prospelash kai anazhthsh energwn eggrafwn se ka8e block */
    for (i=0; i < subfile_size; i++)
    {
        /* Fortwnoume to epomeno block sth mnhmh */
        if ( fread(buffer, 1, BLOCKSIZE, fpin) != BLOCKSIZE ){
           Sort_errno = FREAD_ERROR;
           return FREAD_ERROR;
        }
        
        /* Psaxnoume gia energes eggrafes sto bitmap tou block */
        for (j=0; j < header_data->total_records_per_block; j++){
              if (BIT_IsSet(buffer[j/8], j % 8)) {
                 sum++;
              }
        }
    }

    return sum;
}


/**** int find_min(int *current_block_in_subfile, char *buffer, FILE *fpin, int buffersize, int *subfile_distance,
                    int * total_records_in_subfile, int N, char *temp_block) ****

 Input:
       - int * current_block_in_subfile = o current_block_in_subfile einai enas pinakas ston opoio kratame kathe fora poio 
               block toy kathe subfile exoyme sti mnimi.
       - char * buffer = pointer sthn arxh toy idioy tou buffer poy tha xrhsimopoiithei gia tin merge
       - FILE * fpin = pointer sto arxeio poy periexei ola ta subfiles
       - int buffersize = to synolo twn subfiles poy eksetazoyme kai tha exoyme ap oloys 1 bock sti mnimi
       - int * subfile_distance = enas pinakas stoy opoioy to iosto stoixeio leei posa bytes apexei to
                                  antistoixo iosto subfile apo tin arxi toy arxeiou
       - int * subfile_size = enas pinakas stoy opoioy to iosto stoixeio exei to megethos toy iostoy subfile se blocks                            
       - int * total_records_in_subfile = o total_records_in_subfile einai enas pinakas ston opoio kratame kathe fora poses eggrafes
                                          apomenoyn sto kathe subfile
       - int N = Apo to Nosto subfile kai katw tha ginei to merge

 Operation:
           * vriskei tin elaxisth eggrafh ap ola ta buffersize blocks pou exoyn fortwthei kai na to vazei
             sto temp block poy exoyme gia proswrino save kai meta fwrite.
           * an oi eggrafes se kapoio block teleiwsoyn fortwnei to epomeno sth mnhmh kai enhmerwnei toys katalliloys pinakes
 Output:
        * None

 Return Value:
        * An oloklhrw8ei epityxws, OK
        * Alliws, antistoixo mhnyma la8ous
*/

int find_min(int *current_block_in_subfile, char *buffer, FILE *fpin, int buffersize, int *subfile_distance,
              int * total_records_in_subfile, int N, char *temp_block)
{

    int i, j, min_block;
    char **min_rec_of_block, *min, *tmp;
    /* o min_rec_of_block einai enas pinakas ston opoio tha apothikeyetai h 8esh ths elaxisths record
       apo ola ta blocks poy einai sth mnhmh kai argotera tha ginei h sygkrish metaksy toys gia tin eyresh toy mikroteroy */

    /* TODO: Elegxos gia swsto buffersize */
    if ( (min_rec_of_block = malloc(buffersize * sizeof(char *))) == NULL){
       Sort_errno = MEM_ERROR;
    }

    for (i=0; i < buffersize ; i++)
    {
    
        /* Gia kathe subfile eksetazoyme an exoyn apomeinei eggrafes se ayto */
        if (total_records_in_subfile[i] == 0){
	        min_rec_of_block[i] = NULL;
        }
        else
        {
	       /* an exoyn apomeinei kaloyme tin blocks_minimum poy vriskei thn elaxisth eggrafh apo to block toy
              i-ostoy subfile poy exoyme fortwsei sth mnhmh */
           if ( (tmp = blocks_minimum(buffer + i*BLOCKSIZE)) != NULL ){
              min_rec_of_block[i] = tmp;
           }
           /* an to twrino block den exei alles eggrafes tote fortwnoyme sth mnhmh to epomeno block toy subfile */
           else{
		        current_block_in_subfile[i]++;
		        fseek(fpin, subfile_distance[N+i] + (current_block_in_subfile[i]*BLOCKSIZE), SEEK_SET);
		        if (fread(buffer + (i*BLOCKSIZE), 1, BLOCKSIZE, fpin) != BLOCKSIZE){
                   Sort_errno = FREAD_ERROR;
                }
		        /* ksanakalese tin blocks_min gia na vroyme th minimum eggrafh toy block */
		        min_rec_of_block[i] = blocks_minimum(buffer + i*BLOCKSIZE);
           }
        }
    }

    /* Briskoume th prwth non-NULL eggrafh pros sygkrish */
    min = NULL;
    for (i=0; i<buffersize;i++)
    {
        if ( min_rec_of_block[i] != NULL )
        {
           min = min_rec_of_block[i];
           min_block=i;
           break;
        }
    }
    
    /* Briskoume thn elaxisth eggrafh ap oles tis eggrafes */
    for (i=i+1; i<buffersize;i++)
    {
        if ( (min_rec_of_block[i] != NULL) && (cmp(min_rec_of_block[i], min) < 0 ) )
           {
               min=min_rec_of_block[i];
               min_block=i;
           }
    }

    /* Psaxnoume gia kenh 8esh sto temp_block etsi wste na valoyme se ayth thn minimum_record */
    for (j=0; j < header_data->total_records_per_block; j++)
    {
        /* An einai kenh h 8esh tou bitmap sto temp_block */
        if (!BIT_IsSet(temp_block[j/8], j % 8))
        {           
           /* enhmerwse to bitmap */
           BIT_Flip((unsigned char*)temp_block + j/8, j % 8);
           /* kai vale sto block tin elaxisth eggrafh */
           memcpy(temp_block + header_data->bitmap_size_in_bytes + j * (header_data->attrLength1 + header_data->attrLength2),
                  min,
                  header_data->attrLength1 + header_data->attrLength2);
           break;
        }
    }
    
    /*
       Prepei na enhmerwthei to bitmap toy block apo to opoio phrame tin elaxisth eggrafh.
       Epeidh to block einai taksinomimeno h thesi poy zhtame einai h prwth energh eggrafh sto bitmap
    */
    for (j=0; j < header_data->total_records_per_block; j++)
    {
        if (BIT_IsSet( *(buffer + min_block*BLOCKSIZE + j/8), j % 8))
        {
           BIT_Flip((unsigned char*)buffer + min_block*BLOCKSIZE + j/8, j % 8);
           break;
        }
    }

    /* enhmerwnoyme ton antistoixo pinaka poy krataei posa records emeinan sto kathe subfile */
    total_records_in_subfile[min_block]--;

    free(min_rec_of_block);
    return OK;
}


/**** char *blocks_minimum(char * block) ****

 Input:
       - char *block = pointer sthn arxh toy block to opoio eksetazoyme

 Operation:
           * epistrefei sto onoma tis pointer se thesi mnimis sto block opoy einai h minimum energh record toy block
 Output:
        * None

 Return Value:
        * tin elaxisth energh record tou block
        * NULL an den yparxei oyte mia energh record sto block
*/


char *blocks_minimum(char * block)
{
     char * result, * temp_record;
     int i, flag;

     flag=Sort_EOF;
     
     /* psaxnoyme thn prwth energh eggrafh toy block gia na th xrhsimopoihsoyme parakatw sthn sygkrish */
     
     for (i=0; i < header_data->total_records_per_block; i++)
     {
         /* An einai energh h eggrafh */
          if (BIT_IsSet(block[i/8], i % 8))
          {
                 result = block + header_data->bitmap_size_in_bytes + i * (header_data->attrLength1 + header_data->attrLength2);
                 flag = 1;
                 break;
          }
     }

     if ( flag == Sort_EOF ){ /* An de vrikame oyte mia eggrafh epistrefoyme NULL */
        return NULL;
     }

     /* Twra poy exoyme ena arxiko result gia sygkrish, prospelaynoyme tis ypoloipes energes eggrafes pros eyresh ths mikroterhs */
     
     for(i=i+1; i < header_data->total_records_per_block; i++)
     {         
              
         /* An einai energh h eggrafh */
         if (BIT_IsSet(block[i/8], i % 8))
         {
             temp_record = block + header_data->bitmap_size_in_bytes + i * (header_data->attrLength1 + header_data->attrLength2);
             
             if (cmp(result, temp_record) > 0){
                result = temp_record;
             }
         }
     }

     return result;
}
