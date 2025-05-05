
// b64 ------------------------------

// s -> s
Val __b64_decode (char *pos, Val b64) {
  char *r;
  TRY {
    r = b64_decode(b64.s);
  } CATCH(e) {
    EXC_KUTT(str_f("%s %s", pos, e));
  }_TRY
  return (Val) r;
}

// s -> s
Val __b64_encode (Val s) {
  return (Val)b64_encode(s.s);
}

