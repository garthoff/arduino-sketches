#include "Arduino.h"
#include "SMSlib.h"

SendSMS::SendSMS(String textmessage)
{
  _sms = textmessage;
}
