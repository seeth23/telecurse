#ifndef __TC_TYPES_H
#define __TC_TYPES_H

enum border_type {
	border_default,
	border_type1,
	border_type2,
	border_type3,
};

//extern enum border_type t;

enum free_type {
	free_prompt,
	free_menu,
	free_info,
};

typedef struct CenterCoordinates {
	int y, x;
} centercords_t;

#endif
