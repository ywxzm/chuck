#include "util/chk_string.h"
#include "util/chk_util.h"
#include "util/chk_error.h"
#include <stdlib.h>

chk_string *chk_string_new(const char *ptr,size_t len) {
	size_t cap;
	chk_string *str = calloc(1,sizeof(*str));
	if(!str) return NULL;
	str->ptr = str->buff;
	if(ptr && len > 0) {
		str->len = len;
		cap = chk_size_of_pow2(len+1);
		if(cap > CHK_STRING_INIT_SIZE) {
			str->ptr = calloc(1,cap);
			if(!str->ptr) {
				free(str->ptr);
				str = NULL;
			}
		}
		str->cap = cap > CHK_STRING_INIT_SIZE ? cap : CHK_STRING_INIT_SIZE;
		memcpy(str->ptr,ptr,len);
		str->ptr[len] = 0;		
	}
	return str;
}

chk_string *chk_string_new_cstr(const char *ptr) {
	return chk_string_new(ptr,strlen(ptr));
}

void chk_string_destroy(chk_string *str) {
	if(str->ptr != str->buff)
		free(str->ptr);
	free(str);
}

int32_t chk_string_append_cstr(chk_string *str,const char *ptr) {
	return chk_string_append(str,ptr,strlen(ptr));
}

int32_t chk_string_append(chk_string *str,const char *ptr,size_t len) {
	char  *tmp;
	size_t newlen,cap;
	if(!ptr || 0 >= len)
		return chk_error_invaild_argument;
	newlen = len + str->len;
	cap = chk_size_of_pow2(newlen+1);
	if(cap > CHK_STRING_INIT_SIZE) {
		if(str->ptr == str->buff)
			tmp = calloc(1,cap);
		else
			tmp = realloc(str->ptr,cap);
		if(!tmp) return chk_error_no_memory;
		str->ptr = tmp;		
	}
	memcpy(str->ptr + str->len,ptr,len);
	str->ptr[newlen] = 0;
	str->len = newlen;
	str->cap = cap > CHK_STRING_INIT_SIZE ? cap : CHK_STRING_INIT_SIZE;	
	return chk_error_ok;
}

size_t chk_string_size(chk_string *str) {
	return str->len;
}

int32_t chk_string_equal(chk_string *r,chk_string *l) {
	return strcmp(r->ptr,l->ptr);
}

const char *chk_string_c_str(chk_string *str) {
	return str->ptr;
}