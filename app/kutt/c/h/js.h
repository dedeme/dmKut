// js ----------------------------------

// s->b
Val __js_is_null (Val json);
// s->b
Val __js_rb (Val json);
// s->i
Val __js_ri (Val json);
// s->f
Val __js_rf (Val json);
// s->s
Val __js_rs (Val json);
// s->[.s]
Val __js_ra (Val json);
// s->{s}
Val __js_ro (Val json);
// ->s
Val __js_wn(void);
// b->s
Val __js_wb(Val value);
// i->s
Val __js_wi(Val n);
// f->s
Val __js_wf(Val n);
// s->s
Val __js_ws(Val s);
// [.s]->s
Val __js_wa(Val a);
// {s}->s
Val __js_wo(Val m);

