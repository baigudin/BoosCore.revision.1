; ----------------------------------------------------------------------------
; Interrupt low level module
;
; @author    Sergey Baigudin <baigudin@mail.ru>
; @copyright 2014-2015 Sergey Baigudin
; @license   http://baigudin.com/license/
; @link      http://baigudin.com
; ----------------------------------------------------------------------------
        .include        "oscore_register.asm"
        .include        "oscore_macro.asm"
; ----------------------------------------------------------------------------
        .def            os_int_enable
        .def            os_int_disable
        .def            os_int_gie
        .def            _enable__Q2_6oscore9InterruptSFi
        .def            _disable__Q2_6oscore9InterruptSFv
        .def            _lowLock__Q2_6oscore9InterruptSFUi
        .def            _lowUnlock__Q2_6oscore9InterruptSFUi
        .def            _lowSet__Q2_6oscore9InterruptSFUi
        .def            _lowClear__Q2_6oscore9InterruptSFUi
; ----------------------------------------------------------------------------
        .asg            _enable__Q2_6oscore9InterruptSFi,     _enable
        .asg            _disable__Q2_6oscore9InterruptSFv,    _disable
        .asg            _lowLock__Q2_6oscore9InterruptSFUi,   _lowLock
        .asg            _lowUnlock__Q2_6oscore9InterruptSFUi, _lowUnlock
        .asg            _lowSet__Q2_6oscore9InterruptSFUi,    _lowSet
        .asg            _lowClear__Q2_6oscore9InterruptSFUi,  _lowClear
        .asg            b15, sp
; ----------------------------------------------------------------------------
        .sect           ".os_text"
; ----------------------------------------------------------------------------
; Set GIE bit
;
; param  A4 GIE bit value
; return void
; ----------------------------------------------------------------------------
os_int_gie:
        b               b3
        mv              a4, a1
        mvc             csr, b0
        and            ~C_REG_CSR_GIE, b0, b0
   [a1] or              C_REG_CSR_GIE, b0, b0
        mvc             b0, csr

; ----------------------------------------------------------------------------
; Interrupt enable C++ function
;
; param  A4 If set in one - all interrupts enable, else no effect
; return void
; ----------------------------------------------------------------------------
_enable:
os_int_enable:
        b               b3
        and             1, a4, a1
   [a1] mvc             csr, b0
   [a1] or              C_REG_CSR_GIE, b0, b0
   [a1] mvc             b0, csr
        nop             1

; ----------------------------------------------------------------------------
; Interrupt disable C++ function
;
; return A4 Value of GIE bit before function call
; ----------------------------------------------------------------------------
_disable:
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
_lowLock:
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
_lowUnlock:
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
_lowSet:
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
_lowClear:
        b               b3        
        mvk             1, a1     
        shl             a1, a4, a0
        mvc             a0, icr   
        mvk             0, a4         
        nop             1         
