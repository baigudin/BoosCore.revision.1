/** 
 * Hardware interrupt resource.
 * 
 * @author    Sergey Baigudin, baigudin@mail.ru
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#ifndef OSCORE_INTERRUPT_H_
#define OSCORE_INTERRUPT_H_

#include "oscore_resource.h"

namespace oscore
{
  class  InterruptHandler;
  struct Register;

  class Interrupt : public Resource
  {
    public:

      //Available Interrupt sources for TMS320C64x:
      enum Source 
      {
        DSPINT      = 0x00,     //Host port host to DSP interrupt
        TINT0       = 0x01,     //Timer 0 interrupt
        TINT1       = 0x02,     //Timer 1 interrupt
        SD_INTA     = 0x03,     //EMIFA SDRAM timer interrupt
        GPINT4      = 0x04,     //GPIO interrupt 4/External interrupt 4
        GPINT5      = 0x05,     //GPIO interrupt 5/External interrupt 5
        GPINT6      = 0x06,     //GPIO interrupt 6/External interrupt 6
        GPINT7      = 0x07,     //GPIO interrupt 7/External interrupt 7
        EDMA_INT    = 0x08,     //EDMA channel (0 through 63) interrupt
        XINT0       = 0x0c,     //McBSP 0 transmit interrupt
        RINT0       = 0x0d,     //McBSP 0 receive interrupt
        XINT1       = 0x0e,     //McBSP 1 transmit interrupt
        RINT1       = 0x0f,     //McBSP 1 receive interrupt
        GPINT0      = 0x10,     //GPIO interrupt 0
        XINT2       = 0x11,     //McBSP 2 transmit interrupt
        RINT2       = 0x12,     //McBSP 2 receive interrupt
        TINT2       = 0x13,     //Timer 2 interrupt
        SD_INTB     = 0x14,     //EMIFB SDRAM timer interrupt
        PCI_WAKEUP  = 0x15,     //PCI wakeup interrupt
        UINT        = 0x17,     //UTOPIA interupt
        NMI         = 0xe0      //NMI interrupt
      };

                                Interrupt(Source, InterruptHandler*);
      virtual                  ~Interrupt();
      void                      clear();
      void                      jump();
      void                      lock();
      void                      set();
      void                      unlock();
      static int32              disable();     //Describe in ASM file
      static void               enable(int32); //Describe in ASM file
    
    protected:

                                Interrupt();
      void                      removeSource();
      int32                     setSource(Source, InterruptHandler*);

    private:

      struct Mux
      {
        volatile uint32*        reg;
        uint32                  mask;
      };

      struct Vector
      {
        Source                  source;
        InterruptHandler*       handler;
        uint32                  number;
        Mux                     mux;
      };

      static const uint32       NUMBER_VECTORS = 12;

      Vector*                   vector_;
      static Vector             vectors_[NUMBER_VECTORS];
      static void*              stack_;
      static Register*          pointer_;
      static Register*          register_;

      bool                      isAlloc();
      bool                      setMux(Source, uint32, Mux*);
      static bool               init();
      static bool               deinit();
      static void               handle(int32);
      static void               setRegister(Register*);
      static int32              lockLow(uint32);   //Describe in ASM file
      static int32              unlockLow(uint32); //Describe in ASM file
      static int32              setLow(uint32);    //Describe in ASM file
      static int32              clearLow(uint32);  //Describe in ASM file
      static void               jumpLow(uint32);   //Describe in ASM file

      friend class              System;
      friend class              Thread;
  };
}
#endif //OSCORE_INTERRUPT_H_
