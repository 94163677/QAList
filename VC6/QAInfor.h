
#define QA_INFOR 1
#define QA_INFO_ITEM_LENGTH 10 //问答最大数

typedef struct QAInfoItem_Struct
{
    LPSTR question;
	LPSTR answer;
} QAInfoItem;

QAInfoItem infoList[QA_INFO_ITEM_LENGTH];//问答列表，包括全部的问答数据

/*
 * 将所有问答信息载入infoList（问答列表）
 */
void                getIntoQAInfoList();
