
#define QA_INFOR 1
#define QA_INFO_ITEM_LENGTH 10 //�ʴ������

typedef struct QAInfoItem_Struct
{
    LPSTR question;
	LPSTR answer;
} QAInfoItem;

QAInfoItem infoList[QA_INFO_ITEM_LENGTH];//�ʴ��б�����ȫ�����ʴ�����

/*
 * �������ʴ���Ϣ����infoList���ʴ��б�
 */
void                getIntoQAInfoList();
