#if defined __has_include
# if __has_include(<stdatomic.h>)
#  warning "Found <stdatomic.h>"
# else
#  warning "<stdatomic.h> not found"
# endif
#else
# warning "__has_include macros not found"
#endif // defined __has_include
