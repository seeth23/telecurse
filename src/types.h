#ifndef __TC_TYPES__
#define __TC_TYPES__

enum border_type {
	border_default,
	border_type1,
	border_type2,
	border_type3,

};

extern enum border_type t;

struct term_info {
	int maxx;
	int maxy;
};

struct user_info {


};

enum free_type {
	free_prompt,
	free_menu,
	free_input,
};

#endif
