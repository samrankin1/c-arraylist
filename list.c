#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "list.h"

/**
	TODO:
	char *list_to_string(const StringList *sl);
	void list_sort(const StringList *sl, int (*comparator_funct)(const char *, const char *))); // quicksort
**/

StringList *listNewCapacity(const int capacity) {

	StringList *result = malloc(sizeof(StringList));
	if (result == NULL) {
		return NULL;
	}

	char **list = malloc(capacity * sizeof(char*));
	if (list == NULL) {
		free(result);
		return NULL;
	}

	result->list = list;
	result->length = 0;
	result->capacity = capacity;

	return result;
}

StringList *listNew() {
	return listNewCapacity(10);
}

StringList *listSublist(const StringList *sl, const int from, const int to) {
	assert(from >= 0); // make sure the 'from' index is not negative
	assert(to <= sl->length); // make sure 'to' is an existing index in 'list'
	assert(to > from);

	// initialize a new StringList with initial capacity exactly the finished size
	StringList *result = listNewCapacity((to - from));

	for (int i = from; i < to; i++) { // for each index in [to, from)
		// add this element from the master list to the sublist
		if (listAdd(result, sl->list[i]) == NULL) {
			listDestroy(result);
			return NULL;
		}
	}

	return result;
}

// return a sublist clone containing the entire original list
StringList *listClone(const StringList *sl) {
	return listSublist(sl, 0, sl->length);
}

void listDestroy(StringList *sl) {
	for (int i = 0; i < sl->length; i++) {
		free(sl->list[i]); // free each string buffer
	}
	free(sl->list); // free string buffer array
	free(sl); // free struct memory
}


// beware: providing a capacity less than the current List's length will drop the overflow elements
StringList *listSetCapacity(StringList *sl, const int capacity) {

	if (capacity < sl->length) { // if the new capacity is less than the current length
		// new capacity is the first index to be dropped, continue to the end of the current internal list
		for (int i = capacity; i < sl->length; i++) {
			// free the element's buffer to prevent leak
			free(sl->list[i]);
		}
		sl->length = capacity; // update the length field
	}

	// resize the memory allocated to this StringList's internal list
	char **newList = realloc(sl->list, (capacity * sizeof(char*)));
	if (newList == NULL) {
		return NULL;
	}

	sl->list = newList;
	sl->capacity = capacity;
	return sl;
}

StringList *listEnsureCapacity(StringList *sl, const int capacity) {
	if (capacity > sl->capacity) {
		return listSetCapacity(sl, capacity);
	} else {
		return sl;
	}
}

StringList *listTrimCapacity(StringList *sl) {
	return listSetCapacity(sl, sl->length);
}

StringList *_list_expand_auto(StringList *sl) {
	return listSetCapacity(sl, ((sl->capacity * 2) + 1)); // 2x current capacity plus 1
}


StringList *_list_set_unchecked(StringList *sl, const int index, const char *value) {

	char *copyBuf = malloc(strlen(value) + 1); // create a buffer for a copy of the 'value' string
	if (copyBuf == NULL) {
		return NULL;
	}

	strcpy(copyBuf, value); // copy the 'value' string into the buffer
	sl->list[index] = copyBuf; // set the pointer at the next index in the list to the new buffer
	return sl;
}

StringList *listSet(StringList *sl, const int index, const char *value) {
	// note: copies the 'value' string to make sure values are available until removed from the list
	// printf("setting index '%d' (len = '%d')\n", index, list->length);
	// lists can only be expanded one at a time by passing their current length as an index
	assert(index <= sl->length);
	assert(index >= 0);

	// if this is a set() call for the n+1 element index (special case mentioned above)
	if (index == sl->length) {
		// if more capacity will be needed to store something at this index
		if ((sl->length + 1) > sl->capacity) {
			// expand the capacity to an automatically determined larger size
		 	if (_list_expand_auto(sl) == NULL) {
		 		return NULL;
		 	}
		}
		sl->length++; // increase the length regardless, there is a new valid index
	} else { // if this set() call will overwrite an existing element in the list
		free(sl->list[index]); // free the memory at the pointer to be overwritten
	}

	return _list_set_unchecked(sl, index, value);
}

StringList *listAdd(StringList *sl, const char *value) {
	return listSet(sl, sl->length, value);
}

StringList *listAddAll(StringList *sl, const StringList *src) {
	for (int i = 0; i < src->length; i++) {
		if (listAdd(sl, src->list[i]) == NULL) {
			return NULL;
		}
	}
	return sl;
}

StringList *listInsert(StringList *sl, const int index, const char *value) {
	assert(index < sl->length);
	assert(index >= 0);

	// make sure there's room for the added element
	if ((sl->length + 1) > sl->capacity)  {
		if (_list_expand_auto(sl) == NULL) { // expand the capacity
			return NULL;
		}
	}

	for (int i = (sl->length - 1); i >= index; i--) { // descend down the internal list
		sl->list[(i + 1)] = sl->list[i]; // no copy needed, just move around the existing pointers
	}

	// set the value at this index to a copy of the 'value' string
	if (_list_set_unchecked(sl, index, value) == NULL) {
		return NULL;
	}

	sl->length++; // increment the length, as one element has been added to the list
	return sl;
}

StringList *listInsertAll(StringList *sl, const int index, const StringList *src) {
	assert(index < sl->length);
	assert(index >= 0);

	int destLen = sl->length;
	int srcLen = src->length;

	// make sure the destination has enough space for both sets of elements
	if (listEnsureCapacity(sl, (destLen + srcLen)) == NULL) {
		return NULL;
	}

	// from the insert index to the end of the original list
	for (int i = index; i < destLen; i++) {
		sl->list[i + srcLen] = sl->list[i]; // move each string to the end of the new list
	}

	// copy-insert each string from src to dest
	for (int o = 0; o < srcLen; o++) {
		if (_list_set_unchecked(sl, (index + o), src->list[o]) == NULL) {
			return NULL;
		}
	}

	sl->length += srcLen;
	return sl;
}


int listCapacity(const StringList *sl) {
	return sl->capacity;
}

int listLength(const StringList *sl) {
	return sl->length;
}

char *listGet(const StringList *sl, const int index) {
	assert(index < sl->length); // make sure the index being retrieved actually exists
	assert(index >= 0);

	return sl->list[index];
}

int listIndexOf(const StringList *sl, const char *element) {
	for (int i = 0; i < sl->length; i++) {
		if (strcmp(sl->list[i], element) == 0) {
			return i;
		}
	}
	return -1;
}

int listLastIndexOf(const StringList *sl, const char *element) {
	for (int i = (sl->length - 1); i >= 0; i--) {
		if (strcmp(sl->list[i], element) == 0) {
			return i;
		}
	}
	return -1;
}


bool listIsEmpty(const StringList *sl) {
	return (sl->length == 0);
}

bool listContains(const StringList *sl, const char *element) {
	return (listIndexOf(sl, element) != -1); // index_of returns -1 if the element was not found
}

bool listContainsAll(const StringList *sl, const StringList *must_contain) {
	for (int i = 0; i < must_contain->length; i++) { // for each index in the must_contain List
		// if the target list does not contain this element
		if (!listContains(sl, must_contain->list[i])) {
			return false; // the list does not "contain all"
		}
	}

	// if no missing elements were found, 'list' contains all elements in 'must_contain'
	return true;
}

bool listEquals(const StringList *sl_a, const StringList *sl_b) {
	if (sl_a->length != sl_b->length) { // the Lists cannot be equal if their lengths aren't equal
		return false;
	}

	for (int i = 0; i < sl_a->length; i++) { // for each index in both Lists
		// if the strings at this index in both Lists are not equal to each other
		if (strcmp(sl_a->list[i], sl_b->list[i]) != 0) {
			return false; // the Lists are not equal
		}
	}

	return true; // if no mismatches were found, the Lists are equal
}


void listRemove(StringList *sl, const int index) {
	assert(index >= 0);
	assert(index < sl->length); // make sure the index to be deleted actually exists

	free(sl->list[index]); // free the memory associated with this buffer before overwriting it

	for (int i = index; i < (sl->length - 1); i++) {
		sl->list[i] = sl->list[(i + 1)];
	}

	sl->length--;
}

void listRemoveElement(StringList *sl, const char *element) {
	int index = listIndexOf(sl, element);
	if (index != -1) { // if the element exists
		listRemove(sl, index);
	}
}

void listRemoveElements(StringList *sl, const char *element) {
	int i = 0;
	while (i < sl->length) {
		if (strcmp(sl->list[i], element) == 0) {
			listRemove(sl, i);
		} else {
			i++;
		}
	}
}

void listRemoveAll(StringList *sl, const StringList *to_remove) {
	for (int i = 0; i < to_remove->length; i++) { // for each index in the to_remove List
		listRemoveElements(sl, to_remove->list[i]);
	}
}

void listRemoveIf(StringList *sl, bool (*conditional_funct)(const char *)) {
	int i = 0;
	while (i < sl->length) {
		if (conditional_funct(sl->list[i])) {
			listRemove(sl, i);
		} else {
			i++;
		}
	}
}

void listClear(StringList *sl) {
	while (sl->length > 0) {
		listRemove(sl, 0);
	}
}


void listPrint(const StringList *sl) {
	printf("list (%d/%d) elements: \n", sl->length, sl->capacity);
	for (int i = 0; i < sl->length; i++) {
		printf("element[%d] = '%s'\n", i, sl->list[i]);
	}
}
