#pragma once

typedef struct {
	char **list;
	int length;
	int capacity;
} StringList;

StringList *listNew();
StringList *listNewCapacity(const int capacity);
StringList *listSublist(const StringList *list, const int from, const int to);
StringList *listClone(const StringList *list);
void listDestroy(StringList *list);

StringList *listSetCapacity(StringList *list, const int new_capacity);
StringList *listEnsureCapacity(StringList *list, const int min_capacity);
StringList *listTrimCapacity(StringList *list);

StringList *listSet(StringList *list, const int index, const char *value);
StringList *listAdd(StringList *list, const char *value);
StringList *listAddAll(StringList *list, const StringList *source);
StringList *listInsert(StringList *list, const int index, const char *value);
StringList *listInsertAll(StringList *list, const int index, const StringList *source);

int listCapacity(const StringList *list);
int listLength(const StringList *list);
char *listGet(const StringList *list, const int index);
int listIndexOf(const StringList *list, const char *element);
int listLastIndexOf(const StringList *list, const char *element);

bool listIsEmpty(const StringList *list);
bool listContains(const StringList *list, const char *element);
bool listContainsAll(const StringList *list, const StringList *must_contain);
bool listEquals(const StringList *list_a, const StringList *list_b);

void listRemove(StringList *list, const int index);
void listRemoveElement(StringList *list, const char *element);
void listRemoveElements(StringList *list, const char *element);
void listRemoveIf(StringList *list, bool(*conditional_funct)(const char *));
void listRemoveAll(StringList *list, const StringList *to_remove);
void listClear(StringList *list);

void listPrint(const StringList *list);
