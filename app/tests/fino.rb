def fib(n)
  return 1 if n <= 1
  fib(n - 1) + fib(n - 2)
end

tm = Time.now.to_i
puts fib(37)
puts (Time.now.to_i - tm) * 1000
