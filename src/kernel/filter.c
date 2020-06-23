#include "kernel.h"

typedef struct stri {
	char* str;
	int len;
}stri;

typedef struct filter_rule {
	int rule_id;
	stri filter_dir;
	stri* next;
}f_rule;

static f_rule f_rule_0 = NULL;
static f_rule f_rule_1 = {0, {NULL, 0}, NULL};

bool is_monitoring(){
	return f_rule_0 == NULL ? true : false;
}

