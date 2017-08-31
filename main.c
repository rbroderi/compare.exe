#include <stdio.h>
#include <stdlib.h>
#include "argp.h"
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <float.h>
const char * argp_program_version = "2.0";
const char * argp_program_bug_address = "Richard Broderick";
static char doc[] = "Compares two numbers or strings";
static char args_doc[] = "ARG1 ARG2";
static struct argp_option options[] = {
	{"string",'s', 0,0,"Compare two strings"},
	{"number",'n',0,0,"Compare two numbers"},
	{"case-Insensitive",'i', 0, 0,"Case Insensitive Comparision (for strings)"}	,
	{"equality",'e', 0, 0,"test for equality instead of greater, equal or less than"},
	{0}
};
struct arguments {
	enum {
		STRING_MODE,
		NUMBER_MODE,
		CASE_INSENSITIVE_MODE,
	}
	mode;
	bool isCaseInsensitive;
	bool isEqualityMode;
	char * args[2];
}
;
static error_t parse_opt(int key, char * arg, struct argp_state * state) {
	struct arguments * arguments = state -> input;
	switch (key) {
		case 's':
		arguments -> mode = STRING_MODE;
		break;
		case 'n':
		arguments -> mode = NUMBER_MODE;
		break;
		case 'i':
		arguments -> isCaseInsensitive = true;
		break;
		case 'e':
		arguments -> isEqualityMode = true;
		break;
		case ARGP_KEY_ARG:
		arguments -> args[state -> arg_num] = arg;
		break;
		case ARGP_KEY_END:
		if (state -> arg_num > 2) {
			fputs("Too many arguments!\n", stdout);
			argp_usage(state);
		}
		if (state -> arg_num < 2) {
			fputs("Too few arguments!\n", stdout);
			argp_usage(state);
		}
		default:
		return ARGP_ERR_UNKNOWN;
	}
	return 0;
}
static struct argp argp = {
	options,
	parse_opt,
	args_doc,
	doc,
	0,
	0,
	0
}
;
bool almosteq(double a, double b) {
	return (fabs(a - b) < (DBL_EPSILON * fabs(a + b)));
}
int checkDouble(char * aa, char * bb) {
	double a = atof(aa);
	double b = atof(bb);
	if (almosteq(a, b)) {
		return 0;
	}
	else if (a > b) {
		return 1;
	}
	else {
		return -1;
	}
}
int checkString(char * a, char * b) {
	int ret = strcmp(a, b);
	if (ret < 0) {
		return -1;
	}
	else if (ret > 0) {
		return 1;
	}
	else {
		return 0;
	}
}
int main(int argc, char * argv[]) {
	struct arguments arguments;
	arguments.mode = NUMBER_MODE;
	arguments.isCaseInsensitive = false;
	argp_parse( & argp, argc, argv, 0, 0, & arguments);
	if (arguments.mode == NUMBER_MODE) {
		if(arguments.isEqualityMode == false){
			printf("%d",checkDouble(arguments.args[0], arguments.args[1]));
		}
		else{
			bool test = checkDouble(arguments.args[0], arguments.args[1]);
			test==0?fputs("0",stdout):fputs("1",stdout);
		}
	}
	else if (arguments.mode == STRING_MODE) {
		int cs;
		if (arguments.isCaseInsensitive == false) {
			cs = checkString(arguments.args[0], arguments.args[1]);
		}
		else {
			char * p = arguments.args[0];
			for (; * p; ++p) * p = tolower( * p);
			char * p2 = arguments.args[1];
			for (; * p2; ++p2) * p2 = tolower( * p2);
			cs = checkString(arguments.args[0], arguments.args[1]);
		}
		if(arguments.isEqualityMode == false){
			printf("%d",cs);
		}
		else{
			cs = cs==0?0:1;
			printf("%d",cs);
		}
	}
}
