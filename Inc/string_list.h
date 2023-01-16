

struct StringListElem
{
	char * str;
	struct StringListElem * next;
};

typedef struct StringListElem * StringList;



StringList StringList_Create();
void StringList_AddElem(StringList list, char * str);
char * StringList_GetFirstElem(StringList list);
void StringList_DeleteElem(StringList list);
