#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "list.h"

void announce_test(const char * test_name) {
	printf("running test '%s'... ", test_name);
}

bool test_init() {
	announce_test("list_init");

	StringList* list = listNew();

	bool result = (
		(list->capacity == 10) &&
		(list->length == 0)
	);

	listDestroy(list);

	return result;
}

bool test_init_capacity() {
	announce_test("list_init_capacity");

	int capacity = 67;
	StringList* list = listNewCapacity(capacity);

	bool result = (list->capacity == capacity);

	listDestroy(list);

	return result;
}

bool test_contains() {
	announce_test("list_contains");

	StringList* list = listNewCapacity(1);
	listAdd(list, "123");

	bool result = (
		listContains(list, "123") &&
		!listContains(list, "234")
	);

	listDestroy(list);

	return result;
}

bool test_add() {
	announce_test("list_add");

	StringList* list = listNewCapacity(1);
	listAdd(list, "123");

	bool result = listContains(list, "123");

	listDestroy(list);

	return result;
}

bool test_equals() {
	announce_test("list_equals");

	StringList* list_a = listNewCapacity(3);
	listAdd(list_a, "1");
	listAdd(list_a, "2");
	listAdd(list_a, "3");

	StringList* list_b = listNewCapacity(3);
	listAdd(list_b, "1");
	listAdd(list_b, "2");
	listAdd(list_b, "3");

	StringList* list_c = listNewCapacity(3);
	listAdd(list_c, "a");
	listAdd(list_c, "b");
	listAdd(list_c, "c");

	bool result = (
		listEquals(list_a, list_b) &&
		!listEquals(list_b, list_c)
	);

	listDestroy(list_a);
	listDestroy(list_b);
	listDestroy(list_c);

	return result;
}

bool test_clone() {
	announce_test("list_clone");

	StringList* list = listNewCapacity(3);
	listAdd(list, "1");
	listAdd(list, "2");
	listAdd(list, "3");

	StringList* clone = listClone(list);

	bool result = listEquals(list, clone);

	listDestroy(clone);
	listDestroy(list);

	return result;
}

bool test_get() {
	announce_test("list_get");

	StringList* list = listNewCapacity(3);
	listAdd(list, "1");
	listAdd(list, "2");
	listAdd(list, "3");

	bool result = (
		(strcmp(listGet(list, 0), "1") == 0) &&
		(strcmp(listGet(list, 1), "2") == 0) &&
		(strcmp(listGet(list, 2), "3") == 0)
	);

	listDestroy(list);

	return result;
}

bool test_set() {
	announce_test("list_set");

	StringList* list = listNewCapacity(3);
	listAdd(list, "1");
	listAdd(list, "2");
	listAdd(list, "3");

	listSet(list, 1, "abc");

	bool result = (strcmp(listGet(list, 1), "abc") == 0);

	listDestroy(list);

	return result;
}

bool test_index_of() {
	announce_test("list_index_of");

	StringList* list = listNewCapacity(3);
	listAdd(list, "1");
	listAdd(list, "2");
	listAdd(list, "3");

	bool result = (
		(listIndexOf(list, "3") == 2) &&
		(listIndexOf(list, "2") == 1) &&
		(listIndexOf(list, "1") == 0) &&
		(listIndexOf(list, "0") == -1)
	);

	listDestroy(list);

	return result;
}

bool test_last_index_of() {
	announce_test("list_last_index_of");

	StringList* list = listNewCapacity(7);
	listAdd(list, "0");
	listAdd(list, "1");
	listAdd(list, "2");
	listAdd(list, "3");
	listAdd(list, "3"); // i = 4
	listAdd(list, "2"); // i = 5
	listAdd(list, "1"); // i = 6

	bool result = (
		(listLastIndexOf(list, "1") == 6) &&
		(listLastIndexOf(list, "2") == 5) &&
		(listLastIndexOf(list, "3") == 4) &&
		(listLastIndexOf(list, "0") == 0) &&
		(listLastIndexOf(list, "5") == -1)
	);

	listDestroy(list);

	return result;
}

bool test_capacity() {
	announce_test("list_capacity");

	StringList* list = listNew();

	bool result = (listCapacity(list) == list->capacity);

	listDestroy(list);

	return result;
}

bool test_set_capacity() {
	announce_test("list_set_capacity");

	StringList* list = listNewCapacity(15);

	int capacity = 43;
	for (int i = 0; i < capacity; i++) {
		listAdd(list, "abc");
	}

	listSetCapacity(list, capacity);

	bool result_a = (listCapacity(list) == capacity);

	for (int i = 0; i < 17; i++) { // how many to overwrite?
		listAdd(list, "abc");
	}

	listSetCapacity(list, capacity);

	bool result = (
		result_a &&
		(listCapacity(list) == capacity)
	);

	listDestroy(list);

	return result;
}

bool test_ensure_capacity() {
	announce_test("list_ensure_capacity");

	StringList* list = listNewCapacity(15);

	int capacity = 34;
	listEnsureCapacity(list, 22); // these numbers must be less than 'capacity'
	listEnsureCapacity(list, 18);
	listEnsureCapacity(list, capacity);

	bool result = (listCapacity(list) >= capacity);

	listDestroy(list);

	return result;
}

bool test_length() {
	announce_test("list_length");

	StringList* list = listNewCapacity(100);

	int length = 51;
	for (int i = 0; i < length; i++) {
		listAdd(list, "abc");
	}

	bool result = (listLength(list) == length);

	listDestroy(list);

	return result;
}

bool test_trim_capacity() {
	announce_test("list_trim_capacity");

	StringList* list = listNewCapacity(5);

	for (int i = 0; i < 50; i++) {
		listAdd(list, "abc");
	}

	listTrimCapacity(list);

	bool result = (listCapacity(list) == listLength(list));

	listDestroy(list);

	return result;
}

bool test_is_empty() {
	announce_test("list_is_empty");

	StringList* list = listNew();

	bool result_a = (listIsEmpty(list) == true);

	listAdd(list, "abc");

	bool result = (
		result_a &&
		(listIsEmpty(list) == false)
	);

	listDestroy(list);

	return result;
}

bool test_add_all() {
	announce_test("list_add_all");

	StringList* list_a = listNew();
	listAdd(list_a, "1");
	listAdd(list_a, "2");
	listAdd(list_a, "3");

	StringList* list_b = listNew();
	listAdd(list_b, "a");
	listAdd(list_b, "b");
	listAdd(list_b, "c");

	listAddAll(list_a, list_b);

	StringList* expected = listNew();
	listAdd(expected, "1");
	listAdd(expected, "2");
	listAdd(expected, "3");
	listAdd(expected, "a");
	listAdd(expected, "b");
	listAdd(expected, "c");

	bool result = listEquals(list_a, expected);

	listDestroy(list_a);
	listDestroy(list_b);
	listDestroy(expected);

	return result;
}

bool test_insert() {
	announce_test("list_insert");

	StringList* list = listNew();
	listAdd(list, "a");
	listAdd(list, "b");
	listAdd(list, "c");

	listInsert(list, 1, "0");

	StringList* expected = listNew();
	listAdd(expected, "a");
	listAdd(expected, "0");
	listAdd(expected, "b");
	listAdd(expected, "c");

	bool result = listEquals(list, expected);

	listDestroy(list);
	listDestroy(expected);

	return result;
}

bool test_insert_all() {
	announce_test("list_insert_all");

	StringList* list_a = listNew();
	listAdd(list_a, "a");
	listAdd(list_a, "b");
	listAdd(list_a, "c");

	StringList* list_b = listNew();
	listAdd(list_b, "1");
	listAdd(list_b, "2");
	listAdd(list_b, "3");

	listInsertAll(list_a, 0 , list_b);

	StringList* expected = listNew();
	listAdd(expected, "1");
	listAdd(expected, "2");
	listAdd(expected, "3");
	listAdd(expected, "a");
	listAdd(expected, "b");
	listAdd(expected, "c");

	bool result = listEquals(list_a, expected);

	listDestroy(list_a);
	listDestroy(list_b);
	listDestroy(expected);

	return result;
}

bool test_remove() {
	announce_test("list_remove");

	StringList* list = listNew();
	listAdd(list, "a");
	listAdd(list, "b");
	listAdd(list, "c");

	listRemove(list, 2);

	StringList* expected = listNew();
	listAdd(expected, "a");
	listAdd(expected, "b");

	bool result = listEquals(list, expected);

	listDestroy(list);
	listDestroy(expected);

	return result;
}

bool test_remove_element() {
	announce_test("list_remove_element");

	StringList* list = listNew();
	listAdd(list, "a");
	listAdd(list, "b");
	listAdd(list, "c");

	listRemoveElement(list, "a");
	listRemoveElement(list, "c");

	StringList* expected = listNew();
	listAdd(expected, "b");

	bool result = listEquals(list, expected);

	listDestroy(list);
	listDestroy(expected);

	return result;
}

bool test_remove_elements() {
	announce_test("list_remove_elements");

	StringList* list = listNew();
	listAdd(list, "a");
	listAdd(list, "b");
	listAdd(list, "c");
	listAdd(list, "a");
	listAdd(list, "b");
	listAdd(list, "c");

	listRemoveElements(list, "a");
	listRemoveElements(list, "c");

	StringList* expected = listNew();
	listAdd(expected, "b");
	listAdd(expected, "b");

	bool result = listEquals(list, expected);

	listDestroy(list);
	listDestroy(expected);

	return result;
}

/* remove if the element string does not contain the char 'b' */
bool conditional_funct(const char * element) {
	size_t len = strlen(element);
	for (int i = 0; i < len; i++) {
		if (*(element + i) == 'b') {
			return false;
		}
	}

	return true;
}

bool test_remove_if() {
	announce_test("list_remove_if");

	StringList* list = listNew();
	listAdd(list, "123a");
	listAdd(list, "456a");
	listAdd(list, "789b");
	listAdd(list, "012b");
	listAdd(list, "345c");
	listAdd(list, "678c");

	listRemoveIf(list, &conditional_funct);

	StringList* expected = listNew();
	listAdd(expected, "789b");
	listAdd(expected, "012b");

	bool result = listEquals(list, expected);

	listDestroy(list);
	listDestroy(expected);

	return result;
}

bool test_remove_all() {
	announce_test("list_remove_all");

	StringList* list = listNew();
	listAdd(list, "a");
	listAdd(list, "b");
	listAdd(list, "c");
	listAdd(list, "a");
	listAdd(list, "b");
	listAdd(list, "c");

	StringList* to_remove = listNew();
	listAdd(to_remove, "c");
	listAdd(to_remove, "a");

	listRemoveAll(list, to_remove);

	StringList* expected = listNew();
	listAdd(expected, "b");
	listAdd(expected, "b");

	bool result = listEquals(list, expected);

	listDestroy(list);
	listDestroy(to_remove);
	listDestroy(expected);

	return result;
}

bool test_clear() {
	announce_test("list_clear");

	StringList* list = listNew();
	listAdd(list, "a");
	listAdd(list, "b");
	listAdd(list, "c");
	listAdd(list, "a");
	listAdd(list, "b");
	listAdd(list, "c");

	listClear(list);

	StringList* expected = listNew();
	// this line intentionally left blank

	bool result = listEquals(list, expected);

	listDestroy(list);
	listDestroy(expected);

	return result;
}

bool test_contains_all() {
	announce_test("list_contains_all");

	StringList* list = listNew();
	listAdd(list, "a");
	listAdd(list, "b");
	listAdd(list, "c");
	listAdd(list, "d");
	listAdd(list, "e");
	listAdd(list, "f");


	StringList* sublist = listNew();
	listAdd(sublist, "f");
	listAdd(sublist, "a");
	listAdd(sublist, "d");
	listAdd(sublist, "e");
	listAdd(sublist, "b");
	listAdd(sublist, "f");

	bool result_a = listContainsAll(list, sublist);

	listAdd(sublist, "z"); // add an element not in the original list

	bool result = (
		result_a &&
		!listContainsAll(list, sublist)
	);

	listDestroy(list);
	listDestroy(sublist);

	return result;
}

/*
	TESTS BELOW THIS LINE HAVE NOT BEEN STAGED YET
*/

bool test_sublist() {
	announce_test("list_sublist");

	StringList* list = listNew();

	// TODO

	bool result = false;

	listDestroy(list);

	return result;
}

int main() {

	// add unit tests here and they will automatically be detected and executed
	bool (*tests[])() = {
		&test_init,
		&test_init_capacity,
		&test_contains,
		&test_add,
		&test_equals,
		&test_clone,
		&test_get,
		&test_set,
		&test_index_of,
		&test_last_index_of,
		&test_capacity,
		&test_set_capacity,
		&test_ensure_capacity,
		&test_length,
		&test_trim_capacity,
		&test_is_empty,
		&test_add_all,
		&test_insert,
		&test_insert_all,
		&test_remove,
		&test_remove_element,
		&test_remove_elements,
		&test_remove_if,
		&test_remove_all,
		&test_clear,
		&test_contains_all,
	};

	int test_count = sizeof(tests) / sizeof(bool (*)());
	printf("running %d test(s)...\n\n", test_count);
	for (int i = 0; i < test_count; i++) {
		bool result = tests[i]();

		if (result) {
			printf("PASS\n");
		} else {
			printf("FAIL\n\n");
		}

		assert(result); // halt testing on first failure
	}

	printf("\n");
	printf("%d tests completed successfully!\n", test_count);

	return 0;
}
