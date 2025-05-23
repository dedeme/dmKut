
// INTERFACE ----------------------------------------

#define EXC_KUTT EXC_GENERIC
typedef union built_Val Val;
typedef struct __arr_Varr Varr;
union built_Val {
int b;
int64_t i;
double f;
char *s;
Varr *a;
void *o;
};
struct __arr_Varr {
Val *begin;
Val *end;
Val *end_bf;
};

void ___built_jump (int reference);
char *___built_mpath(char *module_id);
char *___built_mpos(char *module_id, int ln);
Val ___built_divi(Val pos, Val n1, Val n2);
Val ___built_divf(Val pos, Val n1, Val n2);
Val ___built_eq (Val type, Val value1, Val value2);
Val ___built_to_str_trace (Val type, Val value);
Val ___built_to_str (Val type, Val value);
void ___built_throw (Val pos, Val msg);
void ___built_trace (Val isComplete, Val fpos, Val type, Val value);

Val __arr_all (Val a, Val fn);
Val __arr_any (Val a, Val fn);
void __arr_cat (Val a, Val a2);
Val __arr_cat2 (Val a, Val a2);
void __arr_clear (Val a);
Val __arr_copy (Val a);
Val __arr_drop (Val a, Val n);
Val __arr_dropWhile (Val a, Val fn);
Val __arr_duplicates (Val a, Val fn);
void __arr_each (Val a, Val fn);
void __arr_eachIx (Val a, Val fn);
Val __arr_filter (Val a, Val fn);
void __arr_filterIn (Val a, Val fn);
Val __arr_find (Val a, Val fn);
Val __arr_fromIter (Val it);
Val __arr_from_sarr (Arr *a);
Val __arr_get (Val pos, Val a, Val ix);
Val __arr_index (Val a, Val fn);
void __arr_insert (Val pos, Val a, Val ix, Val e);
void __arr_insertArr (Val pos, Val a, Val ix, Val other);
void __arr_insertRange (Val pos, Val a, Val ix, Val other, Val begin, Val end);
Val __arr_join(Val a, Val sep);
Val __arr_map(Val a, Val fn);
Val __arr_new (void);
Val __arr_new_fill (Val el, Val n);
Val __arr_new_from_carr (Val n, Val *els);
Val __arr_peek (Val pos, Val a);
Val __arr_pop (Val pos, Val a);
void __arr_push (Val a, Val el);
Val __arr_reduce (Val a, Val seed, Val fn);
Val __arr_remove (Val pos, Val a, Val ix);
Val __arr_removeRange (Val pos, Val a, Val begin, Val end);
Val __arr_reverse (Val a);
void __arr_reverseIn (Val a);
void __arr_set (Val pos, Val a, Val ix, Val el);
void __arr_setArr (Val pos, Val a, Val ix, Val other);
void __arr_setRange (Val pos, Val a, Val ix, Val other, Val begin, Val end);
Val __arr_shift (Val pos, Val a);
void __arr_shuffle (Val a);
Val __arr_size (Val a);
void __arr_sort (Val a, Val fn);
Val __arr_sub (Val a, Val begin, Val end);
Val __arr_sub0 (Val a, Val begin);
Val __arr_take (Val a, Val n);
Val __arr_takeWhile (Val a, Val fn);
Val __arr_toIter (Val a);
Val __arr_to_js (Val a, Val fn);
Arr *__arr_to_sarr (Val a);
void __arr_unshift (Val a, Val e);
Val __arr_unzip (Val a);
Val __arr_unzip3 (Val a);
Val __arr_zip (Val a1, Val a2);
Val __arr_zip3 (Val a1, Val a2, Val a3);

Val __b64_decode (char *pos, Val b64);
Val __b64_encode (Val s);

Val __bytes_add (Val bs1, Val bs2);
Val __bytes_drop (Val bs, Val begin);
Val __bytes_fromArr (Val a);
Val __bytes_fromStr (Val str);
Val __bytes_get (Val pos, Val bs, Val ix);
Val __bytes_new (Val size);
void __bytes_set (Val pos, Val bs, Val ix, Val byte);
Val __bytes_size (Val bs);
Val __bytes_take (Val bs, Val end);
Val __bytes_toArr (Val bs);
Val __bytes_toStr (Val bs);

Val __dic_from_smap (Map *m);
Val __dic_get(Val d, Val key);
Val __dic_new_from_carr (Val n, Val *els);
void __dic_put(Val d, Val key, Val value);
Val __dic_to_js (Val d, Val (*fn)(Val));
Map *__dic_to_smap (Val a);

Val __iter_all (Val it, Val fn);
Val __iter_any (Val it, Val fn);
Val __iter_cat (Val it1, Val it2);
Val __iter_count (Val it);
Val __iter_drop (Val it, Val n);
Val __iter_empty (Val fake);
Val __iter_hasNext (Val it);
Val __iter_new (Val tuple);
Val __iter_new_aux (Val obj, Val (*fn) (Val));
Val __iter_next (Val it);
Val __iter_range (Val pos, Val vs);
Val __iter_take (Val it, Val n);
Val __iter_unary (Val value);

Val __js_is_null (Val json);
Val __js_rb (Val json);
Val __js_ri (Val json);
Val __js_rf (Val json);
Val __js_rs (Val json);
Val __js_ra (Val json);
Val __js_ro (Val json);
Val __js_wn(void);
Val __js_wb(Val value);
Val __js_wi(Val n);
Val __js_wf(Val n);
Val __js_ws(Val s);
Val __js_wa(Val a);
Val __js_wo(Val m);

Val __math_abs (Val n);
Val __math_acos (Val n);
Val __math_acosh (Val n);
Val __math_asin (Val n);
Val __math_asinh (Val n);
Val __math_atan (Val n);
Val __math_atanh (Val n);
Val __math_ceil (Val n);
Val __math_cos (Val n);
Val __math_cosh (Val n);
Val __math_e (void);
Val __math_eq (Val n1, Val n2, Val gap);
Val __math_exp (Val n);
Val __math_exp2 (Val n);
Val __math_floor (Val n);
Val __math_fromEn (Val s);
Val __math_fromHex (Val s);
Val __math_fromIso (Val s);
Val __math_ftoi (Val n);
Val __math_ftos (Val n);
Val __math_ftos2 (Val n, Val dec);
Val __math_isDigits (Val n);
Val __math_itof (Val n);
Val __math_itoh (Val n);
Val __math_itos (Val n);
Val __math_log (Val n);
Val __math_log10 (Val n);
Val __math_log2 (Val n);
Val __math_max (Val n1, Val n2);
Val __math_maxFloat (void);
Val __math_maxInt (void);
Val __math_min (Val n1, Val n2);
Val __math_minInt (void);
Val __math_pi (void);
Val __math_pow (Val base, Val exp);
Val __math_pow10 (Val exp);
Val __math_rnd (void);
Val __math_rndi (Val top);
Val __math_round (Val n, Val dec);
Val __math_sin (Val n);
Val __math_sinh (Val n);
Val __math_sqrt (Val n);
Val __math_stof (Val s);
Val __math_stoi (Val s);
Val __math_tan (Val n);
Val __math_tanh (Val n);
Val __math_trunc (Val n);

Val __rs_some (Val v);
Val __rs_ok (Val v);
Val __rs_error (Val v);
Val __rs_get (Val rs);
Val __rs_eget (Val rs);

Val __str_cmp (Val s1, Val s2);
Val __str_ends (Val str, Val substr);
Val __str_fmt (Val pos, Val format, Val args);
Val __str_fromIso (Val s);
Val __str_fromRunes (Val a);
Val __str_fromUtf16 (Val codepoints);
Val __str_get(Val pos, Val s, Val ix);
Val __str_index (Val str, Val substr);
Val __str_indexFrom (Val str, Val substr, Val start);
Val __str_lastIndex (Val str, Val substr);
Val __str_left (Val str, Val end);
Val __str_left_slice(Val str, Val begin);
Val __str_len (Val str);
Val __str_ltrim (Val str);
Val __str_replace (Val s, Val old, Val new);
Val __str_right (Val str, Val end);
Val __str_right_slice(Val str, Val begin);
Val __str_rtrim (Val str);
Val __str_split (Val str, Val sep);
Val __str_splitTrim (Val str, Val sep);
Val __str_starts (Val str, Val substr);
Val __str_sub(Val str, Val begin, Val end);
Val __str_sub_slice(Val str, Val begin, Val end);
Val __str_toLower (Val str);
Val __str_toRunes (Val str);
Val __str_toUpper (Val str);
Val __str_toUtf16 (Val str);
Val __str_trim (Val str);

void __sys_init(Val compiler_root);
Val __sys_compilerRoot (void);
Val __sys_getLocale (void);
void __sys_setLocale (Val charset);

