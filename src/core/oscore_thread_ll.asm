; ----------------------------------------------------------------------------
; Thread low level module
;
; @author    Sergey Baigudin <baigudin@mail.ru>
; @copyright 2014-2015 Sergey Baigudin
; @license   http://baigudin.com/license/
; @link      http://baigudin.com
; ----------------------------------------------------------------------------
        .include        "oscore_macro.asm"
; ----------------------------------------------------------------------------
        .def            os_thread_switch
        .def            _begin__Q2_6oscore6ThreadSFv
        .def            _lowDisable__Q2_6oscore6ThreadSFv
        .def            _yield__Q2_6oscore6ThreadSFv

        .ref            os_int_gie
        .ref            os_int_enable
        .ref            os_int_disable
        .ref            _running__Q2_6oscore6ThreadSFv
        .ref            _thread___Q2_6oscore6Thread
        .ref            _sheduler__Q2_6oscore6ThreadSFv
        .ref            _registerShift___Q2_6oscore6Thread
        .ref            _system___Q2_6oscore6Thread
        .ref            _shedulerStack___Q2_6oscore6Thread
; ----------------------------------------------------------------------------
        .asg            _begin__Q2_6oscore6ThreadSFv,       m_start
        .asg            _lowDisable__Q2_6oscore6ThreadSFv,  m_disable
        .asg            _yield__Q2_6oscore6ThreadSFv,       m_yield
        .asg            _running__Q2_6oscore6ThreadSFv,     m_running
        .asg            _thread___Q2_6oscore6Thread,        v_thread
        .asg            _sheduler__Q2_6oscore6ThreadSFv,    m_sheduler
        .asg            _registerShift___Q2_6oscore6Thread, v_registerShift
        .asg            _system___Q2_6oscore6Thread,         v_system
        .asg            _shedulerStack___Q2_6oscore6Thread,  v_shedulerStack
        .asg            b15, sp
; ----------------------------------------------------------------------------

        .sect           ".os_text"
; ----------------------------------------------------------------------------
; Switch OS thread
; ----------------------------------------------------------------------------
os_thread_switch:
        ; Save context
        stdw            a1:a0, *-sp[1]
        stdw            b1:b0, *-sp[2]
        mvkl            v_thread, a0
     || mvkl            v_registerShift, b0
        mvkh            v_thread, a0
     || mvkh            v_registerShift, b0
        ldw             *a0, a0
     || ldw             *b0, b0
     || mv              sp, a1
     || mv              sp, b1
        nop             4
  [!a0] b               os_null_thread?  ;Control _thread.current on NULL
     || add             a0, b0, a0
        ldw             *a0, a0
        stdw            a3:a2, *-a1[3]
     || stdw            b3:b2, *-b1[4]
        nop             3
        lddw            *-a1[1], a3:a2
     || lddw            *-b1[2], b3:b2
     || add             a0, 8, b0
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
        stdw            a7:a6, *a0++[2]
     || stdw            b7:b6, *b0++[2]
        stdw            a9:a8, *a0++[2]
     || stdw            b9:b8, *b0++[2]
        stdw            a11:a10, *a0++[2]
     || stdw            b11:b10, *b0++[2]
        stdw            a13:a12, *a0++[2]
     || stdw            b13:b12, *b0++[2]
        stdw            a15:a14, *a0++[2]
     || stdw            b15:b14, *b0++[2]
     || mvkl            v_thread, a10
        stdw            a17:a16, *a0++[2]
     || stdw            b17:b16, *b0++[2]
     || mvkh            v_thread, a10
        stdw            a19:a18, *a0++[2]
     || stdw            b19:b18, *b0++[2]
     || mvkl            v_registerShift, b10
        stdw            a21:a20, *a0++[2]
     || stdw            b21:b20, *b0++[2]
     || mvkh            v_registerShift, b10
        stdw            a23:a22, *a0++[2]
     || stdw            b23:b22, *b0++[2]
     || mvkl            v_shedulerStack, b15
        stdw            a25:a24, *a0++[2]
     || stdw            b25:b24, *b0++[2]
     || mvkh            v_shedulerStack, b15
        ldw             *b15, b15
        stdw            a27:a26, *a0++[2]
     || stdw            b27:b26, *b0++[2]
        stdw            a29:a28, *a0++[2]
     || stdw            b29:b28, *b0++[2]
     || mvkl            m_sheduler, a3
        stdw            a31:a30, *a0++[2]
     || stdw            b31:b30, *b0++[2]
     || mvkh            m_sheduler, a3
        stw             a1, *a0            ; 43 cycle
     || stw             b1, *-b0[1]
        ; Sheduler call
     || b               a3
ret00?: addkpc          ret00?+4, b3, 4
        ; Load context
        ldw             *a10, a10
     || ldw             *b10, b10
        nop             4
        add             a10, b10, a10
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

os_null_thread?:
        b               irp
     || lddw            *-sp[2], b1:b0
        lddw            *-sp[1], a1:a0
        nop             4

; ----------------------------------------------------------------------------
; Enable thread execution
;
; return void
; ----------------------------------------------------------------------------
m_start:
        ; Save system context:
        stdw            a1:a0, *-sp[1]
        stdw            b1:b0, *-sp[2]
        mvkl            v_system, a0
     || mv              sp, a1
        mvkh            v_system, a0
     || mv              sp, b1
        ldw             *a0, a0
        stdw            a3:a2, *-a1[3]
     || stdw            b3:b2, *-b1[4]
        lddw            *-a1[1], a3:a2
     || lddw            *-b1[2], b3:b2
        nop             2
        add             a0, 8, b0
        nop             1
        stdw            a3:a2, *a0++[2]
     || stdw            b3:b2, *b0++[2]
        lddw            *-a1[3], a3:a2
     || lddw            *-b1[4], b3:b2
        mvc             amr, b1
        mv              b1, a1
     || mvc             irp, b1
        nop             2
        .eval           2, i
        .eval           3, j
        .loop           
        .break(i == 32)
        stdw            a:j::a:i:, *a0++[2]
     || stdw            b:j::b:i:, *b0++[2]
        .eval           i+2, i
        .eval           j+2, j
        .endloop
        stw             a1, *a0
     || stw             b1, *-b0[1]
        ; Running:
        pushr           b3
        call            m_running
        mv              a4, a0
        popr            b3
        ; Return if no running process:
  [!A0] b               b3
        nop             5        
        ; Jump to first thread:
        mvkl            v_thread, a0
     || mvkl            v_registerShift, b0
        mvkh            v_thread, a0
     || mvkh            v_registerShift, b0
        ldw             *a0, a0
     || ldw             *b0, b0
        nop             4
        add             a0, b0, a0
        ldw             *a0, a31
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
; Disable thread execution
;
; return void
; ----------------------------------------------------------------------------
m_disable:

        mvkl            v_system, a31
        mvkh            v_system, a31
        ldw             *a31, a31
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
        b               b3
        lddw            *a31++[2], a31:a30
     || lddw            *b31++[2], b31:b30
        nop             4

; ----------------------------------------------------------------------------
; Yield next thread
;
; return void
; ----------------------------------------------------------------------------
m_yield:
        mv              b3, b13
        call            os_int_disable
        b               os_thread_switch
        mvkl            mr?, b0
        mvkh            mr?, b0
        mvc             b0, irp
        mv              b13, b3
        mv              a4, a1
mr?:    bnop            os_int_gie, 5

; ----------------------------------------------------------------------------
; Geting pointer to virtual function
;
; param  A4 Address of Runnable object
; param  B4 Pointer to Member Function Types
; return uint32
; ----------------------------------------------------------------------------
os_vptrFuncAddr:
        ldh             *+b4[0], a5  ; a5 = d
        ldh             *+b4[1], a6  ; a6 = i
        nop             3
        ldw             *a4[a5], a5  ; a5 = vptr
        nop             4
        lddw            *a5[a6], a5:a4
        nop             4
        b               b3
        mv              a5, a4
        nop             4