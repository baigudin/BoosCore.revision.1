; ----------------------------------------------------------------------------
; Interrupt low level module
;
; @author    Sergey Baigudin, baigudin@mail.ru
; @copyright 2014-2015 Sergey Baigudin
; @license   http://baigudin.com/license/
; @link      http://baigudin.com
; ----------------------------------------------------------------------------
        .include        "oscore_register.asm"
        .include        "oscore_macro.asm"
; ----------------------------------------------------------------------------
        .def            os_int_enable
        .def            os_int_disable
        .def            _enable__Q2_6oscore9InterruptSFi
        .def            _disable__Q2_6oscore9InterruptSFv
        .def            _lockLow__Q2_6oscore9InterruptSFUi
        .def            _unlockLow__Q2_6oscore9InterruptSFUi
        .def            _setLow__Q2_6oscore9InterruptSFUi
        .def            _clearLow__Q2_6oscore9InterruptSFUi
        .def            _jumpLow__Q2_6oscore9InterruptSFUi

        .ref            os_system_init
        .ref            _pointer___Q2_6oscore9Interrupt
        .ref            _stack___Q2_6oscore9Interrupt
        .ref            _handle__Q2_6oscore9InterruptSFi
; ----------------------------------------------------------------------------
        .asg            _enable__Q2_6oscore9InterruptSFi,     m_enable
        .asg            _disable__Q2_6oscore9InterruptSFv,    m_disable
        .asg            _lockLow__Q2_6oscore9InterruptSFUi,   m_lock
        .asg            _unlockLow__Q2_6oscore9InterruptSFUi, m_unlock
        .asg            _setLow__Q2_6oscore9InterruptSFUi,    m_set
        .asg            _clearLow__Q2_6oscore9InterruptSFUi,  m_clear
        .asg            _jumpLow__Q2_6oscore9InterruptSFUi,   m_jump
        .asg            _handle__Q2_6oscore9InterruptSFi,     m_handle
        .asg            _pointer___Q2_6oscore9Interrupt,      v_pointer
        .asg            _stack___Q2_6oscore9Interrupt,        v_stack
        .asg            b15, sp
; ----------------------------------------------------------------------------        
handle  .macro          num
os_int:num:
        stdw            a1:a0, *-sp[1]
     || mvkl            v_pointer, a0
        mvkh            v_pointer, a0
     || b               os_interrupt_handler
        ldw             *a0, a0
        stdw            b1:b0, *-sp[2]
        mvk             num, b0
        nop             2
        .endm
; ----------------------------------------------------------------------------        
empty   .macro          num
os_int:num:
        bnop            os_int:num:, 5
        nop             1
        nop             1
        nop             1
        nop             1
        nop             1
        nop             1
        nop             1
        .endm
; ----------------------------------------------------------------------------        
        .sect           ".hwi"
; ----------------------------------------------------------------------------
; Hardware interrupt table
; ----------------------------------------------------------------------------
os_reset:
        b               os_reset+24
        mvc             csr, b0
        and            ~(C_REG_CSR_GIE|C_REG_CSR_PGIE), b0, b0
        mvc             b0, csr
        mvkl            os_system_init, b0
        mvkh            os_system_init, b0
        b               b0
        nop             5
; ----------------------------------------------------------------------------
        handle          1
        empty           2
        empty           3
        handle          4
        handle          5
        handle          6
        handle          7
        handle          8
        handle          9
        handle          10
        handle          11
        handle          12
        handle          13
        handle          14
        handle          15
; ----------------------------------------------------------------------------
        .text
; ----------------------------------------------------------------------------
; Interrupt handler
; ----------------------------------------------------------------------------
os_interrupt_handler:
        ;Save context:
        stw             b0, *-sp[0]
     || mv              a0, b0
     || mv              sp, a1
     || mv              sp, b1
        stdw            a3:a2, *-a1[3]
     || stdw            b3:b2, *-b1[4]
     || add             b0, 8, b0
        lddw            *-a1[1], a3:a2
     || lddw            *-b1[2], b3:b2
        nop             4
        stdw            a3:a2, *a0++[2]
     || stdw            b3:b2, *b0++[2]
        lddw            *-a1[3], a3:a2
     || lddw            *-b1[4], b3:b2
        mvc             amr, b1
        mv              b1, a1
     || mvc             irp, b1
        nop             2
        stdw            a3:a2, *a0++[2]
     || stdw            b3:b2, *b0++[2]
        stdw            a5:a4, *a0++[2]
     || stdw            b5:b4, *b0++[2]
        ldw             *sp, a4
        stdw            a7:a6, *a0++[2]
     || stdw            b7:b6, *b0++[2]
        stdw            a9:a8, *a0++[2]
     || stdw            b9:b8, *b0++[2]
        stdw            a11:a10, *a0++[2]
     || stdw            b11:b10, *b0++[2]
     || mvkl            v_pointer, a10
        stdw            a13:a12, *a0++[2]
     || stdw            b13:b12, *b0++[2]
     || mvkh            v_pointer, a10
        stdw            a15:a14, *a0++[2]
     || stdw            b15:b14, *b0++[2]
        stdw            a17:a16, *a0++[2]
     || stdw            b17:b16, *b0++[2]
        stdw            a19:a18, *a0++[2]
     || stdw            b19:b18, *b0++[2]
        stdw            a21:a20, *a0++[2]
     || stdw            b21:b20, *b0++[2]
        stdw            a23:a22, *a0++[2]
     || stdw            b23:b22, *b0++[2]
     || mvkl            v_stack, b15
        stdw            a25:a24, *a0++[2]
     || stdw            b25:b24, *b0++[2]
     || mvkh            v_stack, b15
        ldw             *b15, b15
        stdw            a27:a26, *a0++[2]
     || stdw            b27:b26, *b0++[2]
        stdw            a29:a28, *a0++[2]
     || stdw            b29:b28, *b0++[2]
        stdw            a31:a30, *a0++[2]
     || stdw            b31:b30, *b0++[2]
        stw             a1, *a0
     || stw             b1, *-b0[1]
        ;Sheduler call:
     || b               m_handle
ret00?  addkpc          ret00?+4, b3, 4
        ;Load context:
        ldw             *a10, a31
        nop             4
        add             a31, 8, b31
        .eval           0, i
        .eval           1, j
        .loop           
        .break(i == 30)
        lddw            *a31++[2], a:j::a:i:
     || lddw            *b31++[2], b:j::b:i:
        .eval           i+2, i
        .eval           j+2, j
        .endloop
        ldw             *+a31[4], a30 ; AMR
     || ldw             *+b31[3], b30 ; IRP
        nop             4
        mvc             a30, amr
        mvc             b30, irp
        b               irp
        lddw            *a31++[2], a31:a30
     || lddw            *b31++[2], b31:b30
        nop             4

; ----------------------------------------------------------------------------
; Global interrupt enable (C++ function)
;
; param  A4 value witch return disable method. If in one - all interrupts enables.
; return void
; ----------------------------------------------------------------------------
m_enable:
os_int_enable:
        b               b3
        and             1, a4, a1
   [a1] mvc             csr, b0
   [a1] or              C_REG_CSR_GIE, b0, b0
   [a1] mvc             b0, csr
        nop             1

; ----------------------------------------------------------------------------
; Global interrupt disable (C++ function)
;
; return A4 value of GIE bit before method is call
; ----------------------------------------------------------------------------
m_disable:
os_int_disable:
        b               b3
        mvc             csr, b0
        and             C_REG_CSR_GIE, b0, a4
        and            ~C_REG_CSR_GIE, b0, b0
        mvc             b0, csr
        nop             1

; ----------------------------------------------------------------------------
; Lock maskable interrupt C++ function
;
; return A4 Error code or zero
; ----------------------------------------------------------------------------
m_lock:
        b               m10?      
        mvk             1, a1      
        shl             a1, a4, a0 
     || mvc             csr, b0    
        mv              b0, b1  
        and            ~C_REG_CSR_GIE, b0, b0 
        mvc             b0, csr    
     || mvk             0, a4          
m10?:   b               b3         
     || not             a0, a1     
        mvc             ier, b0    
        and             b0, a1, a0 
        or              a0, 3, a1  
        mvc             a1, ier    
        mvc             b1, csr
   
; ----------------------------------------------------------------------------
; Unlock maskable interrupt C++ function
;
; return A4 Error code or zero
; ----------------------------------------------------------------------------
m_unlock:
        b               m20?    
        mvk             1, a1      
        shl             a1, a4, a0 
     || mvc             csr, b0    
        mv              b0, b1  
        and            ~C_REG_CSR_GIE, b0, b0 
        mvc             b0, csr    
     || or              a0, 3, a1  
m20?:   b               b3         
        mvc             ier, b0    
        or              b0, a1, a0 
        mvc             a0, ier    
        mvc             b1, csr    
        mvk             0, a4 
     
; ----------------------------------------------------------------------------
; Set interrupt status C++ function
;
; return A4 Error code or zero
; ----------------------------------------------------------------------------
m_set:
        b               b3        
        mvk             1, a1     
        shl             a1, a4, a0
        mvc             a0, isr   
        mvk             0, a4         
        nop             1         

; ----------------------------------------------------------------------------
; Clear interrupt status C++ function
;
; return A4 Error code or zero
; ----------------------------------------------------------------------------
m_clear:
        b               b3        
        mvk             1, a1     
        shl             a1, a4, a0
        mvc             a0, icr   
        mvk             0, a4         
        nop             1 
          
; ----------------------------------------------------------------------------
; Jump to interrupt HW vector
;
; return void
; ----------------------------------------------------------------------------
m_jump:
        ; Interrupt disable and calc vector address
        bnop            mj?, 1
        mvc             csr, b0
     || shl             a4, 5, a4
        and             C_REG_CSR_GIE, b0, a0
     || mvkl            os_reset, b4 
        and            ~C_REG_CSR_GIE, b0, b0
     || mvkh            os_reset, b4
        mvc             b0, csr
     || add             b4, a4, a4
        ; Jump to handle
mj?:    bnop            a4, 2
        mvkl            mr?, b0
        mvkh            mr?, b0
        mvc             b0, irp
        ; Return point
mr?:    bnop            b3, 1
        mvc             csr, b0
        and            ~C_REG_CSR_GIE, b0, b0
   [a0] or              C_REG_CSR_GIE, b0, b0
        mvc             b0, csr

