#if defined __has_attribute
# if __has_attribute(nonnull)
#  define NON_NULL __attribute__((nonnull))
#  warning "Found __attribute__((nonnull))"
# else
#  warning "__attribute__((nonnull)) was not found"
# endif // __has_attribute(nonnull)
# else
#  warning "__has_attribute macros not found"
#endif // defined __has_attribute
