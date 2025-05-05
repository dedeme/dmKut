
// INTERFACE ----------------------------------------


// -------------------------------------
//02-built_in
// -------------------------------------

void ___built_jump (int reference) {
Exc *exc = exc_get();
longjmp(*((jmp_buf *)arr_pop(exc->buf)), reference);
}
char *___built_mpath(char *module_id) {
char *data = file_read(path_cat(__sys_compilerRoot().s, "index.tb", NULL));
Arr *tb = js_ra(data);
EACH(tb, char, e) {
Arr *fs = js_ra(e);
if (!strcmp(js_rs(arr_get(fs, 1)), module_id))
return js_rs(arr_get(fs, 0));
}_EACH
return "???";
}
char *___built_mpos(char *module_id, int ln) {
return str_f("%s:%d:", ___built_mpath(module_id), ln);
}
Val ___built_divi(Val pos, Val n1, Val n2) {
if (n2.i == 0) ___built_throw(pos, (Val)"Division by 0");
return (Val) (n1.i / n2.i);
}
Val ___built_divf(Val pos, Val n1, Val n2) {
if (math_eq(n2.f, 0)) ___built_throw(pos, (Val)"Division by 0");
return (Val) (n1.f / n2.f);
}
Val ___built_eq (Val type, Val value1, Val value2) {
char *t = __arr_get((Val)"", type, (Val)0).s;
if (*t == 'b') return (Val)(value1.b == value2.b);
if (*t == 'i') return (Val)(value1.i == value2.i);
if (*t == 'f') return (Val)(math_eq(value1.f, value2.f));
if (*t == 's') return (Val)(!strcmp(value1.s, value2.s));
if (*t == 'a') {
if (__arr_size(value1).i != __arr_size(value2).i) return (Val)FALSE;
Val atp = __arr_get((Val)"", __arr_get((Val)"", type, (Val)1), (Val)0);
Varr *a1 = value1.a;
Varr *a2 = value2.a;
Val *e1 = a1->begin;
Val *e2 = a2->begin;
Val *end = a1->end;
while (e1 < end) {
if (!___built_eq(atp, *e1++, *e2++).b) return (Val)FALSE;
}
return (Val)TRUE;
}
if (*t == 'd') {
if (__arr_size(value1).i != __arr_size(value2).i) return (Val)FALSE;
Val dtp = __arr_get((Val)"", __arr_get((Val)"", type, (Val)1), (Val)0);
Varr *a = value1.a;
Val *e = a->begin;
Val *end = a->end;
while (e < end) {
Varr *kv = (*e++).a;
Val rs = __dic_get(value2, kv->begin[0]);
if (
!__arr_size(rs).i |
!___built_eq(dtp, kv->begin[1], rs.a->begin[0]).b
) return (Val)FALSE;
}
return (Val)TRUE;
}
if (*t == 't') {
Varr *atp = __arr_get((Val)"", type, (Val)1).a;
Val *etp = atp->begin;
Val *endtp = atp->end;
Varr *a1 = value1.a;
Varr *a2 = value2.a;
Val *e1 = a1->begin;
Val *e2 = a2->begin;
while (etp < endtp) {
if (!___built_eq(*etp++, *e1++, *e2++).b) return (Val)FALSE;
}
return (Val)TRUE;
}
return (Val)(value1.o == value2.o);
}
Val ___built_to_str_trace (Val type, Val value) {
char *t = __arr_get((Val)"", type, (Val)0).s;
return (*t == 's')
? __js_ws(value)
: ___built_to_str(type, value)
;
}
Val ___built_to_str (Val type, Val value) {
char *t = __arr_get((Val)"", type, (Val)0).s;
if (*t == 'b') return value.b ? (Val)"true" : (Val)"false";
if (*t == 'i') return __math_itos(value);
if (*t == 'f') {
char *r = math_ftos(value.f, 9);
if (str_cindex(r, '.') == -1) r = str_f("%s.0", r);
return (Val)r;
}
if (*t == 's') return value;
if (*t == 'a') {
Val atp = __arr_get((Val)"", __arr_get((Val)"", type, (Val)1), (Val)0);
Arr *bf = arr_new(); // [s.]
Varr *a = value.a;
Val *e = a->begin;
Val *end = a->end;
while (e < end) {
arr_push(bf, ___built_to_str_trace(atp, *e++).s);
}
return (Val)str_f("[%s]", arr_join(bf, ","));
}
if (*t == 'd') {
Val dtp = __arr_get((Val)"", (__arr_get((Val)"", type, (Val)1)), (Val)0);
Arr *bf = arr_new(); // [s.]
Varr *a = value.a;
Val *e = a->begin;
Val *end = a->end;
while (e < end) {
Varr *a2 = e++->a;
Val *e2 = a2->begin;
char *key = js_ws((*e2++).s);
char *value = ___built_to_str_trace(dtp, *e2).s;
arr_push(bf, str_f("%s:%s", key, value));
}
return (Val)str_f("{%s}", arr_join(bf, ","));
}
if (*t == 't') {
Arr *bf = arr_new(); // [s.]
Varr *atp = __arr_get((Val)"", type, (Val)1).a;
Val *etp = atp->begin;
Val *endtp = atp->end;
Varr *a = value.a;
Val *e = a->begin;
while (etp < endtp) {
arr_push(bf, ___built_to_str_trace(*etp++, *e++).s);
}
return (Val)str_f("[. %s]", arr_join(bf, ","));
}
return (Val)str_f("%s(%ld)", t, (long)value.o);
}
void ___built_throw (Val pos, Val msg) {
Exc *exc = exc_get();
void *array[30];
size_t size;
char **strings;
size = backtrace(array, 30);
strings = backtrace_symbols(array, size);
char *path = path_cat(__sys_compilerRoot().s, "compilationDb", NULL);
Arr *stack = arr_new();
RANGE0(i, size) {
char *e = strings[i];
int ix1 = str_last_cindex(e, ')');
if (ix1 == -1) continue;
e = str_left(e, ix1);
ix1 = str_cindex(e, '(');
if (ix1 == -1) continue;
e = str_right(e, ix1 + 1);
ix1 = str_cindex(e, '_');
if (ix1 == -1) continue;
ix1 = str_cindex_from(e, '_', ix1 + 1);
if (ix1 == -1) continue;
int ix2 = str_cindex_from(e, '_', ix1 + 1);
if (ix2 == -1) {
if (str_starts(str_right(e, ix1), "_main+")) ix2 = ix1 + 5;
else continue;
}
char *mdId = str_left(e, ix1);
char *fn = str_sub(e, ix1 + 1, ix2);
char *mdPath = path_cat(path, str_f("%s.mod", mdId), NULL);
if (!file_exists(mdPath)) continue;
Arr *tb = js_ra(file_read(mdPath));
char *fpath = js_rs(arr_get(tb, 0));
if (!strcmp(fn, "main")) {
arr_push(stack, str_f("  %s:[main]", fpath));
continue;
}
Arr *fn_tb = js_ra(arr_get(tb, 3));
int n = -1;
EACH(fn_tb, char, e) {
Arr *fields = js_ra(e);
if (!strcmp(js_rs(arr_get(fields, 1)), fn)) {
n = js_ri(arr_get(fields, 0));
break;
}
}_EACH
if (n != -1) arr_push(stack, str_f("  %s:%d:", fpath, n));
}_RANGE
free(strings);
exc->stack = stack;
exc->msg = str_f("%s %s", pos.s, msg.s);
exc->type = "";
longjmp(*((jmp_buf *)arr_pop(exc->buf)), 1);
}
void ___built_trace (Val isComplete, Val fpos, Val type, Val value) {
char *r = ___built_to_str_trace(type, value).s;
if (!(isComplete.b)) r = arr_join(arr_take(str_runes(r), 50), "");
puts(str_f("%s %s", fpos.s, r));
}


// -------------------------------------
//arr
// -------------------------------------

Val __arr_all (Val a, Val fn) {
Val *p = a.a->begin;
Val *end = a.a->end;
int r = TRUE;
while (p < end) {
Val v = *p++;
r = r && (((Val (*)(Val))fn.o)(v)).b;
}
return (Val)r;
}
Val __arr_any (Val a, Val fn) {
Val *p = a.a->begin;
Val *end = a.a->end;
int r = FALSE;
while (p < end) {
Val v = *p++;
r = r || (((Val (*)(Val))fn.o)(v)).b;
}
return (Val)r;
}
void __arr_clear (Val a) {
a.a->end = a.a->begin;
}
void __arr_cat (Val a, Val a2) {
int64_t other_size = __arr_size(a2).i;
if (other_size) {
int64_t this_size = __arr_size(a).i;
int64_t this_buffer = a.a->end_bf - a.a->begin;
if (this_size + other_size >= this_buffer){
int64_t new_buffer = this_buffer + other_size + 15;
a.a->begin = GC_REALLOC(a.a->begin, new_buffer * sizeof(Val));
a.a->end = a.a->begin + this_size;
a.a->end_bf = a.a->begin + new_buffer;
}
Val *s = a2.a->begin;
Val *s_end = a2.a->end;
Val *t = a.a->end;
while (s < s_end) *t++ = *s++;
a.a->end = t;
}
}
Val __arr_cat2 (Val a, Val a2) {
Val r = __arr_copy(a);
__arr_cat(r, a2);
return r;
}
Val __arr_copy (Val a) {
Varr *this = a.a;
int64_t size = this->end - this->begin;
int64_t buffer = this->end_bf - this->begin;
Varr *r = MALLOC(Varr);
Val *begin = GC_MALLOC(buffer * sizeof(Val));
memcpy(begin, this->begin, size * sizeof(Val));
r->begin = begin;
r->end = begin + size;
r->end_bf = begin + buffer;
return (Val)r;
}
Val __arr_drop (Val a, Val n) {
if (n.i < 0) return __arr_copy(a);
if (n.i >= __arr_size(a).i) return __arr_new();
Val r = __arr_new();
Val *p = a.a->begin + n.i;
Val *end = a.a->end;
while (p < end) __arr_push(r, *p++);
return r;
}
Val __arr_dropWhile (Val a, Val fn) {
Val r = __arr_new();
Val *p = a.a->begin;
Val *end = a.a->end;
while (p < end) {
if (!(((Val (*)(Val))fn.o)(*p)).b) break;
++p;
}
while (p < end) __arr_push(r, *p++);
return r;
}
Val __arr_duplicates (Val a, Val fn) {
Val r = __arr_new();
Val dup = __arr_new();
Val *ap = a.a->begin;
Val *aend = a.a->end;
while (ap < aend) {
Val e = *ap++;
Val *rp = r.a->begin;
Val *rend = r.a->end;
int missing = TRUE;
while (rp < rend) {
Val e2 = *rp++;
if ((((Val (*)(Val,Val))fn.o)(e, e2)).b) {
missing = FALSE;
Val *dp = dup.a->begin;
Val *dend = dup.a->end;
int missing2 = TRUE;
while (dp < dend) {
Val e2 = *dp++;
if ((((Val (*)(Val,Val))fn.o)(e, e2)).b) {
missing2 = FALSE;
break;
}
}
if (missing2) __arr_push(dup, e);
break;
}
}
if (missing) __arr_push(r, e);
}
return __arr_new_from_carr((Val)2, (Val[]){r, dup});
}
void __arr_each (Val a, Val fn) {
Val *p = a.a->begin;
Val *end = a.a->end;
while (p < end) ((void (*)(Val))fn.o)(*p++);
}
void __arr_eachIx (Val a, Val fn) {
Val *p = a.a->begin;
Val *end = a.a->end;
int64_t i = 0;
while (p < end) ((void (*)(Val,Val))fn.o)(*p++, ((Val)i++));
}
Val __arr_filter (Val a, Val fn) {
Val r = __arr_new();
Val *p = a.a->begin;
Val *end = a.a->end;
while (p < end) {
Val e = *p++;
if ((((Val (*)(Val))fn.o)(e)).b) __arr_push(r, e);
}
return (Val)r;
}
void __arr_filterIn (Val a, Val fn) {
Val *p = a.a->begin;
Val *end = a.a->end;
Val *new_end = p;
while (p < end) {
Val e = *p++;
if ((((Val (*)(Val))fn.o)(e)).b) *new_end++ = e;
}
a.a->end = new_end;
}
Val __arr_find (Val a, Val fn) {
Val *p = a.a->begin;
Val *end = a.a->end;
while (p < end) {
Val e = *p++;
if ((((Val (*)(Val))fn.o)(e)).b)
return __arr_new_from_carr((Val)1, (Val[]){e});
}
return __arr_new();
}
Val __arr_fromIter (Val it) {
Val a = __arr_new();
while (__iter_hasNext(it).b) __arr_push(a, __iter_next(it));
return a;
}
Val __arr_from_sarr (Arr *a) {
Val r = __arr_new();
EACH(a, char, el) {
__arr_push(r, (Val)el);
}_EACH
return r;
}
Val __arr_get (Val pos, Val a, Val ix) {
int64_t sz = __arr_size(a).i;
int64_t i = ix.i;
if (i < 0 || i >= sz)
___built_throw(pos, (Val)str_f("Index %d out of [0,%d]", i, sz - 1));
return a.a->begin[i];
}
Val __arr_index (Val a, Val fn) {
Val *p = a.a->begin;
Val *end = a.a->end;
int64_t i = 0;
while (p < end) {
if ((((Val (*)(Val))fn.o)(*p++)).b) return (Val)i;
++i;
}
return (Val)(int64_t)-1;
}
void __arr_insert (Val pos, Val a, Val ix, Val e) {
int64_t size = __arr_size(a).i;
if (ix.i < 0 || ix.i > size)
___built_throw(pos, (Val)str_f("index %d out of [0,%d]", ix.i, size));
__arr_push(a, e);
Val *p = a.a->end;
Val *pix = a.a->begin + ix.i;
while (p > pix) {
*p = *(p - 1);
--p;
}
*p = e;
}
void __arr_insertArr (Val pos, Val a, Val ix, Val other) {
__arr_insertRange(pos, a, ix, other, (Val)0, __arr_size(other));
}
void __arr_insertRange (Val pos, Val this, Val ix, Val other, Val begin, Val end) {
int64_t this_len = __arr_size(this).i;
if (ix.i < 0 || ix.i > this_len)
___built_throw(pos, (Val)str_f("index %d out of [0,%d]", ix.i, this_len));
int64_t other_size = __arr_size(other).i;
if (end.i < begin.i)
___built_throw(pos, (Val)str_f("end(%d) < begin(%d)", end.i, begin.i));
if (begin.i < 0)
___built_throw(pos, (Val)str_f("begin(%d) < 0", begin.i));
if (end.i > other_size)
___built_throw(pos, (Val)str_f("end(%d) > size(%d)", end.i, other_size));
int64_t other_len = end.i - begin.i;
if (!other_len) return;
int64_t new_len = this_len + other_len;
int64_t new_size = new_len + 15;
Val *new_begin = GC_MALLOC(new_size * sizeof(Val));
memcpy(new_begin, this.a->begin, sizeof(Val) * ix.i);
memcpy(new_begin + ix.i, other.a->begin + begin.i, sizeof(Val) * other_len);
memcpy(
new_begin + (ix.i + other_len),
this.a->begin + ix.i,
sizeof(Val) * (__arr_size(this).i - ix.i)
);
this.a->begin = new_begin;
this.a->end = new_begin + new_len;
this.a->end_bf = new_begin + new_size;
}
Val __arr_join(Val a, Val sep) {
Buf *bf = buf_new();
int first = 1;
Val *p = a.a->begin;
Val *end = a.a->end;
while (p < end) {
if (first) {
first = 0;
} else {
buf_add(bf, sep.s);
}
buf_add(bf, (*p++).s);
}
return (Val)str_new(buf_str(bf));
}
Val __arr_map(Val a, Val fn) {
Val r = __arr_new();
Val *p = a.a->begin;
Val *end = a.a->end;
while (p < end) __arr_push(r, ((Val (*)(Val))fn.o)(*p++));
return r;
}
Val __arr_new (void) {
Varr *this = MALLOC(Varr);
this->begin = GC_MALLOC(15 * sizeof(Val));
this->end = this->begin;
this->end_bf = this->begin + 15;
return (Val)this;
}
Val __arr_new_fill (Val el, Val n) {
int64_t size = n.i < 0 ? 0 : n.i;
int64_t bf_size = size + 15;
Varr *this = MALLOC(Varr);
this->begin = GC_MALLOC(bf_size * sizeof(Val));
this->end = this->begin + size;
this->end_bf = this->begin + bf_size;
Val *p = this->begin;
Val *end = this->end;
while (p < end) *p++ = el;
return (Val)this;
}
Val __arr_new_from_carr (Val n, Val *els) {
Val r = __arr_new();
REPEAT(n.i) {
__arr_push(r, *els++);
}_REPEAT
return r;
}
Val __arr_peek (Val pos, Val a) {
int64_t size = __arr_size(a).i;
if (size == 0) ___built_throw(pos, (Val)"Array is empty");
return *(a.a->end - 1);
}
Val __arr_pop (Val pos, Val a) {
int64_t size = __arr_size(a).i;
if (size == 0) ___built_throw(pos, (Val)"Array is empty");
return *(a.a->end-- - 1);
}
void __arr_push (Val a, Val el) {
Varr *this = a.a;
if (this->end == this->end_bf) {
int size = this->end_bf - this->begin;
int new_size = size + size;
this->begin = GC_REALLOC(this->begin, new_size * sizeof(Val));
this->end = this->begin + size;
this->end_bf = this->begin + new_size;
}
*this->end++ = el;
}
Val __arr_reduce (Val a, Val seed, Val fn) {
Val *p = a.a->begin;
Val *end = a.a->end;
while (p < end) seed = ((Val (*)(Val,Val))fn.o)(seed, *p++);
return seed;
}
Val __arr_remove (Val pos, Val a, Val ix) {
int64_t sz = __arr_size(a).i;
int64_t i = ix.i;
if (i < 0 || i >= sz)
___built_throw(pos, (Val)str_f("index %d out of [0,%d]", i, sz - 1));
Val *p = a.a->begin + ix.i;
Val r = *p;
Val *p1 = p + 1;
Val *end = a.a->end--;
while (p1 < end) *p++ = *p1++;
return r;
}
Val __arr_removeRange (Val pos, Val a, Val begin, Val end) {
int64_t sz = __arr_size(a).i;
int64_t b = begin.i;
int64_t e = end.i;
if (b < 0 || b >= sz)
___built_throw(pos, (Val)str_f("begin index %d out of [0,%d]", b, sz - 1));
if (e < b || e > sz)
___built_throw(pos, (Val)str_f("end index %d out of [%d,%d]", e, b, sz));
int64_t len = e - b;
Val r = __arr_take(__arr_drop(a, begin), (Val)len);
Val *p = a.a->begin + b;
Val *p1 = a.a->begin + e;
Val *pend = a.a->end;
a.a->end -= len;
while (p1 < pend) *p++ = *p1++;
return r;
}
Val __arr_reverse (Val a) {
Val r = __arr_copy(a);
__arr_reverseIn(r);
return r;
}
void __arr_reverseIn (Val a) {
int64_t sz = __arr_size(a).i;
Val *p = a.a->begin;
Val *end = a.a->end - 1;
Val tmp;
for (int i = 0; i < sz / 2; ++i) {
tmp = *p;
*p++ = *end;
*end-- = tmp;
}
}
void __arr_set (Val pos, Val a, Val ix, Val el) {
int64_t sz = __arr_size(a).i;
int64_t i = ix.i;
if (i < 0 || i >= sz)
___built_throw(pos, (Val)str_f("index %d out of [0,%d]", i, sz - 1));
a.a->begin[i] = el;
}
void __arr_setArr (Val pos, Val a, Val ix, Val other) {
__arr_setRange(pos, a, ix, other, (Val)0, __arr_size(other));
}
void __arr_setRange (Val pos, Val this, Val ix, Val other, Val begin, Val end) {
int64_t this_len = __arr_size(this).i;
int64_t other_size = __arr_size(other).i;
if (ix.i < 0 || ix.i > this_len - other_size)
___built_throw(
pos, (Val)str_f("index %d out of [0,%d]", ix.i, this_len - other_size)
);
if (end.i < begin.i)
___built_throw(pos, (Val)str_f("end(%d) < begin(%d)", end.i, begin.i));
if (begin.i < 0)
___built_throw(pos, (Val)str_f("begin(%d) < 0", begin.i));
if (end.i > other_size)
___built_throw(pos, (Val)str_f("end(%d) > size(%d)", end.i, other_size));
int64_t other_len = end.i - begin.i;
if (!other_len) return;
Val *target = this.a->begin + ix.i;
Val *source = other.a->begin + begin.i;
memcpy(target, source, sizeof(Val *) * other_len);
}
Val __arr_shift (Val pos, Val a) {
int64_t size = __arr_size(a).i;
if (size == 0) ___built_throw(pos, (Val)"Array is empty");
Val *p = a.a->begin;
Val r = *p;
Val *p1 = p + 1;
Val *end = a.a->end--;
while (p1 < end) *p++ = *p1++;
return r;
}
void __arr_shuffle (Val a) {
int64_t size = __arr_size(a).i;
Val *begin = a.a->begin;
Val *p = a.a->end - 1;
Val *pix, tmp;
while (p > begin) {
pix = begin + sys_rnd_i(size--);
tmp = *p;
*p-- = *pix;
*pix = tmp;
}
}
Val __arr_size (Val a) {
return (Val)(a.a->end - a.a->begin);
}
void __arr_sort (Val a, Val fn) {
Val (*less0)(Val, Val) = (Val (*)(Val, Val))fn.o;
int less (Val e1, Val e2) {
return less0(e1, e2).b;
}
void sort(Val *a, int64_t size) {
if (size < 2) {
return;
}
if (size == 2) {
if (less(a[1], a[0])) { Val tmp = a[0]; a[0] = a[1]; a[1] = tmp; }
return;
}
if (size == 3) {
if (less(a[1], a[0])) { Val tmp = a[0]; a[0] = a[1]; a[1] = tmp; }
if (less(a[2], a[0])) { Val tmp = a[0]; a[0] = a[2]; a[2] = tmp; }
if (less(a[2], a[1])) { Val tmp = a[1]; a[1] = a[2]; a[2] = tmp; }
}
int64_t mid1 = size >> 1;
int64_t mid2 = size - mid1;
Val a1[mid1];
Val a2[mid2];
Val *pa = a;
Val *pa1 = a1;
Val *pa2 = a2;
for (int64_t i = 0; i < mid1; ++i) *pa1++ = *pa++;
for (int64_t i = 0; i < mid2; ++i) *pa2++ = *pa++;
sort(a1, mid1);
sort(a2, mid2);
pa = a;
pa1 = a1;
Val *pa1_end = a1 + mid1;
pa2 = a2;
Val *pa2_end = a2 + mid2;
for(;;) {
if (pa1 == pa1_end) {
while (pa2 < pa2_end) *pa++ = *pa2++;
break;
}
if (pa2 == pa2_end) {
while (pa1 < pa1_end) *pa++ = *pa1++;
break;
}
if (less(*pa2, *pa1)) *pa++ = *pa2++;
else *pa++ = *pa1++;
}
}
sort(a.a->begin, __arr_size(a).i);
}
Val __arr_sub (Val a, Val begin, Val end) {
if (begin.i < 0) begin = (Val)(__arr_size(a).i + begin.i);
if (end.i < 0) end = (Val)(__arr_size(a).i + end.i);
return __arr_drop(__arr_take(a, end), begin);
}
Val __arr_sub0 (Val a, Val begin) {
if (begin.i < 0) begin = (Val)(__arr_size(a).i + begin.i);
return __arr_drop(a, begin);
}
Val __arr_take (Val a, Val n) {
if (n.i > __arr_size(a).i) return __arr_copy(a);
Val r = __arr_new();
Val *p = a.a->begin;
for (int64_t i = 0; i < n.i; ++i) __arr_push(r, *p++);
return r;
}
Val __arr_takeWhile (Val a, Val fn) {
Val r = __arr_new();
Val *p = a.a->begin;
Val *end = a.a->end;
while (p < end) {
if (!(((Val (*)(Val))fn.o)(*p)).b) break;
__arr_push(r, *p++);
}
return r;
}
Val __arr_toIter_next (Val obj) {
Val *p = obj.a->begin;
Val a = *p;
int64_t sz = (p+1)->i;
int64_t c = (p+2)->i;
if (c < sz) {
Val r = *(a.a->begin+c);
*(p+2) = (Val)(c + 1);
return __arr_new_from_carr((Val)1, (Val[]){r});
}
return __arr_new();
}
Val __arr_toIter (Val a) {
return __iter_new(__arr_new_from_carr((Val)2, (Val[]){
__arr_new_from_carr((Val)3, (Val[]){a, __arr_size(a), (Val)0}),
(Val)(void *)__arr_toIter_next
}));
}
Val __arr_to_js (Val a, Val fn) {
return __js_wa(__arr_map(a, fn));
}
Arr *__arr_to_sarr (Val a) {
Arr *r = arr_new();
Varr *va = a.a;
Val *p = va->begin;
Val *end = va->end;
while (p < end) arr_push(r, (*p++).s);
return r;
}
void __arr_unshift (Val a, Val e) {
__arr_push(a, e);
Val *p = a.a->end;
Val *pix = a.a->begin;
while (p > pix) {
*p = *(p - 1);
--p;
}
*p = e;
}
Val __arr_unzip (Val a) {
Val a1 = __arr_new();
Val a2 = __arr_new();
Val *p = a.a->begin;
Val *end = a.a->end;
while (p < end) {
Val t = *p++;
__arr_push(a1, *(t.a->begin));
__arr_push(a2, *(t.a->begin + 1));
}
return __arr_new_from_carr((Val)2, (Val[]){a1, a2});
}
Val __arr_unzip3 (Val a) {
Val a1 = __arr_new();
Val a2 = __arr_new();
Val a3 = __arr_new();
Val *p = a.a->begin;
Val *end = a.a->end;
while (p < end) {
Val t = *p++;
__arr_push(a1, *(t.a->begin));
__arr_push(a2, *(t.a->begin + 1));
__arr_push(a3, *(t.a->begin + 2));
}
return __arr_new_from_carr((Val)3, (Val[]){a1, a2, a3});
}
Val __arr_zip (Val a1, Val a2) {
Val r = __arr_new();
int64_t sz = __arr_size(a1).i < __arr_size(a2).i
? __arr_size(a1).i
: __arr_size(a2).i
;
int64_t c = 0;
Val *p1 = a1.a->begin;
Val *p2 = a2.a->begin;
while (c < sz) {
__arr_push(r, __arr_new_from_carr((Val)2, (Val[]){*p1++, *p2++}));
++c;
}
return r;
}
Val __arr_zip3 (Val a1, Val a2, Val a3) {
Val r = __arr_new();
int64_t sz = __arr_size(a1).i < __arr_size(a2).i
? __arr_size(a1).i
: __arr_size(a2).i
;
sz = sz < __arr_size(a3).i
? sz
: __arr_size(a2).i < __arr_size(a3).i
? __arr_size(a2).i
: __arr_size(a3).i
;
int64_t c = 0;
Val *p1 = a1.a->begin;
Val *p2 = a2.a->begin;
Val *p3 = a3.a->begin;
while (c < sz) {
__arr_push(r, __arr_new_from_carr((Val)3, (Val[]){*p1++, *p2++, *p3++}));
++c;
}
return r;
}


// -------------------------------------
//b64
// -------------------------------------

Val __b64_decode (char *pos, Val b64) {
char *r;
TRY {
r = b64_decode(b64.s);
} CATCH(e) {
EXC_KUTT(str_f("%s %s", pos, e));
}_TRY
return (Val) r;
}
Val __b64_encode (Val s) {
return (Val)b64_encode(s.s);
}


// -------------------------------------
//bytes
// -------------------------------------

Val __bytes_add (Val bs1, Val bs2) {
bytes_add((Bytes *)bs1.o, (Bytes *)bs2.o);
return bs1;
}
Val __bytes_drop (Val bts, Val begin) {
Bytes *bs = (Bytes *)bts.o;
int64_t ix = begin.i;
int64_t sz = bytes_len(bs);
Bytes *bs2 = sz > ix
? ix <= 0
? bytes_from_bytes(bytes_bs(bs), sz)
: bytes_from_bytes(bytes_bs(bs) + ix, sz - ix)
: bytes_new()
;
return (Val)(void *)bs2;
}
Val __bytes_fromArr (Val a) {
int64_t sz = __arr_size(a).i;
Bytes *bs = bytes_new_bf(sz);
unsigned char *pu = bytes_bs(bs);
Val *p = a.a->begin;
Val *end = a.a->end;
while (p < end) *pu++ = (unsigned char)(*p++).i;
return (Val)(void *)bs;
}
Val __bytes_fromStr (Val str) {
return (Val)(void *)bytes_from_str(str.s);
}
Val __bytes_get (Val pos, Val bs, Val ix) {
int64_t sz = __bytes_size(bs).i;
int64_t i = ix.i;
if (i < 0 || i >= sz)
___built_throw(pos, (Val)str_f("Index %d out of [0,%d]", i, sz - 1));
return (Val)(int64_t)(bytes_bs((Bytes *)bs.o)[ix.i]);
}
Val __bytes_new (Val size) {
int64_t bf = size.i;
if (bf < 0) bf = 0;
Bytes *bs = bytes_new_bf(bf);
unsigned char *p = bytes_bs(bs);
while (bf) {
*p++ = 0;
--bf;
}
return (Val)(void *)bs;
}
void __bytes_set (Val pos, Val bs, Val ix, Val byte) {
int64_t sz = __bytes_size(bs).i;
int64_t i = ix.i;
if (i < 0 || i >= sz)
___built_throw(pos, (Val)str_f("Index %d out of [0,%d]", i, sz - 1));
bytes_bs((Bytes *)bs.o)[ix.i] = (unsigned char)byte.i;
}
Val __bytes_size (Val bs) {
return (Val)(int64_t)bytes_len((Bytes *)bs.o);
}
Val __bytes_take (Val bts, Val end) {
Bytes *bs = (Bytes *)bts.o;
int64_t ix = end.i;
int64_t sz = bytes_len(bs);
Bytes *bs2 = ix <= 0
? bytes_new()
: ix >= sz
? bytes_from_bytes(bytes_bs(bs), sz)
: bytes_from_bytes(bytes_bs(bs), ix)
;
return (Val)(void *)bs2;
}
Val __bytes_toArr (Val bts) {
Bytes *bs = (Bytes *)bts.o;
int sz = bytes_len(bs);
Val a = __arr_new();
unsigned char *p = bytes_bs(bs);
while (sz) {
__arr_push(a, (Val)(int64_t)*p++);
--sz;
}
return a;
}
Val __bytes_toStr (Val bs) {
return (Val)bytes_to_str((Bytes *)bs.o);
}


// -------------------------------------
//dic
// -------------------------------------

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
Val __dic_new_from_carr (Val n, Val *els) {
Val r = __arr_new();
REPEAT(n.i) {
Varr *kv = (*els++).a;
__dic_put(r, kv->begin[0], kv->begin[1]);
}_REPEAT
return r;
}
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


// -------------------------------------
//iter
// -------------------------------------

Val __iter_all (Val it, Val fn) {
int r = TRUE;
while (__iter_hasNext(it).b) {
Val v = __iter_next(it);
r = r && (((Val (*)(Val))fn.o)(v)).b;
}
return (Val)r;
}
Val __iter_any (Val it, Val fn) {
int r = FALSE;
while (__iter_hasNext(it).b) {
Val v = __iter_next(it);
r = r || (((Val (*)(Val))fn.o)(v)).b;
}
return (Val)r;
}
Val __iter_cat_next (Val obj) {
Val *p = obj.a->begin;
if (p->b) {
Val it = *(p+1);
if (__iter_hasNext(it).b) return __rs_some(__iter_next(it));
*p = (Val) FALSE;
}
Val it = *(p+2);
return (__iter_hasNext(it).b) ? __rs_some(__iter_next(it)) : __arr_new();
}
Val __iter_cat (Val it1, Val it2) {
Val obj = __arr_new_from_carr((Val)3, (Val[]){(Val)TRUE, it1, it2});
return __iter_new_aux(obj, __iter_cat_next);
}
Val __iter_count (Val it) {
int64_t c = 0;
while (__iter_hasNext(it).b) {
__iter_next(it);
++c;
}
return (Val)c;
}
Val __iter_drop (Val it, Val n) {
int64_t nv = n.i;
int64_t c = 0;
for(;;) {
if (c >= nv || !__iter_hasNext(it).b) break;
++c;
__iter_next(it);
}
return it;
}
Val __iter_empty_next (Val fake) {
return __arr_new();
}
Val __iter_empty (Val fake) {
return __iter_new_aux(fake, __iter_empty_next);
}
Val __iter_hasNext (Val it) {
return (Val)(__arr_size(*(it.a->begin)).i > 0);
}
Val __iter_new (Val tuple) {
Val *p = tuple.a->begin;
Val a = ((Val (*)(Val))(*(p+1)).o)(*p);
return __arr_new_from_carr((Val)2, (Val[]){a, tuple});
}
Val __iter_new_aux (Val obj, Val (*fn) (Val)) {
return __iter_new(__arr_new_from_carr((Val)2, (Val[]){
obj, (Val)(void *)fn
}));
}
Val __iter_next (Val it) {
Val r = *(it.a->begin);
if (__arr_size(r).i == 0)
___built_throw((Val)"Built-in", (Val)"Iterator is empty");
Val tuple = *(it.a->begin + 1);
Val *p = tuple.a->begin;
*(it.a->begin) = ((Val (*)(Val))(*(p+1)).o)(*p);
return *r.a->begin;
}
Val __iter_range_next (Val obj) {
Val *p = obj.a->begin;
int64_t begin = p->i;
int64_t end = (p+1)->i;
int withStep = (p+3)->i;
if (withStep) {
int64_t step = (p+2)->i;
if (step > 0) {
if (begin <= end) {
*p = (Val)(begin + step);
return __arr_new_from_carr((Val)1, (Val[]){(Val)begin});
}
} else {
if (begin >= end) {
*p = (Val)(begin + step);
return __arr_new_from_carr((Val)1, (Val[]){(Val)begin});
}
}
} else {
if (begin < end) {
*p = (Val)(begin + 1);
return __arr_new_from_carr((Val)1, (Val[]){(Val)begin});
}
}
return __arr_new();
}
Val __iter_range (Val pos, Val vs) {
Val obj = __arr_new();
Val *p = vs.a->begin;
int64_t sz = __arr_size(vs).i;
if (sz == 1) {
__arr_push(obj, (Val) 0);
__arr_push(obj, *p);
__arr_push(obj, (Val) 1);
__arr_push(obj, (Val) 0);
} else if (sz == 2) {
__arr_push(obj, *p++);
__arr_push(obj, *p);
__arr_push(obj, (Val) 1);
__arr_push(obj, (Val) 0);
} else {
__arr_push(obj, *p++);
__arr_push(obj, *p++);
if (p->i == 0)
___built_throw(pos, (Val)"Range step is 0");
__arr_push(obj, *p);
__arr_push(obj, (Val) 1);
}
return __iter_new_aux(obj, __iter_range_next);
}
Val __iter_take_next (Val obj) {
Val *p = obj.a->begin;
Val it = *p;
int64_t n = (p+1)->i;
int64_t c = (p+2)->i;
if (c < n) {
if (__iter_hasNext(it).b) {
*(p+2) = (Val)(c + 1);
return __rs_some(__iter_next(it));
}
*(p+2) = *(p+1);
}
return __arr_new();
}
Val __iter_take (Val it, Val n){
return __iter_new_aux(
__arr_new_from_carr((Val)3, (Val[]){it, n, (Val)0}),
__iter_take_next
);
}
Val __iter_unary_next (Val obj) {
if (__arr_size(obj).i) {
Val r = __arr_copy(obj);
__arr_clear(obj);
return r;
}
return __arr_new();
}
Val __iter_unary (Val value) {
return __iter_new_aux(__rs_some(value), __iter_unary_next);
}


// -------------------------------------
//js
// -------------------------------------

Val __js_is_null (Val json) {
return (Val)js_is_null(json.s);
}
Val __js_rb (Val json) {
return (Val)js_rb(json.s);
}
Val __js_ri (Val json) {
return (Val)js_rl(json.s);
}
Val __js_rf (Val json) {
return (Val)js_rd(json.s);
}
Val __js_rs (Val json) {
return (Val)js_rs(json.s);
}
Val __js_ra (Val json) {
return __arr_from_sarr(js_ra(json.s));
}
Val __js_ro (Val json) {
return __dic_from_smap(js_ro(json.s));
}
Val __js_wn(void) {
return (Val)js_wn();
}
Val __js_wb(Val value) {
return (Val)js_wb(value.b);
}
Val __js_wi(Val n) {
return (Val)math_itos(n.i);
}
Val __js_wf(Val n) {
return (Val)math_ftos(n.f, 9);
}
Val __js_ws(Val s) {
return (Val)js_ws(s.s);
}
Val __js_wa(Val a) {
return (Val)js_wa(__arr_to_sarr(a));
}
Val __js_wo(Val m) {
return (Val)js_wo(__dic_to_smap(m));
}


// -------------------------------------
//math
// -------------------------------------

Val __math_abs (Val n) {
return (Val)math_abs(n.f);
}
Val __math_acos (Val n) {
return (Val)acos(n.f);
}
Val __math_acosh (Val n) {
return (Val)acosh(n.f);
}
Val __math_asin (Val n) {
return (Val)asin(n.f);
}
Val __math_asinh (Val n) {
return (Val)asinh(n.f);
}
Val __math_atan (Val n) {
return (Val)atan(n.f);
}
Val __math_atanh (Val n) {
return (Val)atanh(n.f);
}
Val __math_ceil (Val n) {
return (Val)ceil(n.f);
}
Val __math_cos (Val n) {
return (Val)cos(n.f);
}
Val __math_cosh (Val n) {
return (Val)cosh(n.f);
}
Val __math_e (void) {
return (Val)M_E;
}
Val __math_eq (Val n1, Val n2, Val gap) {
return (Val)math_eq_gap(n1.f, n2.f, gap.f);
}
Val __math_exp (Val n) {
return (Val)exp(n.f);
}
Val __math_exp2 (Val n) {
return (Val)exp2(n.f);
}
Val __math_floor (Val n) {
return (Val)floor(n.f);
}
Val __math_fromEn (Val s) {
return __math_stof((Val)str_replace(s.s, ",", ""));
}
Val __math_fromHex (Val s) {
char *n = str_trim(s.s);
if (!*n) return __arr_new();
char *tmp;
int64_t r = strtol(n, &tmp, 16);
if (*tmp) return __arr_new();
return __rs_some((Val)r);
}
Val __math_fromIso (Val s) {
return __math_stof((Val)str_replace(str_replace(s.s, ".", ""), ",", "."));
}
Val __math_ftoi (Val n) {
return (Val)(int64_t)(n.f);
}
Val __math_ftos (Val n) {
return (Val)math_ftos(n.f, 9);
}
Val __math_ftos2 (Val n, Val dec) {
return (Val)math_ftos(n.f, dec.i);
}
Val __math_isDigits (Val n) {
return (Val)math_digits(n.s);
}
Val __math_itof (Val n) {
return (Val)(double)(n.i);
}
Val __math_itoh (Val n) {
return (Val)str_f("%x", n.i);
}
Val __math_itos (Val n) {
return (Val)math_itos(n.i);
}
Val __math_log (Val n) {
return (Val)log(n.f);
}
Val __math_log10 (Val n) {
return (Val)log10(n.f);
}
Val __math_log2 (Val n) {
return (Val)log2(n.f);
}
Val __math_max (Val n1, Val n2) {
return n1.f >= n2.f ? n1 : n2;
}
Val __math_maxFloat (void) {
return (Val)DBL_MAX;
}
Val __math_maxInt (void) {
return (Val)(int64_t)
(sizeof(int64_t) == sizeof(long long int) ? LLONG_MAX : LONG_MAX);
}
Val __math_min (Val n1, Val n2) {
return n1.f <= n2.f ? n1 : n2;
}
Val __math_minInt (void) {
return (Val)(int64_t)
(sizeof(int64_t) == sizeof(long long int) ? LLONG_MIN : LONG_MIN);
}
Val __math_pi (void) {
return (Val)M_PI;
}
Val __math_pow (Val base, Val exp) {
return (Val)pow(base.f, exp.f);
}
Val __math_pow10 (Val exp) {
int64_t n = exp.i;
double r = 1;
double mul = n > 0 ? 10 : 0.1;
n = n >= 0 ? n : -n;
REPEAT(n) {
r *= mul;
}_REPEAT
return (Val)r;
}
Val __math_rnd (void) {
return (Val)((double)rand() / (double)RAND_MAX);
}
Val __math_rndi (Val top) {
return (Val)(int64_t)(((double)rand() / (double)RAND_MAX) * top.i);
}
Val __math_round (Val n, Val dec) {
return (Val)math_round(n.f, dec.i);
}
Val __math_sin (Val n) {
return (Val)sin(n.f);
}
Val __math_sinh (Val n) {
return (Val)sinh(n.f);
}
Val __math_sqrt (Val n) {
return (Val)sqrt(n.f);
}
Val __math_stof (Val s) {
char *n = str_trim(s.s);
if (!*n) return __arr_new();
struct lconv *lc = localeconv();
int ix = str_cindex(n, '.');
if (ix != -1) n[ix] = *lc->decimal_point;
char *tmp;
double r = strtod(n, &tmp);
if (*tmp) return __arr_new();
return __rs_some((Val)r);
}
Val __math_stoi (Val s) {
char *n = str_trim(s.s);
if (!*n) return __arr_new();
char *tmp;
int64_t r = strtol(n, &tmp, 10);
if (*tmp) return __arr_new();
return __rs_some((Val)r);
}
Val __math_tan (Val n) {
return (Val)tan(n.f);
}
Val __math_tanh (Val n) {
return (Val)tanh(n.f);
}
Val __math_trunc (Val n) {
return (Val)trunc(n.f);
}


// -------------------------------------
//rs
// -------------------------------------

Val __rs_some (Val v) {
return __arr_new_from_carr((Val) 1, (Val[]){v});
}
Val __rs_ok (Val v) {
return __arr_new_from_carr((Val) 2, (Val[]){v, (Val)""});
}
Val __rs_error (Val v) {
return __arr_new_from_carr((Val) 2, (Val[]){v, v});
}
Val __rs_get (Val rs) {
return rs.a->begin[0];
}
Val __rs_eget (Val rs) {
return rs.a->begin[1];
}


// -------------------------------------
//str
// -------------------------------------

Val __str_cmp (Val s1, Val s2) {
return (Val)(int64_t)strcoll(s1.s, s2.s);
}
Val __str_ends (Val str, Val substr) {
return (Val)str_ends(str.s, substr.s);
}
Val __str_fmt (Val pos, Val format, Val args) {
char *fmt = format.s;
Varr *as = args.a;
Arr *pts = str_split(fmt, "%v");
if (arr_size(pts) != __arr_size(args).i + 1)
___built_throw(pos, (Val)str_f(
"Wrong arguments number (expected %d, fund %s)",
arr_size(pts) - 1, __arr_size(args).i
));
Val rs = __arr_new();
void **ps = arr_begin(pts);
__arr_push(rs, (Val)str_replace((char *) *ps++, "%%", "%"));
Val *pas = as->begin;
Val *pend = as->end;
while (pas < pend) {
__arr_push(rs, *pas++);
__arr_push(rs, (Val)str_replace((char *) *ps++, "%%", "%"));
}
return __arr_join(rs, (Val)"");
}
Val __str_fromIso (Val s) {
return (Val)str_from_iso(s.s);
}
Val __str_fromRunes (Val a) {
return __arr_join(a, (Val)"");
}
Val __str_fromUtf16 (Val codepoints) {
int64_t sz = __arr_size(codepoints).i;
unsigned *u = ATOMIC(sizeof(unsigned) * (sz + 1));
Val *p = codepoints.a->begin;
Val *end = codepoints.a->end;
unsigned *pu = u;
while (p < end) *pu++ = (unsigned)(*p++).i;
*pu = 0;
return (Val)str_from_unicode(u);
};
Val __str_get(Val pos, Val s, Val ix) {
if (ix.i < 0 || ix.i >= strlen(s.s))
___built_throw(pos, (Val)str_f("Index %d out of [0,%d]", ix.i, strlen(s.s) - 1));
return (Val)str_new_c(s.s[ix.i]);
}
Val __str_index(Val str, Val substr) {
return (Val)(int64_t)str_index(str.s, substr.s);
}
Val __str_indexFrom (Val str, Val substr, Val start) {
return (Val)(int64_t)str_index_from(str.s, substr.s, start.i);
}
Val __str_lastIndex (Val str, Val substr) {
return (Val)(int64_t)str_last_index(str.s, substr.s);
}
Val __str_left (Val str, Val end) {
Val r = __str_toRunes(str);
int64_t sz = __arr_size(r).i;
end = end.i < 0 ? (Val)(sz + end.i) : end;
return __arr_join(__arr_take(r, end), (Val)"");
}
Val __str_left_slice(Val str, Val begin) {
return (Val)str_left(str.s, begin.i);
}
Val __str_len (Val str) {
return (Val)(int64_t)strlen(str.s);
}
Val __str_ltrim (Val str) {
return (Val)str_ltrim(str.s);
}
Val __str_replace (Val s, Val old, Val new) {
return (Val)str_replace(s.s, old.s, new.s);
}
Val __str_right (Val str, Val begin) {
Val r = __str_toRunes(str);
int64_t sz = __arr_size(r).i;
begin = begin.i < 0 ? (Val)(sz + begin.i) : begin;
return __arr_join(__arr_drop(r, begin), (Val)"");
}
Val __str_right_slice(Val str, Val begin) {
return (Val)str_right(str.s, begin.i);
}
Val __str_rtrim (Val str) {
return (Val)str_rtrim(str.s);
}
Val __str_split (Val str, Val sep) {
char *s = str.s;
char *ssep = sep.s;
Val r = __arr_new();
int len = strlen(ssep);
if (!len) {
char *rune;
s = str_next_rune(&rune, s);
while (*rune) {
__arr_push(r, (Val)rune);
s = str_next_rune(&rune, s);
}
return r;
}
int i = str_index(s, ssep);
while (i != -1) {
char *sub = ATOMIC(i + 1);
sub[i] = 0;
memcpy(sub, s, i);
__arr_push(r, (Val)sub);
s = s + i + len;
i = str_index(s, ssep);
}
__arr_push(r, (Val)str_new(s));
return r;
}
Val __str_splitTrim (Val str, Val sep) {
Val r = __str_split(str, sep);
Val *p = r.a->begin;
Val *end = r.a->end;
while (p < end) {
*p = __str_trim(*p);
++p;
}
return r;
}
Val __str_starts(Val str, Val substr) {
return (Val)str_starts(str.s, substr.s);
}
Val __str_sub (Val str, Val begin, Val end) {
Val r = __str_toRunes(str);
int64_t sz = __arr_size(r).i;
begin = begin.i < 0 ? (Val)(sz + begin.i) : begin;
end = end.i < 0 ? (Val)(sz + end.i) : end;
return __arr_join(__arr_drop(__arr_take(r, end), begin), (Val)"");
}
Val __str_sub_slice(Val str, Val begin, Val end) {
return (Val)str_sub(str.s, begin.i, end.i);
}
Val __str_toLower(Val s) {
return (Val)str_to_lower(s.s);
}
Val __str_toUpper(Val s) {
return (Val)str_to_upper(s.s);
}
Val __str_toRunes (Val str) {
char *s = str.s;
Val r = __arr_new();
char *rune;
s = str_next_rune(&rune, s);
while (*rune) {
__arr_push(r, (Val)rune);
s = str_next_rune(&rune, s);
}
return r;
}
Val __str_toUtf16 (Val str) {
unsigned *u = str_to_unicode(str.s);
Val a = __arr_new();
unsigned *pu = u;
while (*pu) __arr_push(a, (Val)(int64_t)*pu++);
return a;
}
Val __str_trim (Val str) {
return (Val)str_trim(str.s);
}


// -------------------------------------
//sys
// -------------------------------------

static Val __sys_compiler_root_v = (Val) "";
static void __sys_shandler (int sig) {
___built_throw((Val)"buil-in", (Val)"Arithmetic exception");
}
static void __sys_sshandler (int sig) {
___built_throw((Val)"buil-in", (Val)"Segmentation violation");
}
void __sys_init(Val compiler_root) {
sys_init();
signal (SIGFPE, __sys_shandler);
signal (SIGSEGV, __sys_sshandler);
__sys_compiler_root_v = compiler_root;
}
Val __sys_compilerRoot (void) {
return __sys_compiler_root_v;
}
Val __sys_getLocale (void) {
return (Val)sys_get_locale();
}
void __sys_setLocale (Val charset) {
return sys_set_locale(charset.s);
}

