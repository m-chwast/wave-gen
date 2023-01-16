

struct StringListElem
{
	const char * str;
	struct ListElem * next;
};

typedef struct StringListElem * StringList;



StringList StringList_Create();
void StringList_AddElem(StringList * list, const char * str);
char * StringList_GetFirstElem();
void StringList_DeleteElem();
