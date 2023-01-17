#ifndef STRING_LIST_H
#define STRING_LIST_H


struct StringListElem
{
	char * str;
	struct StringListElem * next;
};

typedef struct StringListElem * StringList;



StringList StringList_Create();
void StringList_AddElem(StringList * list, const char * str);
char * StringList_GetFirstElem(const StringList list);
void StringList_DeleteElem(StringList * list);


#endif
