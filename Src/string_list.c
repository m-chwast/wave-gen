#include "string_list.h"
#include "cmsis_os.h"
#include <string.h>


StringList StringList_Create()
{
	StringList list = NULL;
	return list;
}


void StringList_AddElem(StringList * list, const char * str)
{
	struct StringListElem * newElem = pvPortMalloc(sizeof(struct StringListElem));
	char * newStr = pvPortMalloc(strlen(str) + 1);
	strcpy(newStr, str);

	newElem->str = newStr;
	newElem->next = NULL;

	if(*list != NULL)
	{
		struct StringListElem * tmpElem = *list;
		while(tmpElem->next != NULL)
			tmpElem = tmpElem->next;
		tmpElem->next = newElem;
	}
	else
	{
		*list = newElem;
	}
}

char * StringList_GetFirstElem(const StringList list)
{
	if(list == NULL)
		return NULL;
	return list->str;
}

void StringList_DeleteElem(StringList * list)
{
	if(*list == NULL)
		return;

	StringList newHead = (*list)->next;

	if((*list)->str != NULL)
		vPortFree((*list)->str);
	vPortFree(*list);

	*list = newHead;
}

uint16_t StringList_GetElemsCount(StringList list)
{
	uint16_t cnt = 0;
	while(list != NULL)
	{
		cnt++;
		list = list->next;
	}
	return cnt;
}

uint32_t StringList_GetCharsCount(StringList list)
{
	uint32_t cnt = 0;
	while(list != NULL)
	{
		cnt += strlen(list->str);
		list = list->next;
	}
	return cnt;
}
