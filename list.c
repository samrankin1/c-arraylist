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

/*

	TODO:
	;

	;

	;
	void list_remove_if(StringList* list, bool (*conditional_funct)(const char *));
	void liar_remove_all(StringList* list, const StringList* to_remove);
	void list_trim_capacity(StringList* list);
	void list_clear(StringList* list);
	void list_ensure_capacity(StringList* list, const int min_capacity);

	StringList* list_sublist(const StringList* list, const int from, const int to);
	StringList* list_clone(const StringList* source); // sublist from 0 to source->length
	bool list_contains(const StringList* list, const char * element);
	int list_index_of(const StringList* list, const char * element); // update delete methods to use this
	int list_last_index_of(const StringList* list, const char * element);
	int list_size(const StringList* list);
	bool list_is_empty(const StringList* list); // return (list_length(list) == 0);
	void list_sort(const StringList* list, int (*comparator_funct)(const char *, const char *))); // quicksort
	char*[] list_to_array(const StringList* list);
	bool list_equal(const StringList* list, const StringList* other_list);
	bool list_contains_all(const StringList* list, const StringList* must_contain);
	char * list_to_string(const StringList* list);

**/

StringList* list_init(const int capacity) {
	StringList* result = malloc(sizeof(StringList));
	result->list = malloc(capacity * sizeof(char*));
	result->length = 0;
	result->capacity = capacity;

	return result;
}

StringList* list_init_duplicate(const StringList* source) {
	StringList* result = list_init(list_size(source)); // create a new StringList with the capacity of the source List'
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

void _list_expand_auto(StringList* list) {
	list_set_capacity(list, ((list->capacity * 1.5) + 1)); // 150% current capacity plus 1
}

char * list_get(StringList* list, const int index) {
	assert(index < list->length); // make sure the index being retrieved actually exists

	return list->list[index];
}

void list_set(StringList* list, const int index, const char * value) {
	// note: stores a carbon-copy of the 'value' string to make sure values are available until dismissed from the list
	assert(index <= list->length); // i.e. lists can only be expended one at a time by setting a value one index past the end of the list

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

void list_add(StringList* list, const char * value) {
	list_set(list, list->length, value);
}

void list_add_all(StringList* list, StringList* source) {
	for (int i = 0; i < source->length; i++) {
		list_add(list, source[i]);
	}
}

void list_insert(StringList* list, const int index, const char * value) {
	list_ensure_capacity(list->length + 1); // make sure there's room for the added element

	for (int i = (list->length - 1); i > index; i--) { // descend down the internal list
		list->list[(i + 1)] = list->list[i];
	}

	// TODO: insert value
}

void list_insert_all(StringList* list, const int index, StringList* source) {
	// move the entire tail of the array far enough back to fit the elements in source
	// TODO here
}

void list_remove(StringList* list, const int index) {
	assert(index < list->length); // make sure the index to be deleted actually exists

	for (int i = index; i < (list->length - 1); i++) {
		list->list[i] = list->list[(i + 1)];
	}

	list->length--;
}

void list_remove_element(StringList* list, const char * element) {
	for (int i = 0; i < list->length; i++) {
		if (strcmp(list->list[i], element) == 0) {
			list_del(list, i);
			break;
		}
	}
}

void list_remove_elements(StringList* list, const char * element) {
	int i = 0;
	while (i < list->length) {
		if (strcmp(list->list[i], element) == 0) {
			list_del(list, i);
		} else {
			i++;
		}
	}
}

void list_print(const StringList* list) {
	printf("list elements: %d/%d\n", list->length, list->capacity);
	for (int i = 0; i < list->length; i++) {
		printf("element[%d] = '%s'\n", i, list->list[i]);
	}
}

int main() {
	StringList* list = list_init(5);

	list_add(list, "a");
	list_add(list, "b");
	list_add(list, "b");
	list_add(list, "d");
	list_add(list, "e");

	list_del(list, 3);

	list_del_elements(list, "b");

	list_dump(list);

	char * get = list_get(list, 2);
	printf("got str: '%s'\n", get);

	list_destroy(list);

	char * test = malloc(5);
	strcpy(test, "heyo");
	test = realloc(test, 2);
}
