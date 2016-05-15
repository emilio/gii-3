#!/usr/bin/env ruby
#
require 'pp'

def determinant(a)
  a[0] * a[3] - a[1] * a[2]
end

a = [5.234, 2.52, 1.23, 7.52]
b = [1.25, -4.7545, 1.6, -7.213]

det_a = determinant(a)
det_b = determinant(b)

puts "dets: #{det_a}, #{det_b}, #{1 / det_a}, #{1 / det_b}"

x = (1 / det_b) * (1 / det_b) * (1 / det_a) * (1 / det_a)
y = (1 / det_b) * (1 / det_b) * (1 / det_b) * (1 / det_a)
z = (1 / det_a) * (1 / det_a) * (1 / det_a) * (1 / det_b)

puts "x: #{x}, y: #{y}, z: #{z}"

result = [
  a[3] * x, -a[1] * x, b[3] * y, -b[1] * y,
  -a[2] * x, a[0] * x, -b[2] * y, b[0] * y,
  b[3] * x, -b[1] * x, a[3] * z, -a[1] * z,
  -b[2] * x, b[0] * x, -a[2] * z, a[0] * z
]

puts "-------------------------------------"
pp(result)

def inverse(a)
  f = 1 / determinant(a)
  [a[3], -a[1], -a[2], a[0]].map { |x| x * f }
end

def ohmn(a, b)
  det_a = determinant(a)
  det_b = determinant(b)
  [
    a[0] * det_b, a[1] * det_b, b[0] * det_b, b[1] * det_b,
    a[2] * det_b, a[3] * det_b, b[2] * det_b, b[3] * det_b,
    b[0] * det_a, b[1] * det_a, a[0] * det_a, a[1] * det_a,
    b[2] * det_a, b[3] * det_a, a[2] * det_a, a[3] * det_a,
  ]
end

puts "-------------------------------------"
result = ohmn(inverse(a), inverse(b)).map { |x| x * 1 / det_a * 1 / det_b }

pp(result)
