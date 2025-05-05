// dic ---------------------------------

// Auxiliar of __js_ro
// Map<char>->{s}
Val __dic_from_smap (Map *m);
// {A},s -> [A]
Val __dic_get(Val d, Val key);
// i,*[s,A]->{A}
Val __dic_new_from_carr (Val n, Val *els);
// {A},s,A->()
void __dic_put(Val d, Val key, Val value);
// {A},(A->s)->s
Val __dic_to_js (Val d, Val (*fn)(Val));
// Auxiliar of __js_wo
// {A}->Map<char>
Map *__dic_to_smap (Val a);

