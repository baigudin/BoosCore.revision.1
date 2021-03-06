; ----------------------------------------------------------------------------
; Assembler macro commads
;
; @author    Sergey Baigudin, baigudin@mail.ru
; @copyright 2014-2015 Sergey Baigudin
; @license   http://baigudin.com/license/
; @link      http://baigudin.com
; ----------------------------------------------------------------------------
        .asg             b15, sp
; ----------------------------------------------------------------------------
; Function call
; ----------------------------------------------------------------------------
call     .macro          lable
         b               lable
mr?      addkpc          mr?+4, b3, 4
         .endm

; ----------------------------------------------------------------------------
; Function call by CPU register
; ----------------------------------------------------------------------------
bacc     .macro          ac
         b               ac
mr?      addkpc          mr?+4, b3, 4
         .endm

; ----------------------------------------------------------------------------
; Return from interrupt
; ----------------------------------------------------------------------------
rete     .macro
         b               irp
         nop             5
         .endm

; ----------------------------------------------------------------------------
; Return from function
; ----------------------------------------------------------------------------
ret      .macro      
         b		         b3
         nop             5
         .endm

; ----------------------------------------------------------------------------
; Push register to stack
; ----------------------------------------------------------------------------
pushr   .macro           ac   
        stw              ac, *sp--[0x2]
        .endm

; ----------------------------------------------------------------------------
; Pop register from stack
; ----------------------------------------------------------------------------
popr    .macro           ac
        ldw              *++sp[0x2], ac
        nop 4
        .endm


