/*
HT.h

std06066 ���������� ���������-������
std06101 �������� ����������
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
    char   *fileName,      /* ����� ������� */
    char   attrType1,      /* ����� ������ ������: 'c' (����� ����������), 'i' (��������), 'f' (�����������) */
    int    attrLength1,    /* ����� ������ ������: 4 ��� 'i' � 'f', 1-255 ��� 'c' */
    char   attrType2,      /* ����� �������� ������: 'c' (����� ����������), 'i' (��������), 'f' (�����������) */
    int    attrLength2,    /* ����� �������� ������: 4 ��� 'i' � 'f', 1-255 ��� 'c' */
    int    buckets         /* ������� ����� ��������������� */
);

int HT_DestroyIndex(
    char   *fileName       /* ����� ������� */
);

int HT_OpenIndex(
    char   *fileName       /* ����� ������� */
);

int HT_CloseIndex(
    int    fileDesc        /* ������� ��� ����������� ��� ������� ������ */
);

int HT_InsertEntry(
    int    fileDesc,       /* ������� ��� ����������� ��� ������� ������ */
    void   *value1,        /* ���� ��� ������-�������� ���� �������� */
    void   *value2         /* ���� ��� �������� ������ ��� �������� ���� �������� */
);

int HT_DeleteEntry(
    int    fileDesc,       /* ������� ��� ����������� ��� ������� ������ */
    void   *value1,        /* ���� ��� ������-�������� ���� �������� */
    void   *value2         /* ���� ��� �������� ������ ��� �������� ���� �������� */
);

int HT_OpenIndexScan(
    int    fileDesc,       /* ������� ��� ����������� ��� ������� ������ */
    int    op,             /* �������� ��������� */
    void   *value          /* ���� ��� ������-�������� ���� �������� */
);

void *HT_FindNextEntry(
    int    scanDesc        /* ������� ��� ����������� ���� ������� ������ */
);

int HT_CloseIndexScan(
    int    scanDesc        /* ������� ��� ����������� ���� ������� ������ */
);

void HT_PrintError(
    char    *errString     /* ������� ��� �������� */
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
