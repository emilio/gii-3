.data 0x2000
; Input matrix 1
M1:
M10:
  .float 5.234
M11:
  .float 2.52
M12:
  .float 1.23
M13:
  .float 7.52

; Input matrix 2
M2:
M20:
  .float 1.25
M21:
  .float -4.7545
M22:
  .float 1.6
M23:
  .float -7.213

; Output matrix
MR:
  .float 0, 0, 0, 0
  .float 0, 0, 0, 0
  .float 0, 0, 0, 0
  .float 0, 0, 0, 0

ONE:
  .float 1.0

; http://bits.stephan-brumme.com/inverse.html
INVERSE_CONSTANT:
  .word 0x7EEEEEEE

.text
.global main

main:
; lf f0, ZERO

; 1..4 = A_1..4
  lf f1, M10(r0)
  lf f4, M13(r0)
  lw r31, INVERSE_CONSTANT(r0)
; f7: |A|
  multf f7, f1, f4
  lf f2, M11(r0)
  lf f3, M12(r0)

  multf f8, f2, f3
; 11..14 = B_1..4
  lf f11, M20(r0)
  lf f14, M23(r0)
  lf f12, M21(r0)
; f17: |B|
  multf f17, f11, f14
  lf f13, M22(r0)
  subf  f7, f7, f8

  multf f18, f12, f13
; f7: 1/|A|
; NOTE: no way to access |A| again
  movfp2i r1, f7
  subf  f17, f17, f18
  subu r1, r31, r1
  movi2fp f7, r1

; f17: 1/|B|
  movfp2i r1, f17
  subu r1, r31, r1
  movi2fp f17, r1

; f8 = tmp = 1/|A| * 1 / |B|
; f10 = X = tmp * tmp
; f18 = Y = 1/(|B|^3) * 1/|A|
; f19 = Z = 1/(|A|^3) * 1/|B|
  multf f8, f7, f17
; Transact the first row
  lw r1, MR(r0)
; f6: -A_3
  subf f6, f0, f3
  multf f18, f8, f17
; f5: -A_2
  subf f5, f0, f2
  multf f19, f8, f7
; f15: -B_2
  subf f15, f0, f12
  multf f10, f8, f8
; f16: -B_3
  subf f16, f0, f13
  multf f18, f18, f17
  multf f19, f19, f7

; Row 1
; R0 =  a4 * X
  multf f20,  f4, f10
; R1 = -a2 * X
  multf f21,  f5, f10
; R2 =  b4 * Y
  multf f22, f14, f18
; R3 = -b2 * Y
  multf f23, f15, f18

; Row 2
; R4  = -a3 * X
  multf f24,  f6, f10
  sf  0(r1), f20
; R5  =  a1 * X
  multf f25,  f1, f10
  sf  4(r1), f21
; R6  = -b3 * Y
  multf f26, f16, f18
  sf  8(r1), f22
; R7  =  b1 * Y
  multf f27, f11, f18
  sf 12(r1), f23

; Row 3
; R8  =  b4 * X
  multf f28, f14, f10
  sf 16(r1), f24
; R9  = -b2 * X
  multf f29, f15, f10
  sf 20(r1), f25
; R10 =  a4 * Z
  multf f30,  f4, f19
  sf 24(r1), f26
; R11 = -a2 * Z
  multf f31,  f5, f19
  sf 28(r1), f27


; Row 4
; R12 = -b3 * X
  multf f20, f16, f10
  sf 32(r1), f28
  sf 36(r1), f29
; R13 =  b1 * X
  multf f21, f11, f10
  sf 40(r1), f30
  sf 44(r1), f31
; R14 = -a3 * Z
  multf f22,  f6, f19
  sf 48(r1), f20
  sf 52(r1), f21
; R15 =  a1 * Z
  multf f23,  f1, f19

; Second row

; Third row

; Fourth (and last!) row
  sf 56(r1), f22
  sf 60(r1), f23

  trap 0
