; Test to see if a float is in IEEE 754 format
.data 0x1000

FLOAT:
  .float 5.1
  .space 3
DEC:
  .word 0

.text
.global main
main:
  lf f1, FLOAT(r0)
  movfp2i r1, f1
  sw DEC(r0), r1

  trap 0
