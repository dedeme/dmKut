
// iter ------------------------------

// [!A], (A->b) -> b
Val __iter_all (Val it, Val fn);

// [!A], (A->b) -> b
Val __iter_any (Val it, Val fn);

// [!A], [!A] -> [!A]
Val __iter_cat (Val it1, Val it2);

// [!A] -> i
Val __iter_count (Val it);

// [!A],i->[!A]
Val __iter_drop (Val it, Val n);

// -> [!A]
Val __iter_empty (Val fake);

// [! A] -> b
Val __iter_hasNext (Val it);

// [. O, (O->[E])]->[!E]
Val __iter_new (Val tuple);

// O, (O->[E]) -> [!E]
Val __iter_new_aux (Val obj, Val (*fn) (Val));

// s,[! A] -> A
Val __iter_next (Val it);

// s,[i] -> [! i]
Val __iter_range (Val pos, Val vs);

// [!A],i->[!A]
Val __iter_take (Val it, Val n);

// A -> [!A]
Val __iter_unary (Val value);

