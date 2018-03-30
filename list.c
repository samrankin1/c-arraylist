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

StringList *list_init_capacity(const int capacity) {

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

StringList *list_init() {
	return list_init_capacity(10);
}

StringList *list_sublist(const StringList *sl, const int from, const int to) {
	assert(from >= 0); // make sure the 'from' index is not negative
	assert(to <= sl->length); // make sure 'to' is an existing index in 'list'
	assert(to > from);

	StringList *result = list_init_capacity((to - from)); // initialize a new StringList with initial capacity exactly the finished size

	for (int i = from; i < to; i++) { // for each index in [to, from)
		if (list_add(result, sl->list[i]) == NULL) { // add this element from the master list to the sublist
			list_destroy(result);
			return NULL;
		}
	}

	return result;
}

StringList *list_clone(const StringList *sl) {
	return list_sublist(sl, 0, sl->length); // return a sublist clone containing the entire original list
}

void list_destroy(StringList *sl) {
	for (int i = 0; i < sl->length; i++) {
		free(sl->list[i]); // free each string buffer
	}
	free(sl->list); // free string buffer array
	free(sl); // free struct memory
}


StringList *list_set_capacity(StringList *sl, const int capacity) {
	// warning -- providing a capacity less than the current List's length will drop those overflow elements AND free() their memory, rendering them inaccesible
	if (capacity < sl->length) { // if the new capacity is less than the current length
		for (int i = capacity; i < sl->length; i++) { // new capacity is the first index to be dropped, continue to the end of the current internal list
			free(sl->list[i]); // free the memeory at this previously-malloc'd address to prevent "lost pointers"
		}

		sl->length = capacity; // update the length field
	}

	char **newList = realloc(sl->list, (capacity * sizeof(char*))); // resize the memory allocated to this StringList's internal list
	if (newList == NULL) {
		return NULL;
	}

	sl->list = newList;
	sl->capacity = capacity;
	return sl;
}

StringList *list_ensure_capacity(StringList *sl, const int capacity) {
	if (capacity > sl->capacity) {
		return list_set_capacity(sl, capacity);
	} else {
		return sl;
	}
}

StringList *list_trim_capacity(StringList *sl) {
	return list_set_capacity(sl, sl->length);
}

StringList *_list_expand_auto(StringList *sl) {
	return list_set_capacity(sl, ((sl->capacity * 2) + 1)); // 2x current capacity plus 1
}


StringList *_list_set_unchecked(StringList *sl, const int index, const char *value) {

	char *copyBuf = malloc(strlen(value) + 1); // create a buffer for a copy of the 'value' string
	if (copyBuf == NULL) {
		return NULL;
	}

	strcpy(copyBuf, value); // copy the 'value' string into the buffer
	sl->list[index] = copyBuf; // set the pointer at the next index in the string to the buffer to permanize it
	return sl;
}

StringList *list_set(StringList *sl, const int index, const char *value) {
	// note: stores a carbon-copy of the 'value' string to make sure values are available until dismissed from the list
	// printf("setting index '%d' (len = '%d')\n", index, list->length);
	assert(index <= sl->length); // i.e. lists can only be expended one at a time by setting a value one index past the end of the list
	assert(index >= 0);

	if (index == sl->length) { // if this is a set() call for the n+1 element index (special case mentioned above)
		if ((sl->length + 1) > sl->capacity) { // if more capacity will be needed to store something at this index
		 	if (_list_expand_auto(sl) == NULL) { // expand the capacity to an automatically determined larger size
		 		return NULL;
		 	}
		}

		sl->length++; // increase the length regardless, there is a new valid index
	} else { // if this set() call will overwrite an existing element in the list
		free(sl->list[index]); // free the memory at the pointer to be overwritten
	}

	return _list_set_unchecked(sl, index, value);
}

StringList *list_add(StringList *sl, const char *value) {
	return list_set(sl, sl->length, value);
}

StringList *list_add_all(StringList *sl, const StringList *src) {
	for (int i = 0; i < src->length; i++) {
		if (list_add(sl, src->list[i]) == NULL) {
			return NULL;
		}
	}
	return sl;
}

StringList *list_insert(StringList *sl, const int index, const char *value) {
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

	if (_list_set_unchecked(sl, index, value) == NULL) { // set the value at this index to a copy of the 'value' string
		return NULL;
	}

	sl->length++; // increment the length, as one element has been added to the list
	return sl;
}

StringList *list_insert_all(StringList *sl, const int index, const StringList *src) {
	assert(index < sl->length);
	assert(index >= 0);

	int destLen = sl->length;
	int srcLen = src->length;

	// make sure the destination has enough space for both sets of elements
	if (list_ensure_capacity(sl, (destLen + srcLen)) == NULL) {
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


int list_capacity(const StringList *sl) {
	return sl->capacity;
}

int list_length(const StringList *sl) {
	return sl->length;
}

char *list_get(const StringList *sl, const int index) {
	assert(index < sl->length); // make sure the index being retrieved actually exists
	assert(index >= 0);

	return sl->list[index];
}

int list_index_of(const StringList *sl, const char *element) {
	for (int i = 0; i < sl->length; i++) {
		if (strcmp(sl->list[i], element) == 0) {
			return i;
		}
	}
	return -1;
}

int list_last_index_of(const StringList *sl, const char *element) {
	for (int i = (sl->length - 1); i >= 0; i--) {
		if (strcmp(sl->list[i], element) == 0) {
			return i;
		}
	}
	return -1;
}


bool list_is_empty(const StringList *sl) {
	return (sl->length == 0);
}

bool list_contains(const StringList *sl, const char *element) {
	return (list_index_of(sl, element) != -1); // index_of returns -1 if the element was not found
}

bool list_contains_all(const StringList *sl, const StringList *must_contain) {
	for (int i = 0; i < must_contain->length; i++) { // for each index in the must_contain List
		if (!list_contains(sl, must_contain->list[i])) { // if the target list does not contain this element
			return false; // the list does not "contain all"
		}
	}

	return true; // if no missing elements were found, 'list' contains all elements in 'must_contain'
}

bool list_equals(const StringList *sl_a, const StringList *sl_b) {
	if (sl_a->length != sl_b->length) { // the Lists cannot be equal if their lengths aren't equal
		return false;
	}

	for (int i = 0; i < sl_a->length; i++) { // for each index in both Lists
		if (strcmp(sl_a->list[i], sl_b->list[i]) != 0) { // if the strings at this index in both Lists are not equal to each other
			return false; // the Lists are not equal
		}
	}

	return true; // if no mismatches were found, the Lists are equal
}


void list_remove(StringList *sl, const int index) {
	assert(index >= 0);
	assert(index < sl->length); // make sure the index to be deleted actually exists

	free(sl->list[index]); // free the memory associated with this buffer before overwriting it

	for (int i = index; i < (sl->length - 1); i++) {
		sl->list[i] = sl->list[(i + 1)];
	}

	sl->length--;
}

void list_remove_element(StringList *sl, const char *element) {
	int index = list_index_of(sl, element);
	if (index != -1) { // if the element exists
		list_remove(sl, index);
	}
}

void list_remove_elements(StringList *sl, const char *element) {
	int i = 0;
	while (i < sl->length) {
		if (strcmp(sl->list[i], element) == 0) {
			list_remove(sl, i);
		} else {
			i++;
		}
	}
}

void list_remove_all(StringList *sl, const StringList *to_remove) {
	for (int i = 0; i < to_remove->length; i++) { // for each index in the to_remove List
		list_remove_elements(sl, to_remove->list[i]);
	}
}

void list_remove_if(StringList *sl, bool (*conditional_funct)(const char *)) {
	int i = 0;
	while (i < sl->length) {
		if (conditional_funct(sl->list[i])) {
			list_remove(sl, i);
		} else {
			i++;
		}
	}
}

void list_clear(StringList *sl) {
	while (sl->length > 0) {
		list_remove(sl, 0);
	}
}


void list_print(const StringList *sl) {
	printf("list (%d/%d) elements: \n", sl->length, sl->capacity);
	for (int i = 0; i < sl->length; i++) {
		printf("element[%d] = '%s'\n", i, sl->list[i]);
	}
}
