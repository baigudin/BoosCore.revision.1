; ----------------------------------------------------------------------------
; Core of Operation System
;
; @author Sergey Baigudin <baigudin@mail.ru>
; @copyright 2014-2015 Sergey Baigudin
; @license   http://baigudin.com/license/
; @link      http://baigudin.com
; ----------------------------------------------------------------------------
        .include        "os_core_register.asm"
        .include        "os_core_macro.asm"
; ----------------------------------------------------------------------------
        .def            _c_int00
        .def            os_system_init
        .def            __initClasses__Q2_6OsCore6SystemSFv
; ----------------------------------------------------------------------------
        .ref            os_int_enable
        .ref            os_int_disable
        .ref            _OS_STACK_END
        .ref            __start__Q2_6OsCore6SystemSFv
        .ref            ___cinit__
        .ref            ___pinit__
; ----------------------------------------------------------------------------
        .asg            b15, sp
; ----------------------------------------------------------------------------

        .sect           ".os_text"
; ----------------------------------------------------------------------------
; Low level system initialization
;
; return void
; ----------------------------------------------------------------------------
_c_int00:
os_system_init:
        ; Reset Interrupt Selector Registers:
        mvk             0, a0
        mvkl            DREG_MUXH, b0
        mvkh            DREG_MUXH, b0
        stw             a0, *b0
        nop             2
        mvkl            DREG_MUXL, b0
        mvkh            DREG_MUXL, b0
        stw             a0, *b0
        nop             2
        ; Interrupt disable:
        call            os_int_disable
        ; Set CPU register:
        mvk             0, a0
        mvc             a0, istp
        mvc             a0, amr
        mvc             a0, ier
        mvc             a0, irp
        mvc             a0, nrp
        mvk             -1, a0
        mvc             a0, icr
        nop
        ; Reset Cache Configuration Register:
        mvkl            DREG_CCFG, b0
     || mvk             0x0300, a0
        mvkh            DREG_CCFG, b0
        stw             a0, *b0
        nop             2
        ; Named section initialization:
        call            os_system_init_cinit
        ; Zero CPU register:
        .eval           0, i
        .loop           32
        mvk             0, a:i:
     || mvk             0, b:i:
        .eval           i+1, i
        .endloop
        ; Setup OS core stack pointer
        mvkl            _OS_STACK_END, b15
        mvkh            _OS_STACK_END, b15
        sub             b15, 8, b15
        ; Call hi level initialization
        mvkl            os_system_deinit, b3
     || mvkl            __start__Q2_6OsCore6SystemSFv, a3
        mvkh            os_system_deinit, b3
     || mvkh            __start__Q2_6OsCore6SystemSFv, a3
        b               a3
        nop             5

; ----------------------------------------------------------------------------
; Low level system deinitialization
;
; return void
; ----------------------------------------------------------------------------
os_system_deinit:
        nop
        nop
        nop
        bnop            os_system_deinit, 5
;       idle

; ----------------------------------------------------------------------------
; Initialize constants (.cinit)
;
; return void
; ----------------------------------------------------------------------------
os_system_init_cinit:
        mvkl            ___cinit__, a10
     || mvkl            0xffffffff, b0
        mvkh            ___cinit__, a10         ; A10 pointer to record
     || mvkh            0xffffffff, b0
        xor             a10, b0, b0
  [!b0] bnop            m_cinit_return?, 5
mc_cinit_00?:
        ldw             *a10++[0x1], b0         ; B0 copy size
        nop             4
  [!b0] bnop            m_cinit_return?, 5
        ldw             *a10++[0x1], a11        ; A11 pointer to .bss
        mv              b0, a12
        and             a12, 0x7, a12
        mvk             0x8, a13
        sub             a13, a12, a12           ; A12 size shift data
        and             a12, 0x7, a12
mc_cinit_01?:
        ldb             *a10++[0x1], a13
        nop             4
        stb             a13, *a11++[0x1]
        sub             b0, 0x1, b0
        nop             1
   [b0] bnop            mc_cinit_01?, 5
        add             a10, a12, a10
        bnop            mc_cinit_00?, 5
m_cinit_return?:
        b		        b3
		nop	5

; ----------------------------------------------------------------------------
; Initialize global classes (.pinit)
;
; return void
; ----------------------------------------------------------------------------
os_system_init_pinit:
__initClasses__Q2_6OsCore6SystemSFv:
        mv              b3, b10     
        mvkl            ___pinit__, a10
     || mvkl            0xffffffff, b0
        mvkh            ___pinit__, a10         ; A10 pointer to record
     || mvkh            0xffffffff, b0
        xor             a10, b0, b0
  [!b0] bnop            m_pinit_return?, 5
mc_pinit_00?:
        ldw             *a10++[0x1], b0
        nop             4
  [!b0] bnop            m_pinit_return?, 5
        bacc            b0
        bnop            mc_pinit_00?, 5
m_pinit_return?:
        b		        b10
		nop	5
