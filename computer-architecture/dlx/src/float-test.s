; Test to see if a float is in IEEE 754 format
.data 0x1000

FLOAT:
  .float 5.1
  .space 4
DEC:
  .word 0

FLOAT1: .float 3.0
FLOAT2: .float 4.0
FLOAT3: .float 5.0

.text
.global main
main:
  ; This was used to test that floats are in IEEE 754 representation
  lf f1, FLOAT(r0)
  movfp2i r1, f1
  sw DEC(r0), r1

  ; These are a few pipeline tests
  lf f1, FLOAT1(r0)
  lf f2, FLOAT2(r0)
  lf f3, FLOAT3(r0)

  multf f1, f1, f1
  multf f1, f1, f1
  multf f1, f1, f1

  multf f2, f2, f2
  multf f2, f2, f2
  multf f2, f2, f2

  multf f3, f3, f3
  multf f3, f3, f3
  multf f3, f3, f3

  multf f1, f1, f1
  multf f2, f2, f2
  multf f3, f3, f3

  multf f1, f1, f1
  multf f2, f2, f2
  multf f3, f3, f3

  multf f1, f1, f1
  multf f2, f2, f2
  multf f3, f3, f3

  trap 0
