#pragma once

typedef struct {
	char **list;
	int length;
	int capacity;
} StringList;

StringList *list_init();
StringList *list_init_capacity(const int capacity);
StringList *list_sublist(const StringList *list, const int from, const int to);
StringList *list_clone(const StringList *list);
void list_destroy(StringList *list);

StringList *list_set_capacity(StringList *list, const int new_capacity);
StringList *list_ensure_capacity(StringList *list, const int min_capacity);
StringList *list_trim_capacity(StringList *list);

StringList *list_set(StringList *list, const int index, const char *value);
StringList *list_add(StringList *list, const char *value);
StringList *list_add_all(StringList *list, const StringList *source);
StringList *list_insert(StringList *list, const int index, const char *value);
StringList *list_insert_all(StringList *list, const int index, const StringList *source);

int list_capacity(const StringList *list);
int list_length(const StringList *list);
char *list_get(const StringList *list, const int index);
int list_index_of(const StringList *list, const char *element);
int list_last_index_of(const StringList *list, const char *element);

bool list_is_empty(const StringList *list);
bool list_contains(const StringList *list, const char *element);
bool list_contains_all(const StringList *list, const StringList *must_contain);
bool list_equals(const StringList *list_a, const StringList *list_b);

void list_remove(StringList *list, const int index);
void list_remove_element(StringList *list, const char *element);
void list_remove_elements(StringList *list, const char *element);
void list_remove_if(StringList *list, bool(*conditional_funct)(const char *));
void list_remove_all(StringList *list, const StringList *to_remove);
void list_clear(StringList *list);

void list_print(const StringList *list);
