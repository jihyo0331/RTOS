  .syntax unfified
  .cpu cortex-3
  .fpu softvfp
  .thumb

  .global  g_pfnVectors
  .global  Default_Handler

  .word  _sidata
  .word  _sdata
.word
