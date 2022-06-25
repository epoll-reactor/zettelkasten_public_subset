#if defined __has_builtin
# if __has_builtin(__builtin_expect)
#  warning "Found __builtin_expect"
# else
#  warning "__builtin_expect was not found"
# endif // __has_builtin(__builtin_expect)
#else
# warning "__has_builtin was not found"
#endif // defined __has_builtin
