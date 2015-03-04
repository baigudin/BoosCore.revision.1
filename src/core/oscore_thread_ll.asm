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
        .def            _initiateLow__Q2_6oscore6ThreadSFv
        .def            _terminateLow__Q2_6oscore6ThreadSFv

        .ref            _running__Q2_6oscore6ThreadSFv
        .ref            _thread___Q2_6oscore6Thread
        .ref            _registerShift___Q2_6oscore6Thread
        .ref            _system___Q2_6oscore6Thread
; ----------------------------------------------------------------------------
        .asg            _initiateLow__Q2_6oscore6ThreadSFv,  m_initiate
        .asg            _terminateLow__Q2_6oscore6ThreadSFv, m_terminate
        .asg            _running__Q2_6oscore6ThreadSFv,      m_running
        .asg            _thread___Q2_6oscore6Thread,         v_thread
        .asg            _registerShift___Q2_6oscore6Thread,  v_registerShift
        .asg            _system___Q2_6oscore6Thread,         v_system
        .asg            b15, sp
; ----------------------------------------------------------------------------

        .text
; ----------------------------------------------------------------------------
; Initiate thread execution
;
; return void
; ----------------------------------------------------------------------------
m_initiate:
        ; Save current contex for restore
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
; Terminate thread execution
;
; return void
; ----------------------------------------------------------------------------
m_terminate:
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
