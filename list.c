#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

typedef struct {
	char** list;
	int length;
	int capacity;
} StringList;

/**
	TODO:
	char * list_to_string(const StringList* list);
	void list_sort(const StringList* list, int (*comparator_funct)(const char *, const char *))); // quicksort
**/

void list_add_all(StringList* list, const StringList* source);

StringList* list_init_capacity(const int capacity) {
	StringList* result = malloc(sizeof(StringList));
	result->list = malloc(capacity * sizeof(char*));
	result->length = 0;
	result->capacity = capacity;

	return result;
}

StringList* list_init() {
	return list_init_capacity(10);
}

StringList* list_init_duplicate(const StringList* source) {
	StringList* result = list_init_capacity(source->length); // create a new StringList with the capacity of the source List
	list_add_all(result, source);
	return result;
}

void list_destroy(StringList* list) {
	for (int i = 0; i < list->length; i++) {
		free(list->list[i]);
	}
	free(list->list);
	free(list);
}

int list_capacity(const StringList* list) {
	return (list->capacity);
}

void list_set_capacity(StringList* list, const int new_capacity) {
	// warning -- providing a new_capacity less than the current List's length will drop those overflow elements AND free() their memory, rendering them inaccesible
	if (new_capacity < list->length) { // if the new capacity is less than the current length
		for (int i = new_capacity; i < list->length; i++) { // new_capacity is the first index to be dropped, continue to the end of the current internal list
			free(list->list[i]); // free the memeory at this previously-malloc'd address to prevent "lost pointers"
		}

		list->length = new_capacity; // update the length property to prevent accidential Segfaults
	}

	list->list = realloc(list->list, (new_capacity * sizeof(char*))); // resize the memory allocated to this StringList's internal list

	list->capacity = new_capacity; // update the list's stored capacity value
}

void list_ensure_capacity(StringList* list, const int min_capacity) {
	if (min_capacity > list->capacity) {
		list_set_capacity(list, min_capacity);
	}
}

void list_trim_capacity(StringList* list) {
	list_set_capacity(list, list->length);
}

void _list_expand_auto(StringList* list) {
	list_set_capacity(list, ((list->capacity * 1.5) + 1)); // 150% current capacity plus 1
}

char * list_get(const StringList* list, const int index) {
	assert(index < list->length); // make sure the index being retrieved actually exists
	assert(index >= 0);

	return list->list[index];
}

void list_set(StringList* list, const int index, const char * value) {
	// note: stores a carbon-copy of the 'value' string to make sure values are available until dismissed from the list
	// printf("setting index '%d' (len = '%d')\n", index, list->length);
	assert(index <= list->length); // i.e. lists can only be expended one at a time by setting a value one index past the end of the list
	assert(index >= 0);

	if (index == list->length) { // if this is a set() call for the n+1 element index (special case mentioned above)
		if ((list->length + 1) > list->capacity) { // if more capacity will be needed to store something at this index
		 	_list_expand_auto(list); // expand the capacity to an automatically determined larger size
		}

		list->length++; // increase the length regardless, there is a new valid index
	}

	char * copy_buff = malloc((strlen(value) + 1) * sizeof(char)); // create a buffer for a copy of the 'value' string
	strcpy(copy_buff, value); // copy the 'value' string into the buffer
	list->list[index] = copy_buff; // set the pointer at the next index in the string to the buffer to permanize it
}

int list_index_of(const StringList* list, const char * element) {
	for (int i = 0; i < list->length; i++) {
		if (strcmp(list->list[i], element) == 0) {
			return i;
		}
	}

	return -1;
}

int list_last_index_of(const StringList* list, const char * element) {
	for (int i = (list->length - 1); i >= 0; i--) {
		if (strcmp(list->list[i], element) == 0) {
			return i;
		}
	}

	return -1;
}

void list_add(StringList* list, const char * value) {
	list_set(list, list->length, value);
}

void list_add_all(StringList* list, const StringList* source) {
	for (int i = 0; i < source->length; i++) {
		list_add(list, source->list[i]);
	}
}

void list_insert(StringList* list, const int index, const char * value) {
	assert(index < list->length);
	assert(index >= 0);

	list_ensure_capacity(list, (list->length + 1)); // make sure there's room for the added element // TODO: auto expand?

	for (int i = (list->length - 1); i >= index; i--) { // descend down the internal list
		list->list[(i + 1)] = list->list[i]; // no copy needed, just move around the existing pointers
	}

	list_set(list, index, value); // set the value at this index to a copy of the 'value' string
	list->length++; // increment the length, as one element has been added to the list
}

void list_insert_all(StringList* list, const int index, const StringList* source) {
	assert(index < list->length);
	assert(index >= 0);

	list_ensure_capacity(list, (list->length + source->length)); // make sure the destination has enough space for both sets of elements
	list->length += source->length;

	for (int i = index; i < (list->length - source->length); i++) {
		list_set(list, (i + source->length), list->list[i]); // move each string to the end of the new list
	}

	for (int i = 0; i < source->length; i++) {
		list_set(list, (i + index), source->list[i]);
	}
}

void list_remove(StringList* list, const int index) {
	assert(index >= 0);
	assert(index < list->length); // make sure the index to be deleted actually exists

	free(list->list[index]); // free the memory associated with this buffer before overwriting it

	for (int i = index; i < (list->length - 1); i++) {
		list->list[i] = list->list[(i + 1)];
	}

	list->length--;
}

void list_remove_element(StringList* list, const char * element) {
	int index = list_index_of(list, element);
	if (index != -1) { // if the element exists
		list_remove(list, index);
	}
}

void list_remove_elements(StringList* list, const char * element) {
	int i = 0;
	while (i < list->length) {
		if (strcmp(list->list[i], element) == 0) {
			list_remove(list, i);
		} else {
			i++;
		}
	}
}

void list_remove_all(StringList* list, const StringList* to_remove) {
	for (int i = 0; i < to_remove->length; i++) { // for each index in the to_remove List
		list_remove_elements(list, to_remove->list[i]);
	}
}

void list_remove_if(StringList* list, bool (*conditional_funct)(const char *)) {
	int i = 0;
	while (i < list->length) {
		if (conditional_funct(list->list[i])) {
			list_remove(list, i);
		} else {
			i++;
		}
	}
}

void list_clear(StringList* list) {
	while (list->length > 0) {
		list_remove(list, 0);
	}
}

void list_print(const StringList* list) {
	printf("list elements: %d/%d\n", list->length, list->capacity);
	for (int i = 0; i < list->length; i++) {
		printf("element[%d] = '%s'\n", i, list->list[i]);
	}
}

int list_length(const StringList* list) {
	return (list->length);
}

bool list_is_empty(const StringList* list) {
	return (list->length == 0);
}

bool list_contains(const StringList* list, const char * element) {
	return (list_index_of(list, element) != -1); // index_of returns -1 if the element was not found
}

bool list_contains_all(const StringList* list, const StringList* must_contain) {
	for (int i = 0; i < must_contain->length; i++) { // for each index in the must_contain List
		if (!list_contains(list, must_contain->list[i])) { // if the target list does not contain this element
			return false; // the list does not "contain all"
		}
	}

	return true; // if no missing elements were found, 'list' contains all elements in 'must_contain'
}

bool list_equals(const StringList* list_a, const StringList* list_b) {
	if (list_a->length != list_b->length) { // the Lists cannot be equal if their lengths aren't equal
		return false;
	}

	for (int i = 0; i < list_a->length; i++) { // for each index in both Lists
		if (strcmp(list_a->list[i], list_b->list[i]) != 0) { // if the strings at this index in both Lists are not equal to each other
			return false; // the Lists are not equal
		}
	}

	return true; // if no mismatches were found, the Lists are equal
}

StringList* list_sublist(const StringList* list, const int from, const int to) {
	assert(from >= 0); // make sure the 'from' index is not negative
	assert(to <= list->length); // make sure 'to' is an existing index in 'list'
	assert(to > from);

	StringList* result = list_init_capacity((to - from)); // initialize a new StringList with initial capacity exactly the finished size

	for (int i = from; i < to; i++) { // for each index in [to, from)
		list_add(result, list->list[i]); // add this element from the master list to the sublist
	}

	return result;
}

StringList* list_clone(const StringList* list) {
	return list_sublist(list, 0, list->length); // return a sublist clone containing the entire original list
}
