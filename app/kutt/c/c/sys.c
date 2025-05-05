
// sys ---------------------------------

static Val __sys_compiler_root_v = (Val) "";

static void __sys_shandler (int sig) {
  ___built_throw((Val)"buil-in", (Val)"Arithmetic exception");
}
static void __sys_sshandler (int sig) {
  ___built_throw((Val)"buil-in", (Val)"Segmentation violation");
}

// \s -> ()
void __sys_init(Val compiler_root) {
  sys_init();
  signal (SIGFPE, __sys_shandler);
  signal (SIGSEGV, __sys_sshandler);
  __sys_compiler_root_v = compiler_root;
}

// -----------------

// () -> s
Val __sys_compilerRoot (void) {
  return __sys_compiler_root_v;
}

// () -> s
Val __sys_getLocale (void) {
  return (Val)sys_get_locale();
}

// s -> ()
void __sys_setLocale (Val charset) {
  return sys_set_locale(charset.s);
}
