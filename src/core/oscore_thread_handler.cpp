/**
 * Thread interrupt handler
 * 
 * @author    Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#include "rts.h"
#include "oscore_thread.h"
#include "oscore_thread_handler.h"

namespace oscore
{
  /** 
   * Constructor
   */
  ThreadHandler::ThreadHandler()
  {
  }
  
  /** 
   * Destructor
   */
  ThreadHandler::~ThreadHandler()
  {
  }
  
  /**
   * Handler
   *
   * @return void
   */  
  void ThreadHandler::handle()
  {
    Thread::sheduler();
  }
}
