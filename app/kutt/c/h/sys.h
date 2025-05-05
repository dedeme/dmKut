
// sys ---------------------------------

// \s -> ()
void __sys_init(Val compiler_root);

// -----------------

// () -> s
Val __sys_compilerRoot (void);

// () -> s
Val __sys_getLocale (void);

// s -> ()
void __sys_setLocale (Val charset);
