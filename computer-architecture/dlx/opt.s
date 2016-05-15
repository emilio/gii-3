.data
; Input matrix 1
M1:
M10:
  .float 1.0
M11:
  .float 0.0
M12:
  .float 0.0
M13:
  .float 1.0

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
  lw r31, INVERSE_CONSTANT(r0)

; 1..4 = A_1..4
  lf f1, M10(r0)
  lf f2, M11(r0)
  lf f3, M12(r0)
  lf f4, M13(r0)
; TODO: f5 and f6 can be moved around to be convenient
; f5: -A_2
  subf f5, f0, f2
; f6: -A_3
  subf f6, f0, f3

; f17: |A|
  multf f7, f1, f4
  multf f8, f2, f3
  subf  f7, f7, f8

; 11..14 = B_1..4
  lf f11, M20(r0)
  lf f12, M21(r0)
  lf f13, M22(r0)
  lf f14, M23(r0)

; TODO: f15 and f16 can be moved around to be convenient
; f15: -B_2
  subf f15, f0, f12
; f16: -B_3
  subf f16, f0, f13

; f17: |B|
  multf f17, f11, f14
  multf f18, f12, f13
  subf  f17, f17, f18

; f7: 1/|A|
  movfp2i r1, f6
  subu r1, r31, r1
  movi2fp f7, r1

; f17: 1/|B|
  movfp2i r1, f16
  subu r1, r31, r1
  movi2fp f17, r1

; f18 = X = 1/(|A|^2) * 1/|B|
; f19 = Y = 1/(|B|^2) * 1/|A|
  multf f18, f7, f7
  multf f18, f18, f17

  multf f19, f17, f17
  multf f19, f19, f7

; Row 1
; R0 =  a4 * X
  multf f20,  f4, f18
; R1 = -a2 * X
  multf f21,  f5, f18
; R2 =  b4 * X
  multf f22, f14, f18
; R3 = -b2 * X
  multf f23, f15, f18

; Row 2
; R4  = -a3 * X
  multf f24,  f6, f18
; R5  =  a1 * X
  multf f25,  f1, f18
; R6  = -b3 * X
  multf f26, f16, f18
; R7  =  b1 * X
  multf f27, f11, f18

; Row 3
; R8  =  b4 * Y
  multf f28, f14, f19
; R9  = -b2 * Y
  multf f29, f15, f19
; R10 =  a4 * Y
  multf f30,  f4, f19
; R11 = -a2 * Y
  multf f31,  f5, f19

; Transact the first row
  lw r1, MR(r0)
  sf  0(r1), f20
  sf  4(r1), f21
  sf  8(r1), f22
  sf 12(r1), f23

; Row 4
; R12 = -b3 * Y
  multf f20, f16, f19
; R13 =  b1 * Y
  multf f21, f11, f19
; R14 = -a3 * Y
  multf f22,  f6, f19
; R15 =  a1 * Y
  multf f23,  f1, f19

; Second row
  sf 16(r1), f24
  sf 20(r1), f25
  sf 24(r1), f26
  sf 28(r1), f27

; Third row
  sf 32(r1), f28
  sf 36(r1), f29
  sf 40(r1), f30
  sf 44(r1), f31

; Fourth (and last!) row
  sf 48(r1), f20
  sf 52(r1), f21
  sf 56(r1), f22
  sf 60(r1), f23

  trap 0
