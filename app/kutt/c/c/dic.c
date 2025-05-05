
// dic ---------------------------------

// Auxiliar of __js_ro
// Map<char>->{s}
Val __dic_from_smap (Map *m) {
  Val r = __arr_new();
  EACH(m, Kv, el) {
    Val e = __arr_new();
    __arr_push(e, (Val)kv_key(el));
    __arr_push(e, (Val)((char *)kv_value(el)));
    __arr_push(r, e);
  }_EACH
  return r;
}

// {A},s -> [A]
Val __dic_get(Val d, Val key) {
  Varr *a = d.a;
  Val *p = a->begin;
  Val *end = a->end;
  while (p < end) {
    Varr *kv = (*p++).a;
    if (!strcmp(kv->begin[0].s, key.s)) return __rs_some(kv->begin[1]);
  }
  return  __arr_new();
}

// i,*[[s,A]]->{A}
Val __dic_new_from_carr (Val n, Val *els) {
  Val r = __arr_new();
  REPEAT(n.i) {
    Varr *kv = (*els++).a;
    __dic_put(r, kv->begin[0], kv->begin[1]);
  }_REPEAT
  return r;
}

// {A},s,A->()
void __dic_put(Val d, Val key, Val value) {
  int new = TRUE;
  char *k = key.s;
  Varr *a = d.a;
  Val *p = a->begin;
  Val *end = a->end;
  while (p < end) {
    Varr *kv = (*p++).a;
    if (!strcmp(k, kv->begin[0].s)) {
      kv->begin[1] = value;
      new = FALSE;
      break;
    }
  }
  if (new) __arr_push(d, __arr_new_from_carr((Val)2, (Val[]){key, value}));
}

// {A},(A->s)->s
Val __dic_to_js (Val d, Val (*fn)(Val)) {
  Map *r = map_new();
  Varr *va = d.a;
  Val *p = va->begin;
  Val *end = va->end;
  while (p < end) {
    Varr *kv = (*p++).a;
    arr_push((Arr *)r, kv_new((kv->begin[0]).s, fn(kv->begin[1]).s));
  }
  return __js_wo(__dic_from_smap(r));
}

// Auxiliar of __js_wo
// {A}->Map<char>
Map *__dic_to_smap (Val a) {
  Map *r = map_new();
  Varr *va = a.a;
  Val *p = va->begin;
  Val *end = va->end;
  while (p < end) {
    Varr *kv = (*p++).a;
    arr_push((Arr *)r, kv_new((kv->begin[0]).s, (kv->begin[1]).s));
  }
  return r;
}

