#include  "error_check.h"

#include <stdio.h>
void __assert_fail(const char *__assertion, const char *__file,
                    unsigned int __line, const char *__function) {
	printf("%s:%d:%s: Assertion %s failed.\n", __file, __line, __function, __assertion);
	for (;;) ;
}