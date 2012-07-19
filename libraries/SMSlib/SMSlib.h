/*
 * SMSlib.h - Library for sending SMS messages.
 * It takes care of AT Commands.
 * Currently tested with SM5100B from Sparkfun with SoftwareSerial
 */

#ifndef SMSlib_h
#define SMSlib_h

#include "Arduino.h"

class SendSMS
{
  public:
    void SendSMS(textmessage);
  private:
    void StartSMS();
    void EndSMS();
    String _sms;
};

#endif
