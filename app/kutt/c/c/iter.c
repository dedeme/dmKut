
// iter -----------------------------

// [!A], (A->b) -> b
Val __iter_all (Val it, Val fn) {
  int r = TRUE;
  while (__iter_hasNext(it).b) {
    Val v = __iter_next(it);
    r = r && (((Val (*)(Val))fn.o)(v)).b;
  }
  return (Val)r;
}

// [!A], (A->b) -> b
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
// [!A], [!A] -> [!A]
Val __iter_cat (Val it1, Val it2) {
  Val obj = __arr_new_from_carr((Val)3, (Val[]){(Val)TRUE, it1, it2});
  return __iter_new_aux(obj, __iter_cat_next);
}

// [!A] -> i
Val __iter_count (Val it) {
  int64_t c = 0;
  while (__iter_hasNext(it).b) {
    __iter_next(it);
    ++c;
  }
  return (Val)c;
}

// [!A],i->[!A]
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
// -> [!A]
Val __iter_empty (Val fake) {
  return __iter_new_aux(fake, __iter_empty_next);
}

// [! A] -> b
Val __iter_hasNext (Val it) {
  return (Val)(__arr_size(*(it.a->begin)).i > 0);
}

// [. O, (O->[E])]->[! E]
Val __iter_new (Val tuple) {
  Val *p = tuple.a->begin;
  Val a = ((Val (*)(Val))(*(p+1)).o)(*p);
  return __arr_new_from_carr((Val)2, (Val[]){a, tuple});
}
// O, (O->[E]) -> [!E]
Val __iter_new_aux (Val obj, Val (*fn) (Val)) {
  return __iter_new(__arr_new_from_carr((Val)2, (Val[]){
    obj, (Val)(void *)fn
  }));
}

// s,[! A] -> A
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

// s,[i] -> [! i]
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
// [!A],i->[!A]
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
// A -> [!A]
Val __iter_unary (Val value) {
  return __iter_new_aux(__rs_some(value), __iter_unary_next);
}
