; ----------------------------------------------------------------------------
; Interrupt low level module
;
; @author Sergey Baigudin <baigudin@mail.ru>
; @copyright 2014-2015 Sergey Baigudin
; @license   http://baigudin.com/license/
; @link      http://baigudin.com
; ----------------------------------------------------------------------------
        .include        "os_core_register.asm"
        .include        "os_core_macro.asm"
; ----------------------------------------------------------------------------
        .def            os_int_enable
        .def            os_int_disable
        .def            os_int_gie
        .def            _enable__Q2_6OsCore9InterruptSFUi
        .def            _disable__Q2_6OsCore9InterruptSFv
        .def            __lock__Q2_6OsCore9InterruptSFUi
        .def            __unlock__Q2_6OsCore9InterruptSFUi
        .def            __set__Q2_6OsCore9InterruptSFUi
        .def            __clear__Q2_6OsCore9InterruptSFUi
; ----------------------------------------------------------------------------
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
_enable__Q2_6OsCore9InterruptSFUi:
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
_disable__Q2_6OsCore9InterruptSFv:
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
__lock__Q2_6OsCore9InterruptSFUi:
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
__unlock__Q2_6OsCore9InterruptSFUi:
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
__set__Q2_6OsCore9InterruptSFUi:
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
__clear__Q2_6OsCore9InterruptSFUi:
        b               b3        
        mvk             1, a1     
        shl             a1, a4, a0
        mvc             a0, icr   
        mvk             0, a4         
        nop             1         
