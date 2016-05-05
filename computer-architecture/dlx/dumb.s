.data
; Input matrix 1
M1:
  .float 1.0, 0.0
  .float 0.0, 1.0

; Input matrix 2
M2:
  .float 1.25, -4.7545
  .float 1.6, -7.213

; Output matrix
MR:
  .float 0, 0, 0, 0
  .float 0, 0, 0, 0
  .float 0, 0, 0, 0
  .float 0, 0, 0, 0

ONE:
  .float 1.0

.text
.global main

main:
  add r1, r0, M1
  add r2, r0, M2

  jal inverse

  trap 0

; Computes the inverse of a matrix of address r1,
; and leaves the result in r2
inverse:
  lf f0, 0(r1)
  lf f1, 4(r1)
  lf f2, 8(r1)
  lf f3, 12(r1)

  ; First diagonal in f4, second in f5
  multf f4, f0, f3
  multf f5, f1, f2
  subf f4, f4, f5

  ; f4 -> determinant
  ; f5 -> 1 / determinant
  lf f6, ONE
  divf f5, f6, f4

  multf f0, f0, f5
  multf f1, f1, f5
  multf f2, f2, f5
  multf f3, f3, f5

  sf 0(r2), f0
  sf 4(r2), f1
  sf 8(r2), f2
  sf 12(r2), f3

  jr r31
