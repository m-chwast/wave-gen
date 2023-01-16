#include "string_list.h"
#include "cmsis_os.h"
#include <string.h>


StringList StringList_Create()
{
	StringList list = NULL;
	return list;
}


void StringList_AddElem(StringList list, char * str)
{
	struct StringListElem * newElem = pvPortMalloc(sizeof(struct StringListElem));
	char * newStr = pvPortMalloc(strlen(str) + 1);
	strcpy(newStr, str);

	newElem->str = newStr;
	newElem->next = NULL;

	if(list != NULL)
	{
		while(list->next != NULL)
			list = list->next;

	}
	list = newElem;
}

char * StringList_GetFirstElem(StringList list)
{
	if(list == NULL)
		return NULL;
	return list->str;
}

void StringList_DeleteElem(StringList list)
{
	if(list == NULL)
		return;

	StringList newHead = list->next;

	if(list->str != NULL)
		vPortFree(list->str);
	vPortFree(list);

	list = newHead;
}
