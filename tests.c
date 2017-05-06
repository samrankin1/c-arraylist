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

	StringList* list = list_init();

	bool result = (
		(list->capacity == 10) &&
		(list->length == 0)
	);

	list_destroy(list);

	return result;
}

bool test_init_capacity() {
	announce_test("list_init_capacity");

	int capacity = 67;
	StringList* list = list_init_capacity(capacity);

	bool result = (list->capacity == capacity);

	list_destroy(list);

	return result;
}

bool test_contains() {
	announce_test("list_contains");

	StringList* list = list_init_capacity(1);
	list_add(list, "123");

	bool result = (
		(list_contains(list, "123")) &&
		(!list_contains(list, "234"))
	);

	list_destroy(list);

	return result;
}

bool test_add() {
	announce_test("list_add");

	StringList* list = list_init_capacity(1);
	list_add(list, "123");

	bool result = list_contains(list, "123");

	list_destroy(list);

	return result;
}

bool test_equals() {
	announce_test("list_equals");

	StringList* list_a = list_init_capacity(3);
	list_add(list_a, "1");
	list_add(list_a, "2");
	list_add(list_a, "3");

	StringList* list_b = list_init_capacity(3);
	list_add(list_b, "1");
	list_add(list_b, "2");
	list_add(list_b, "3");

	StringList* list_c = list_init_capacity(3);
	list_add(list_c, "a");
	list_add(list_c, "b");
	list_add(list_c, "c");

	bool result = (
		list_equals(list_a, list_b) &&
		!list_equals(list_b, list_c)
	);

	list_destroy(list_a);
	list_destroy(list_b);
	list_destroy(list_c);

	return result;
}

bool test_init_duplicate() {
	announce_test("list_init_duplicate");

	StringList* list = list_init_capacity(3);
	list_add(list, "1");
	list_add(list, "2");
	list_add(list, "3");

	StringList* new_list = list_init_duplicate(list);

	bool result = list_equals(list, new_list);

	list_destroy(list);
	list_destroy(new_list);

	return result;
}

bool test_clone() {
	announce_test("list_clone");

	StringList* list = list_init_capacity(3);
	list_add(list, "1");
	list_add(list, "2");
	list_add(list, "3");

	StringList* clone = list_clone(list);

	bool result = list_equals(list, clone);

	list_destroy(clone);
	list_destroy(list);

	return result;
}

bool test_get() {
	announce_test("list_get");

	StringList* list = list_init_capacity(3);
	list_add(list, "1");
	list_add(list, "2");
	list_add(list, "3");

	bool result = (
		(strcmp(list_get(list, 0), "1") == 0) &&
		(strcmp(list_get(list, 1), "2") == 0) &&
		(strcmp(list_get(list, 2), "3") == 0)
	);

	list_destroy(list);

	return result;
}

bool test_set() {
	announce_test("list_set");

	StringList* list = list_init_capacity(3);
	list_add(list, "1");
	list_add(list, "2");
	list_add(list, "3");

	list_set(list, 1, "abc");

	bool result = (strcmp(list_get(list, 1), "abc") == 0);

	list_destroy(list);

	return result;
}

bool test_index_of() {
	announce_test("list_index_of");

	StringList* list = list_init_capacity(3);
	list_add(list, "1");
	list_add(list, "2");
	list_add(list, "3");

	bool result = (
		(list_index_of(list, "3") == 2) &&
		(list_index_of(list, "2") == 1) &&
		(list_index_of(list, "1") == 0) &&
		(list_index_of(list, "0") == -1)
	);

	list_destroy(list);

	return result;
}

bool test_last_index_of() {
	announce_test("list_last_index_of");

	StringList* list = list_init_capacity(7);
	list_add(list, "0");
	list_add(list, "1");
	list_add(list, "2");
	list_add(list, "3");
	list_add(list, "3"); // i = 4
	list_add(list, "2"); // i = 5
	list_add(list, "1"); // i = 6

	bool result = (
		(list_last_index_of(list, "1") == 6) &&
		(list_last_index_of(list, "2") == 5) &&
		(list_last_index_of(list, "3") == 4) &&
		(list_last_index_of(list, "0") == 0) &&
		(list_last_index_of(list, "5") == -1)
	);

	list_destroy(list);

	return result;
}

bool test_capacity() {
	announce_test("list_capacity");

	StringList* list = list_init();

	bool result = (list_capacity(list) == list->capacity);

	list_destroy(list);

	return result;
}

bool test_set_capacity() {
	announce_test("list_set_capacity");

	StringList* list = list_init_capacity(15);

	int capacity = 43;
	for (int i = 0; i < capacity; i++) {
		list_add(list, "abc");
	}

	list_set_capacity(list, capacity);

	bool result_a = (list_capacity(list) == capacity);

	for (int i = 0; i < 17; i++) { // how many to overwrite?
		list_add(list, "abc");
	}

	list_set_capacity(list, capacity);

	bool result = (
		(result_a) &&
		(list_capacity(list) == capacity)
	);

	list_destroy(list);

	return result;
}

bool test_ensure_capacity() {
	announce_test("list_ensure_capacity");

	StringList* list = list_init_capacity(15);

	int capacity = 34;
	list_ensure_capacity(list, 22); // these numbers must be less than 'capacity'
	list_ensure_capacity(list, 18);
	list_ensure_capacity(list, capacity);

	bool result = (list_capacity(list) >= capacity);

	list_destroy(list);

	return result;
}

bool test_length() {
	announce_test("list_length");

	StringList* list = list_init_capacity(100);

	int length = 51;
	for (int i = 0; i < length; i++) {
		list_add(list, "abc");
	}

	bool result = (list_length(list) == length);

	list_destroy(list);

	return result;
}

bool test_trim_capacity() {
	announce_test("list_trim_capacity");

	StringList* list = list_init_capacity(5);

	for (int i = 0; i < 50; i++) {
		list_add(list, "abc");
	}

	list_trim_capacity(list);

	bool result = (list_capacity(list) == list_length(list));

	list_destroy(list);

	return result;
}

bool test_is_empty() {
	announce_test("list_is_empty");

	StringList* list = list_init();

	bool result_a = (list_is_empty(list) == true);

	list_add(list, "abc");

	bool result = (
		(result_a) &&
		(list_is_empty(list) == false)
	);

	list_destroy(list);

	return result;
}

bool test_add_all() {
	announce_test("list_add_all");

	StringList* list_a = list_init();
	list_add(list_a, "1");
	list_add(list_a, "2");
	list_add(list_a, "3");

	StringList* list_b = list_init();
	list_add(list_b, "a");
	list_add(list_b, "b");
	list_add(list_b, "c");

	list_add_all(list_a, list_b);

	StringList* expected = list_init();
	list_add(expected, "1");
	list_add(expected, "2");
	list_add(expected, "3");
	list_add(expected, "a");
	list_add(expected, "b");
	list_add(expected, "c");

	bool result = (list_equals(list_a, expected));

	list_destroy(list_a);
	list_destroy(list_b);
	list_destroy(expected);

	return result;
}

bool test_insert() {
	announce_test("list_insert");

	StringList* list = list_init();
	list_add(list, "a");
	list_add(list, "b");
	list_add(list, "c");

	list_insert(list, 1, "0");

	StringList* expected = list_init();
	list_add(expected, "a");
	list_add(expected, "0");
	list_add(expected, "b");
	list_add(expected, "c");

	bool result = (list_equals(list, expected));

	list_destroy(list);
	list_destroy(expected);

	return result;
}

bool test_insert_all() {
	announce_test("list_insert_all");

	StringList* list_a = list_init();
	list_add(list_a, "a");
	list_add(list_a, "b");
	list_add(list_a, "c");

	StringList* list_b = list_init();
	list_add(list_b, "1");
	list_add(list_b, "2");
	list_add(list_b, "3");

	list_insert_all(list_a, 0 , list_b);

	StringList* expected = list_init();
	list_add(expected, "1");
	list_add(expected, "2");
	list_add(expected, "3");
	list_add(expected, "a");
	list_add(expected, "b");
	list_add(expected, "c");

	bool result = (list_equals(list_a, expected));

	list_destroy(list_a);
	list_destroy(list_b);
	list_destroy(expected);

	return result;
}

bool test_remove() {
	announce_test("list_remove");

	StringList* list = list_init();
	list_add(list, "a");
	list_add(list, "b");
	list_add(list, "c");

	list_remove(list, 2);

	StringList* expected = list_init();
	list_add(expected, "a");
	list_add(expected, "b");

	bool result = (list_equals(list, expected));

	list_destroy(list);
	list_destroy(expected);

	return result;
}

bool test_remove_element() {
	announce_test("list_remove_element");

	StringList* list = list_init();
	list_add(list, "a");
	list_add(list, "b");
	list_add(list, "c");

	list_remove_element(list, "a");
	list_remove_element(list, "c");

	StringList* expected = list_init();
	list_add(expected, "b");

	bool result = (list_equals(list, expected));

	list_destroy(list);
	list_destroy(expected);

	return result;
}

bool test_remove_elements() {
	announce_test("list_remove_elements");

	StringList* list = list_init();
	list_add(list, "a");
	list_add(list, "b");
	list_add(list, "c");
	list_add(list, "a");
	list_add(list, "b");
	list_add(list, "c");

	list_remove_elements(list, "a");
	list_remove_elements(list, "c");

	StringList* expected = list_init();
	list_add(expected, "b");
	list_add(expected, "b");

	bool result = (list_equals(list, expected));

	list_destroy(list);
	list_destroy(expected);

	return result;
}

bool conditional_funct(const char * element) {
	for (int i = 0; i < strlen(element); i++) {
		if (*(element + i) == 'b') {
			return false;
		}
	}

	return true;
}

bool test_remove_if() {
	announce_test("list_remove_if");

	StringList* list = list_init();
	list_add(list, "123a");
	list_add(list, "456a");
	list_add(list, "789b");
	list_add(list, "012b");
	list_add(list, "345c");
	list_add(list, "678c");

	list_remove_if(list, &conditional_funct);

	StringList* expected = list_init();
	list_add(expected, "789b");
	list_add(expected, "012b");

	bool result = (list_equals(list, expected));

	list_destroy(list);
	list_destroy(expected);

	return result;
}

bool test_remove_all() {
	announce_test("list_remove_all");

	StringList* list = list_init();
	list_add(list, "a");
	list_add(list, "b");
	list_add(list, "c");
	list_add(list, "a");
	list_add(list, "b");
	list_add(list, "c");

	StringList* to_remove = list_init();
	list_add(to_remove, "c");
	list_add(to_remove, "a");

	list_remove_all(list, to_remove);

	StringList* expected = list_init();
	list_add(expected, "b");
	list_add(expected, "b");

	bool result = (list_equals(list, expected));

	list_destroy(list);
	list_destroy(to_remove);
	list_destroy(expected);

	return result;
}

bool test_clear() {
	announce_test("list_clear");

	StringList* list = list_init();
	list_add(list, "a");
	list_add(list, "b");
	list_add(list, "c");
	list_add(list, "a");
	list_add(list, "b");
	list_add(list, "c");

	list_clear(list);

	StringList* expected = list_init();
	// this line intentionally left blank

	bool result = (list_equals(list, expected));

	list_destroy(list);
	list_destroy(expected);

	return result;
}

/*
	TESTS BELOW THIS LINE HAVE NOT BEEN STAGED YET
*/

bool test_contains_all() {
	announce_test("list_contains_all");

	StringList* list = list_init();

	// TODO

	bool result = false;

	list_destroy(list);

	return result;
}

bool test_sublist() {
	announce_test("list_sublist");

	StringList* list = list_init();

	// TODO

	bool result = false;

	list_destroy(list);

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
		&test_init_duplicate,
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
