.data 0x1000
; Input matrix 1
M1:
  .float 5.234, 2.52
  .float 1.23,  7.52

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

.text
.global main

main:
  lw r1, M1(0)
  jal determinant

  halt

; Computes the determinant of a matrix and stores the result in f0
; Gets the data from the register 1's address
determinant:
  ; Get the four floats into the registers f0..3
  lf f0, 0(r1)
  lf f1, 1(r1)
  lf f2, 2(r1)
  lf f3, 3(r1)
  mult f0, f0, f3 ; First diagonal in f4
  mult f1, f1, f2 ; Second in f5
  sub f0, f1, f0

  jr r31
