
// * -> [*]
Val __rs_some (Val v) {
  return __arr_new_from_carr((Val) 1, (Val[]){v});
}

// * -> [*,s]
Val __rs_ok (Val v) {
  return __arr_new_from_carr((Val) 2, (Val[]){v, (Val)""});
}

// s -> [*,s]
Val __rs_error (Val v) {
  return __arr_new_from_carr((Val) 2, (Val[]){v, v});
}

// [*, s] -> *
Val __rs_get (Val rs) {
  return rs.a->begin[0];
}

// [*, s] -> s
Val __rs_eget (Val rs) {
  return rs.a->begin[1];
}

