#include "new_msg.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iostream>




CAS2805Msg::CAS2805Msg(const char* message) 
    : m_message_len(0),
      m_bBitmapsValid(false),
      m_bIsValidISOMsg(false)
{
    memset(m_message, 0, sizeof(m_message));
    memset(_logbuf, 0, sizeof(m_message));
//    strcpy(m_message, message);
    memcpy(m_message, message, strlen(message)+1 );
    
}





bool CAS2805Msg::PackMessage() 
{ 
    m_message_len = PackReply(m_message, sizeof(m_message)); 
/*
   
    if (!m_bIsValidISOMsg) { 
        std::string errormsg; 
        errormsg += "CAS2805Msg::PackMessage - could not pack messsage"; 
        WriteLogMessage(errormsg.c_str(), LOGALL);
        return false; 
    } 
    return m_bIsValidISOMsg; 
*/
} 

int  CAS2805Msg::PackReply(BYTE *Dest, int nBufLen)
{
    BYTE *p, *p1;
    char text[200];
    int  a;

    int bit28on = IsFieldPresent(28);
    int bit29on = IsFieldPresent(29);
    int bit48on = IsFieldPresent(48);
    int bit57on = IsFieldPresent(57);

    SetReplyBitmap();

    if (bit28on) {
        SetBitOn(28);
    }
    if (bit29on) {
        SetBitOn(29);
    }
    if (bit48on) {
        SetBitOn(48);
    }
    if (bit57on) {
        SetBitOn(57);
    }

    p = Dest;

    p = PackNumericRight    (p, m_i.MsgType, 4, NULL);

    memcpy(p, m_i.BitMap,8);
    p += 8;

    if (IsFieldPresent(1))
    {
        memcpy(p, m_i.BitMap+8, 8);
        p += 8;
    }

    if (IsFieldPresent(2))
        p = PackNumericLeft  (p, m_i.F2Pan, 0, m_i.F2Len, 0x0f);

    if (IsFieldPresent(3))
        p = PackNumericRight (p, m_i.F3ProcessingCode, 6, NULL);

    if (IsFieldPresent(4))
        p = PackNumericRight (p, m_i.F4Amount, 12, NULL);

    if (IsFieldPresent(5))
        p = PackNumericRight (p, m_i.F5AmountSettlement, 12, NULL);

    if (IsFieldPresent(6))
        p = PackNumericRight (p, m_i.F6AmountCardholder, 12, NULL);

    if (IsFieldPresent(7))
        p = PackNumericRight (p, m_i.F7DateTime, 10, NULL);

    if (IsFieldPresent(8))
        p = PackNumericRight (p, m_i.F8AmountCardBillFee, 8, NULL);

    if (IsFieldPresent(9))
        p = PackNumericRight (p, m_i.F9ConvRateSettle, 8, NULL);

    if (IsFieldPresent(10))
        p = PackNumericRight (p, m_i.F10ConversionRate, 8, NULL);

    if (IsFieldPresent(11))
        p = PackNumericRight (p, m_i.F11Stan, 6, NULL);

    if (IsFieldPresent(12))
        p = PackNumericRight (p, m_i.F12TimeLocalTrans, 6, NULL);

    if (IsFieldPresent(13))
        p = PackNumericRight (p, m_i.F13DateLocalTrans, 4, NULL);

    if (IsFieldPresent(14))
        p = PackNumericRight (p, m_i.F14ExpiryDate, 4, NULL);

    if (IsFieldPresent(15))
        p = PackNumericRight (p, m_i.F15DateSettlement, 4, NULL);

    if (IsFieldPresent(16))
        p = PackNumericRight (p, m_i.F16DateConversion, 4, NULL);

    if (IsFieldPresent(17))
        p = PackNumericRight (p, m_i.F17DateCapture, 4, NULL);

    if (IsFieldPresent(18))
        p = PackNumericRight (p, m_i.F18MerchantType, 4, NULL);

    if (IsFieldPresent(19))
        p = PackNumericRight (p, m_i.F19AcqCtryCode, 3, NULL);

    if (IsFieldPresent(20))
        p = PackNumericRight (p, m_i.F20PanCountryCode, 3, NULL);

    if (IsFieldPresent(21))
        p = PackNumericRight (p, m_i.F21FwdCtryCode, 3, NULL);

    if (IsFieldPresent(22))
        p = PackNumericRight (p, m_i.F22PosEntryMode, 3, NULL);

    if (IsFieldPresent(23))
        p = PackNumericRight (p, m_i.F23CardSeqNumber, 3, NULL);

    if (IsFieldPresent(24))
        p = PackNumericRight (p, m_i.F24NetworkIdentifier, 3, NULL);

    if (IsFieldPresent(25))
        p = PackNumericRight (p, m_i.F25PosCondCode, 2, NULL);

    if (IsFieldPresent(26))
        p = PackNumericRight (p, m_i.F26PosCaptureCode, 2, NULL);

    if (IsFieldPresent(27))
        p = PackUnsupported  (p, 27);

    if (IsFieldPresent(28))
        p = PackAmtSign      (p, m_i.F28TransactionFee, 8);

    if (IsFieldPresent(29))
        p = PackAmtSign      (p, m_i.F29SettlementFee, 8);

    if (IsFieldPresent(30))
        p = PackAmtSign      (p, m_i.F30TranProcFee, 8);

    if (IsFieldPresent(31))
        p = PackAmtSign      (p, m_i.F31SettleProcFee, 8);

    if (IsFieldPresent(32))
        p = PackNumericLeft  (p, m_i.F32AcquiringId, 0, m_i.F32Len, 0x0f);

    if (IsFieldPresent(33))
        p = PackNumericLeft  (p, m_i.F33ForwardingId, 0, m_i.F33Len, 0x0f);

    if (IsFieldPresent(34))
        p = PackNumericLeft  (p, m_i.F34PanExtended, 0, m_i.F34Len, 0x0f);

    if (IsFieldPresent(35))
        p = PackNumericLeft  (p, m_i.F35Track2, 0, m_i.F35Len, 0x0f);

    if (IsFieldPresent(36))
        p = PackNumericLeft  (p, m_i.F36Track3, -1, m_i.F36Len, 0x0f);

    if (IsFieldPresent(37))
        p = PackAlpha        (p, m_i.F37RefNumber, 12, NULL);

    if (IsFieldPresent(38))
        p = PackAlpha        (p, m_i.F38AuthNumber, 6, NULL);

    if (IsFieldPresent(39))
        p = PackAlpha        (p, m_i.F39ResponseCode, 2, NULL);

    if (IsFieldPresent(40))
        p = PackAlpha        (p, m_i.F40ServiceCode, 3, NULL);

    if (IsFieldPresent(41))
        p = PackAlpha        (p, m_i.F41TerminalId, 8, NULL);

    if (IsFieldPresent(42))
        p = PackAlpha        (p, m_i.F42CardAcceptId, 15, NULL);

    if (IsFieldPresent(43))
        p = PackAlpha        (p, m_i.F43CardAcceptName, 40, NULL);

    if (IsFieldPresent(44))
        p = PackAlpha        (p, m_i.F44AdditionalResp, 0, m_i.F44Len);

    if (IsFieldPresent(45))
        p = PackAlpha        (p, m_i.F45Track1, 0, m_i.F45Len);

    if (IsFieldPresent(46))
        p = PackAlpha        (p, m_i.F46AddDataIso, -1, m_i.F46Len);

    if (IsFieldPresent(47))
        p = PackAlpha        (p, m_i.F47AddDataNational, -1, m_i.F47Len);

    if (IsFieldPresent(48))
        p = PackAlpha        (p, m_i.F48AdditionalData, -1, m_i.F48Len);

    if (IsFieldPresent(49))
        p = PackNumericRight (p, m_i.F49CurrencyCode, 3, NULL);

    if (IsFieldPresent(50))
        p = PackNumericRight (p, m_i.F50CurrencyCodeStl, 3, NULL);

    if (IsFieldPresent(51))
        p = PackNumericRight (p, m_i.F51CurrencyCodeChb, 3, NULL);

    if (IsFieldPresent(52))
        p = PackBinary       (p, m_i.F52PinBlock, 8, NULL);

    if (IsFieldPresent(53))
        p = PackNumericRight (p, m_i.F53SecurityInfo, 16, NULL);

    if (IsFieldPresent(54))
        p = PackAlpha        (p, m_i.F54BalanceInfo, -1, m_i.F54Len);

    if (IsFieldPresent(55))
        p = PackUnsupported  (p, 55);

    if (IsFieldPresent(56))
        p = PackUnsupported  (p, 56);

    if (IsFieldPresent(57))
        p = PackNumericRight (p, m_i.F57AmountCash, 12, NULL);

    if (IsFieldPresent(58))
        p = PackNumericRight (p, m_i.F58LedgerBalance, 12, NULL);

    if (IsFieldPresent(59))
        p = PackNumericRight (p, m_i.F59AvailBalance, 12, NULL);

    if (IsFieldPresent(60))
        p = PackAlpha         (p, (char *) m_i.F60Private, -1, m_i.F60Len);

    if (IsFieldPresent(61))
        p = PackAlpha         (p, (char *) m_i.F61Private, -1, m_i.F61Len);

    if (IsFieldPresent(62))
        p = PackAlpha         (p, (char *) m_i.F62Private, -1, m_i.F62Len);

    if (IsFieldPresent(63))
        p = PackAlpha         (p, (char *) m_i.F63Private, -1, m_i.F63Len);

    if (IsFieldPresent(64))
        p = PackBinary       (p, m_i.F64Mac, 8, NULL);

    if (IsFieldPresent(65))
        p = PackUnsupported  (p, 65);

    if (IsFieldPresent(66))
        p = PackNumericRight (p, m_i.F66SettlementCode, 1, NULL);

    if (IsFieldPresent(67))
        p = PackNumericRight (p, m_i.F67ExtendedPayCode, 2, NULL);

    if (IsFieldPresent(68))
        p = PackNumericRight (p, m_i.F68ReceiveCtryCode, 3, NULL);

    if (IsFieldPresent(69))
        p = PackNumericRight (p, m_i.F69SettleCtryCode, 3, NULL);

    if (IsFieldPresent(70))
        p = PackNumericRight (p, m_i.F70NetworkInfoCode, 3, NULL);

    if (IsFieldPresent(71))
        p = PackNumericRight (p, m_i.F71MessageNumber, 4, NULL);

    if (IsFieldPresent(72))
        p = PackNumericRight (p, m_i.F72MessageNumberLast, 4, NULL);

    if (IsFieldPresent(73))
        p = PackNumericRight (p, m_i.F73DateAction, 6, NULL);

    if (IsFieldPresent(74))
        p = PackNumericRight (p, m_i.F74CreditsNbr, 10, NULL);

    if (IsFieldPresent(75))
        p = PackNumericRight (p, m_i.F75CreditsRevNbr, 10, NULL);

    if (IsFieldPresent(76))
        p = PackNumericRight (p, m_i.F76DebitsNbr, 10, NULL);

    if (IsFieldPresent(77))
        p = PackNumericRight (p, m_i.F77DebitsRevNbr, 10, NULL);

    if (IsFieldPresent(78))
        p = PackNumericRight (p, m_i.F78TransferNumber, 10, NULL);

    if (IsFieldPresent(79))
        p = PackNumericRight (p, m_i.F79TransferReversalNum, 10, NULL);

    if (IsFieldPresent(80))
        p = PackNumericRight (p, m_i.F80InquiriesNumber, 10, NULL);

    if (IsFieldPresent(81))
        p = PackNumericRight (p, m_i.F81AuthorisationsNumber, 10, NULL);

    if (IsFieldPresent(82))
        p = PackNumericRight (p, m_i.F82CreditProcFeeAmt, 12, NULL);

    if (IsFieldPresent(83))
        p = PackNumericRight (p, m_i.F83CreditTranFeeAmt, 12, NULL);

    if (IsFieldPresent(84))
        p = PackNumericRight (p, m_i.F84DebitProcFeeAmt, 12, NULL);

    if (IsFieldPresent(85))
        p = PackNumericRight (p, m_i.F85DebitTranFeeAmt, 12, NULL);

    if (IsFieldPresent(86))
        p = PackNumericRight (p, m_i.F86CreditsAmt, 16, NULL);

    if (IsFieldPresent(87))
        p = PackNumericRight (p, m_i.F87CreditsRevAmt, 16, NULL);

    if (IsFieldPresent(88))
        p = PackNumericRight (p, m_i.F88DebitsAmt, 16, NULL);

    if (IsFieldPresent(89))
        p = PackNumericRight (p, m_i.F89DebitsRevAmt, 16, NULL);

    if (IsFieldPresent(90))
        p = PackNumericRight (p, m_i.F90OriginalData, 42, NULL);

    if (IsFieldPresent(91))
        p = PackAlpha        (p, m_i.F91FileUpdateCode, 1, NULL);

    if (IsFieldPresent(92))
        p = PackAlpha        (p, m_i.F92FileSecurityCode, 2, NULL);

    if (IsFieldPresent(93))
        p = PackAlpha        (p, m_i.F93ResponseId, 5, NULL);

    if (IsFieldPresent(94))
        p = PackAlpha        (p, m_i.F94ServiceIndicator, 7, NULL);

    if (IsFieldPresent(95))
        p = PackAlpha        (p, m_i.F95ReplaceAmounts, 42, NULL);

    if (IsFieldPresent(96))
        p = PackBinary       (p, m_i.F96SecurityCode, 8, NULL);

    if (IsFieldPresent(97))
        p = PackAmtSign      (p, m_i.F97AmtNetSettle, 16);

    if (IsFieldPresent(98))
        p = PackAlpha        (p, m_i.F98Payee, 0, m_i.F98Len);

    if (IsFieldPresent(99))
        p = PackNumericLeft  (p, m_i.F99SettlementId, 0, m_i.F99Len, 0x0f);

    if (IsFieldPresent(100))
        p = PackNumericLeft  (p, m_i.F100ReceivingId, 0, m_i.F100Len, 0x0f);

    if (IsFieldPresent(101))
        p = PackAlpha        (p, m_i.F101FileName, 0, m_i.F101Len);

    if (IsFieldPresent(102))
        p = PackAlpha        (p, m_i.F102Account1, 0, m_i.F102Len);

    if (IsFieldPresent(103))
        p = PackAlpha        (p, m_i.F103Account2, 0, m_i.F103Len);

    if (IsFieldPresent(104))
        p = PackAlpha        (p, m_i.F104Description, 0, m_i.F104Len);

    if (IsFieldPresent(105))
        p = PackUnsupported  (p, 105);

    if (IsFieldPresent(106))
        p = PackUnsupported  (p, 106);

    if (IsFieldPresent(107))
        p = PackUnsupported  (p, 107);

    if (IsFieldPresent(108))
        p = PackUnsupported  (p, 108);

    if (IsFieldPresent(109))
        p = PackUnsupported  (p, 109);

    if (IsFieldPresent(110))
        p = PackUnsupported  (p, 110);

    if (IsFieldPresent(111))
        p = PackUnsupported  (p, 111);

    if (IsFieldPresent(112))
        p = PackUnsupported  (p, 112);

    if (IsFieldPresent(113))
        p = PackUnsupported  (p, 113);

    if (IsFieldPresent(114))
        p = PackUnsupported  (p, 114);

    if (IsFieldPresent(115))
        p = PackUnsupported  (p, 115);

    if (IsFieldPresent(116))
        p = PackUnsupported  (p, 116);

    if (IsFieldPresent(117))
        p = PackAlpha        (p, m_i.F117CardStatusUpdCode, 2, NULL);

    if (IsFieldPresent(118))
        p = PackNumericRight (p, m_i.F118CashTotalNum, 10, NULL);

    if (IsFieldPresent(119))
        p = PackNumericRight (p, m_i.F119CashTotalAmt, 16, NULL);

    if (IsFieldPresent(120))
        p = PackAlpha        (p, (char *) m_i.F120Private, -1, m_i.F120Len);

    if (IsFieldPresent(121))
        p = PackUnsupported  (p, 121);

    if (IsFieldPresent(122))
        p = PackUnsupported  (p, 122);

    if (IsFieldPresent(123))
        p = PackUnsupported  (p, 123);

    if (IsFieldPresent(124))
        p = PackUnsupported  (p, 124);

    if (IsFieldPresent(125))
        p = PackUnsupported  (p, 125);

    if (IsFieldPresent(126))
        p = PackUnsupported  (p, 126);

    if (IsFieldPresent(127))
        p = PackUnsupported    (p, 127);

    if (IsFieldPresent(128))
        p = PackBinary       (p, m_i.F128Mac, 8, NULL);

    sprintf(_logbuf,"Outgoing .................. %s",m_i.MsgType);
    WriteLogMessage(_logbuf);
    

    if (IsFieldPresent(1))
    {
        sprintf(_logbuf,"Bit Map. .................. %.02X%.02X%.02X%.02X%.02X%.02X%.02X%.02X%.02X%.02X%.02X%.02X%.02X%.02X%.02X%.02X",
                m_i.BitMap[0], m_i.BitMap[1], m_i.BitMap[2], m_i.BitMap[3],
                m_i.BitMap[4], m_i.BitMap[5], m_i.BitMap[6], m_i.BitMap[7],
                m_i.BitMap[8], m_i.BitMap[9], m_i.BitMap[10],m_i.BitMap[11],
                m_i.BitMap[12],m_i.BitMap[13],m_i.BitMap[14],m_i.BitMap[15]);
        WriteLogMessage(_logbuf);
    }
    else
    {
        sprintf(_logbuf,"Bit Map. .................. %.02X%.02X%.02X%.02X%.02X%.02X%.02X%.02X",
                m_i.BitMap[0], m_i.BitMap[1], m_i.BitMap[2], m_i.BitMap[3],
                m_i.BitMap[4], m_i.BitMap[5], m_i.BitMap[6], m_i.BitMap[7]);
        WriteLogMessage(_logbuf);
    }

    for (a = 2; a < 129; a++)
    {
        if (IsFieldPresent(a))
        {
            LogSwitchMsg (a, text);
            sprintf(_logbuf,"%s",text);
            WriteLogMessage(_logbuf);
        }
    }

    if (p == NULL)
    {
        m_bBitmapsValid = false;  //we have a good message!!! :)
        m_bIsValidISOMsg = false;
    }
    else
    {
        m_bBitmapsValid = true;  //we have a good message!!! :)
        m_bIsValidISOMsg = true;
    }

    p1 = Dest;
    a = p - p1;

    return a;

}

// Pack a binary field

BYTE *CAS2805Msg::PackBinary(BYTE *p, BYTE *field, int len, char *field_len)
{
    int length;

    length = len;

    if (length == 0)
    {
        length = atoi(field_len);
        *p++ = length;
    }

    memcpy(p, field, length);

    p += length;

    return p;

}

// Pack an alpha field for an as2805 link. If the length is 0; then length
// is taken from field_length parameter and placed in the first two bytes (LLVAR).
// If length is -1 then LLLVAR field is processed.

BYTE *CAS2805Msg::PackAlpha(BYTE *p, char *field, int len, char *field_length)
{
    int  a, length;
    char wk_len[3+1];

    length = len;

    if (length == 0)
    {
        length = atoi(field_length);
        sprintf(wk_len,"%.02d",length);
        *p++ = wk_len[0];
        *p++ = wk_len[1];
    }

    if (length == -1)
    {
        length = atoi(field_length);
        sprintf(wk_len,"%.03d",length);
        *p++ = wk_len[0];
        *p++ = wk_len[1];
        *p++ = wk_len[2];
    }

    for (a = 0; a < length; a++)
        if (field[a] == 0)
            *p++ = ' ';
        else
            *p++ = field[a];

    return p;

}

// Pack a fixed length numeric BCD data. Length may be odd or even
// If odd length then pad first half of the first byte with a zero
// If len = 0 the length is held as packed data in the first byte
// of the field. If len = -1 then length is held in the first two
// bytes of the field

BYTE *CAS2805Msg::PackNumericRight(BYTE *p, char *field, int len, char *field_len)
{
    BYTE w1[5];
    BYTE x;
    int  a, length;

    length = len;

    if (length == 0)
    {
        length = atoi(field_len);
        sprintf((char *)w1,"%.02d",length);
        x = ((w1[0] & 0x0F) << 4) | (w1[1] & 0x0F);
        *p++ = x;
    }

    if (length == -1)
    {
        length = atoi(field_len);
        sprintf((char *)w1,"%.04d",length);
        x = ((w1[0] & 0x0F) << 4) | (w1[1] & 0x0F);
        *p++ = x;
        x = ((w1[2] & 0x0F) << 4) | (w1[3] & 0x0F);
        *p++ = x;
    }

    for (a = 0; a < length; a += 2)
    {
        if (a == 0 && (length & 0x01))
        {
            w1[0] = '0';
            w1[1] = *field++;
        }
        else
        {
            w1[0] = *field++;
            w1[1] = *field++;
        }

        if (w1[0] >= 'A')
            w1[0] -= 7;
        if (w1[1] >= 'A')
            w1[1] -= 7;

        w1[0] -= 0x30;
        w1[1] -= 0x30;

        x = ((w1[0] & 0x0F) << 4) | (w1[1] & 0x0F);
        *p++ = x;
    }

    return (p);

}

// Pack a fixed length numeric BCD data. Length may be odd or even
// If odd length then pad last half of the last byte with pad value
// If len = 0 the length is held as packed data in the first byte
// of the field. If len = -1 then length is held in the first two
// bytes of the field

BYTE *CAS2805Msg::PackNumericLeft(BYTE *p, char *field, int len, char *field_len, char pad)
{
    BYTE w1[5];
    BYTE x;
    int  a, length;

    length = len;

    if (length == 0)
    {
        length = atoi(field_len);
        sprintf((char *)w1,"%.02d",length);
        x = ((w1[0] & 0x0F) << 4) | (w1[1] & 0x0F);
        *p++ = x;
    }

    if (length == -1)
    {
        length = atoi(field_len);
        sprintf((char *)w1,"%.04d",length);
        x = ((w1[0] & 0x0F) << 4) | (w1[1] & 0x0F);
        *p++ = x;
        x = ((w1[2] & 0x0F) << 4) | (w1[3] & 0x0F);
        *p++ = x;
    }

    for (a = 0; a < length; a += 2)
    {
        w1[0] = *field++;
        w1[1] = *field++;

        if (w1[0] >= 'A')
            w1[0] -= 7;
        if (w1[1] >= 'A')
            w1[1] -= 7;

        w1[0] -= 0x30;
        w1[1] -= 0x30;

        x = ((w1[0] & 0x0F) << 4) | (w1[1] & 0x0F);
        *p++ = x;
    }

    if (length & 0x01)
    {
        p--;
        x = ((w1[0] & 0x0F) << 4) | (pad & 0x0F);
        *p++ = x;
    }

    return (p);

}

// Pack an amount field. This has a one byte sign.

BYTE *CAS2805Msg::PackAmtSign(BYTE *p, char *field, int len)
{
    BYTE w1[3];
    BYTE x;
    int a;

    *p++ = *field++;

    for (a = 0; a < len; a += 2)
    {
        w1[0] = *field++;
        w1[1] = *field++;
        w1[0] -= 0x30;
        w1[1] -= 0x30;
        x = ((w1[0] & 0x0F) << 4) | (w1[1] & 0x0F);
        *p++ = x;
    }

    return (p);

}

BYTE *CAS2805Msg::PackUnsupported(BYTE *Dest, int field)
{

    return Dest;

}

int CAS2805Msg::SetReplyBitmap()
{
    const char *test = NULL;

    {
        switch (atoi(m_i.MsgType))
        {
        case 110:
            test = POS_0110_MANDATORY_BITS;
            break;

        case 130:
            test = POS_0130_MANDATORY_BITS;
            break;

        case 210:
            test = POS_0210_MANDATORY_BITS;
            break;

        case 230:
            test = POS_0230_MANDATORY_BITS;
            break;

        case 430:
            test = POS_0430_MANDATORY_BITS;
            break;

        case 810:
            test = NET_0810_MANDATORY_BITS;
            break;

        default:
            break;
        }
    }

    if (test == NULL)
        return 1;

    PackNumericRight(m_i.BitMap, (char *) test, 32, NULL);

    return 0;

}


int CAS2805Msg::IsFieldPresent(int field)
{
BYTE bits[] = {0x80,0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
int  a, b;

    field--;
    a = (field / 8);
    b = (field % 8);

    if (m_i.BitMap[a] & bits[b])
        return 1;
    else
        return 0;

}


void CAS2805Msg::SetBitOn(int bit)
{
    int a, b;
    BYTE bits[] = {0x80,0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

    bit--;
    a = (bit / 8);
    b = (bit % 8);

    m_i.BitMap[a] |= bits[b];
    return;

}

void CAS2805Msg::SetBitOff(int bit)
{
    int a, b;
    BYTE bits[] = {0x7f,0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd, 0xfe};

    bit--;
    a = (bit / 8);
    b = (bit % 8);

    m_i.BitMap[a] &= bits[b];
    return;

}



void CAS2805Msg::WriteLogMessage(const char* message)
{
    std::cout << message << std::endl;
}


void CAS2805Msg::LogSwitchMsg (int field, char *text)
{
    switch (field)
    {
    case   2 :
                // Ticket #142
        sprintf(text,"2 Primary Account Number .. (%s) %s",m_i.F2Len, m_i.F2Pan);
        break;
    case   3 :
        sprintf(text,"3 Processing Code.......... %s",m_i.F3ProcessingCode);
        break;
    case   4 :
        sprintf(text,"4 Amount................... %s",m_i.F4Amount);
        break;
    case   5 :
        sprintf(text,"5 Amount Settlement........ %s",m_i.F5AmountSettlement);
        break;
    case   6 :
        sprintf(text,"6 Amount Cardholder........ %s",m_i.F6AmountCardholder);
        break;
    case   7 :
        sprintf(text,"7 Date and Time............ %s",m_i.F7DateTime);
        break;
    case   8 :
        sprintf(text,"8 Amount, Card Bill Fee.... %s",m_i.F8AmountCardBillFee);
        break;
    case   9 :
        sprintf(text,"9 Conversion Rate Settle... %s",m_i.F9ConvRateSettle);
        break;
    case  10 :
        sprintf(text,"10 Conversion Rate......... %s",m_i.F10ConversionRate);
        break;
    case  11 :
        sprintf(text,"11 System Trace Number..... %s",m_i.F11Stan);
        break;
    case  12 :
        sprintf(text,"12 Time, Local Trans....... %s",m_i.F12TimeLocalTrans);
        break;
    case  13 :
        sprintf(text,"13 Date, Local Trans....... %s",m_i.F13DateLocalTrans);
        break;
    case  14 :
        sprintf(text,"14 Expiry Date............. %s",m_i.F14ExpiryDate);
        break;
    case  15 :
        sprintf(text,"15 Date Settlement......... %s",m_i.F15DateSettlement);
        break;
    case  16 :
        sprintf(text,"16 Date Conversion......... %s",m_i.F16DateConversion);
        break;
    case  17 :
        sprintf(text,"17 Date Capture............ %s",m_i.F17DateCapture);
        break;
    case  18 :
        sprintf(text,"18 Merchant Type........... %s",m_i.F18MerchantType);
        break;
    case  19 :
        sprintf(text,"19 Acquiring Country....... %s",m_i.F19AcqCtryCode);
        break;
    case  20 :
        sprintf(text,"20 PAN Country Code........ %s",m_i.F20PanCountryCode);
        break;
    case  21 :
        sprintf(text,"21 Forwarding Country Code. %s",m_i.F21FwdCtryCode);
        break;
    case  22 :
        sprintf(text,"22 POS Entry Mode.......... %s",m_i.F22PosEntryMode);
        break;
    case  23 :
        sprintf(text,"23 Card Sequence Number.... %s",m_i.F23CardSeqNumber);
        break;
    case  24 :
        sprintf(text,"24 Network Identifier...... %s",m_i.F24NetworkIdentifier);
        break;
    case  25 :
        sprintf(text,"25 POS Cond Code........... %s",m_i.F25PosCondCode);
        break;
    case  26 :
        sprintf(text,"26 POS Capture Code........ %s",m_i.F26PosCaptureCode);
        break;
    case  27 :
        sprintf(text,"27 Authorisation ID Length. %s",m_i.F27AuthIdRespLen);
        break;
    case  28 :
        sprintf(text,"28 Transaction Fee ........ %s",m_i.F28TransactionFee);
        break;
    case  29 :
        sprintf(text,"29 Settlement Fee.......... %s",m_i.F29SettlementFee);
        break;
    case  30 :
        sprintf(text,"30 Transaction Proc Fee.... %s",m_i.F30TranProcFee);
        break;
    case  31 :
        sprintf(text,"31 Settlement Proc Fee..... %s",m_i.F31SettleProcFee);
        break;
    case  32 :
        sprintf(text,"32 Acquiring Institution... (%s) %s",m_i.F32Len,m_i.F32AcquiringId);
        break;
    case  33 :
        sprintf(text,"33 Forwarding Institution.. (%s) %s",m_i.F33Len,m_i.F33ForwardingId);
        break;
    case  34 :
        sprintf(text,"34 PAN Extended............ (%s) %s",m_i.F34Len,m_i.F34PanExtended);
        break;
    case  35 :
                // Ticket #142
        sprintf(text,"35 Track 2 Data............ (%s) %s",m_i.F35Len, m_i.F35Track2);
        break;
    case  36 :
        // Ticket #263, can switch to obfuscate later if needed
        //sprintf(text,"36 Track 3 Data............ (%s) %s",m_i.F36Len,m_i.F36Track3);
        break;
    case  37 :
        sprintf(text,"37 Retrieval Reference No.. %s",m_i.F37RefNumber);
        break;
    case  38 :
        sprintf(text,"38 Authorisation Number.... %s",m_i.F38AuthNumber);
        break;
    case  39 :
        sprintf(text,"39 Response Code........... %s",m_i.F39ResponseCode);
        break;
    case  40 :
        sprintf(text,"40 Service Code............ %s",m_i.F40ServiceCode);
        break;
    case  41 :
        sprintf(text,"41 Terminal ID............. %s",m_i.F41TerminalId);
        break;
    case  42 :
        sprintf(text,"42 Card Accept ID.......... %s",m_i.F42CardAcceptId);
        break;
    case  43 :
        sprintf(text,"43 Card Accept Name........ %s",m_i.F43CardAcceptName);
        break;
    case  44 :
        sprintf(text,"44 Additional Response Data (%s) %s",m_i.F44Len,m_i.F44AdditionalResp);
        break;
    case  45 :
        // Ticket #263, can switch to obfuscate later if needed
        //sprintf(text,"45 Track 1 Data............ (%s) %s",m_i.F45Len,m_i.F45Track1);
        break;
    case  46 :
        sprintf(text,"46 Additional Data ISO..... (%s) %s",m_i.F46Len,m_i.F46AddDataIso);
        break;
    case  47 :
        sprintf(text,"47 Additional Data National (%s) %s",m_i.F47Len,m_i.F47AddDataNational);
        break;
    case  48 :
        sprintf(text,"48 Additional Data Private. (%s) %s",m_i.F48Len,m_i.F48AdditionalData);
        break;
    case  49 :
        sprintf(text,"49 Currency Code........... %s",m_i.F49CurrencyCode);
        break;
    case  50 :
        sprintf(text,"50 Currency Code Settle.... %s",m_i.F50CurrencyCodeStl);
        break;
    case  51 :
        sprintf(text,"51 Currency Code Billing... %s",m_i.F51CurrencyCodeChb);
        break;
    case  52 :
        sprintf(text,"52 PIN Block............... %.02X%.02X%.02X%.02X%.02X%.02X%.02X%.02X",
                        m_i.F52PinBlock[0],m_i.F52PinBlock[1],
                        m_i.F52PinBlock[2],m_i.F52PinBlock[3],
                        m_i.F52PinBlock[4],m_i.F52PinBlock[5],
                        m_i.F52PinBlock[6],m_i.F52PinBlock[7]);
        break;
    case  53 :
        sprintf(text,"53 Security Info........... %s",m_i.F53SecurityInfo);
        break;
    case  54 :
        sprintf(text,"54 Balance Info ........... (%s) %s",m_i.F54Len,m_i.F54BalanceInfo);
        break;
    case  55 :
        sprintf(text,"%d Unsupported Field",55);
        break;
    case  56 :
        sprintf(text,"%d Unsupported Field",56);
        break;
    case  57 :
        sprintf(text,"57 Amount Cash............. %s",m_i.F57AmountCash);
        break;
    case  58 :
        sprintf(text,"58 Acct Balance(Ledger).... %s",m_i.F58LedgerBalance);
        break;
    case  59 :
        sprintf(text,"59 Acct Balance(Available). %s",m_i.F59AvailBalance);
        break;
    case  60 :
        sprintf(text,"60 Private Data............ (%s) %.100s",m_i.F60Len,m_i.F60Private);
        break;
    case  61 :
        sprintf(text,"61 Private Data............ (%s) %.100s",m_i.F61Len,m_i.F61Private);
        break;
    case  62 :
        sprintf(text,"62 Private Data............ (%s) %.100s",m_i.F62Len,m_i.F62Private);
        break;
    case  63 :
        sprintf(text,"63 Private Data............ (%s) %.100s",m_i.F63Len,m_i.F63Private);
        break;
    case  64 :
        sprintf(text,"64 MAC..................... %.02X%.02X%.02X%.02X%.02X%.02X%.02X%.02X",
                        m_i.F64Mac[0],m_i.F64Mac[1],m_i.F64Mac[2],m_i.F64Mac[3],
                        m_i.F64Mac[4],m_i.F64Mac[5],m_i.F64Mac[6],m_i.F64Mac[7]);
    case  65 :
        sprintf(text,"%d Unsupported Field",65);
        break;
    case  66 :
        sprintf(text,"66 Settlement Code......... %s",m_i.F66SettlementCode);
        break;
    case  67 :
        sprintf(text,"67 Extended Payment Code... %s",m_i.F67ExtendedPayCode);
        break;
    case  68 :
        sprintf(text,"68 Receiving Country....... %s",m_i.F68ReceiveCtryCode);
        break;
    case  69 :
        sprintf(text,"69 Settlement Country...... %s",m_i.F69SettleCtryCode);
        break;
    case  70 :
        sprintf(text,"70 Network Info Code....... %s",m_i.F70NetworkInfoCode);
        break;
    case  71 :
        sprintf(text,"71 Message Number.......... %s",m_i.F71MessageNumber);
        break;
    case  72 :
        sprintf(text,"72 Message Number Last..... %s",m_i.F72MessageNumberLast);
        break;
    case  73 :
        sprintf(text,"73 Date Action............. %s",m_i.F73DateAction);
        break;
    case  74 :
        sprintf(text,"74 Credits Number.......... %s",m_i.F74CreditsNbr);
        break;
    case  75 :
        sprintf(text,"75 Credit Reversals Number. %s",m_i.F75CreditsRevNbr);
        break;
    case  76 :
        sprintf(text,"76 Debits Number........... %s",m_i.F76DebitsNbr);
        break;
    case  77 :
        sprintf(text,"77 Debits Reversals Number. %s",m_i.F77DebitsRevNbr);
        break;
    case  78 :
        sprintf(text,"78 Transfers Number........ %s",m_i.F78TransferNumber);
        break;
    case  79 :
        sprintf(text,"79 Transfer Reversals, Num. %s",m_i.F79TransferReversalNum);
        break;
    case  80 :
        sprintf(text,"80 Inquiries Number........ %s",m_i.F80InquiriesNumber);
        break;
    case  81 :
        sprintf(text,"81 Authorisations Number... %s",m_i.F81AuthorisationsNumber);
        break;
    case  82 :
        sprintf(text,"82 Credits Processing Fee.. %s",m_i.F82CreditProcFeeAmt);
        break;
    case  83 :
        sprintf(text,"83 Credits Transaction Fee. %s",m_i.F83CreditTranFeeAmt);
        break;
    case  84 :
        sprintf(text,"84 Debits Processing Fee... %s",m_i.F84DebitProcFeeAmt);
        break;
    case  85 :
        sprintf(text,"85 Debits Transaction Fee.. %s",m_i.F85DebitTranFeeAmt);
        break;
    case  86 :
        sprintf(text,"86 Credits Amount.......... %s",m_i.F86CreditsAmt);
        break;
    case  87 :
        sprintf(text,"87 Credit Reversal Amount.. %s",m_i.F87CreditsRevAmt);
        break;
    case  88 :
        sprintf(text,"88 Debits Amount........... %s",m_i.F88DebitsAmt);
        break;
    case  89 :
        sprintf(text,"89 Debits Rev Amt.......... %s",m_i.F89DebitsRevAmt);
        break;
    case  90 :
        sprintf(text,"90 Original Data Elements.. %s",m_i.F90OriginalData);
        break;
    case  91 :
        sprintf(text,"91 File Update Code........ %s",m_i.F91FileUpdateCode);
        break;
    case  92 :
        sprintf(text,"92 File Security Code...... %s",m_i.F92FileSecurityCode);
        break;
    case  93 :
        sprintf(text,"93 Response ID............. %s",m_i.F93ResponseId);
        break;
    case  94 :
        sprintf(text,"94 Service Indicator....... %s",m_i.F94ServiceIndicator);
        break;
    case  95 :
        sprintf(text,"95 Replace Amounts......... %s",m_i.F95ReplaceAmounts);
        break;
    case  96 : sprintf(text,"96 Security Code........... %.02X%.02X%.02X%.02X%.02X%.02X%.02X%.02X",
                        m_i.F96SecurityCode[0],m_i.F96SecurityCode[1],
                        m_i.F96SecurityCode[2],m_i.F96SecurityCode[3],
                        m_i.F96SecurityCode[4],m_i.F96SecurityCode[5],
                        m_i.F96SecurityCode[6],m_i.F96SecurityCode[7]);
        break;
    case  97 :
        sprintf(text,"97 Amount Net Settlement... %s",m_i.F97AmtNetSettle);
        break;
    case  98 :
        sprintf(text,"98 Payee................... (%s) %s",m_i.F98Len,m_i.F98Payee);
        break;
    case  99 :
        sprintf(text,"99 Settlement ID........... (%s) %s",m_i.F99Len,m_i.F99SettlementId);
        break;
    case 100 :
        sprintf(text,"100 Receiving ID .......... (%s) %s",m_i.F100Len,m_i.F100ReceivingId);
        break;
    case 101 :
        sprintf(text,"101 File Name.............. (%s) %s",m_i.F101Len,m_i.F101FileName);
        break;
    case 102 :
        sprintf(text,"102 Account 1.............. (%s) %s",m_i.F102Len,m_i.F102Account1);
        break;
    case 103 :
        sprintf(text,"103 Account 2.............. (%s) %s",m_i.F103Len,m_i.F103Account2);
        break;
    case 104 :
        sprintf(text,"104 Description............ (%s) %s",m_i.F104Len,m_i.F104Description);
        break;
    case 105 :
        sprintf(text,"%d Unsupported Field",105);
        break;
    case 106 :
        sprintf(text,"%d Unsupported Field",106);
        break;
    case 107 :
        sprintf(text,"%d Unsupported Field",107);
        break;
    case 108 :
        sprintf(text,"%d Unsupported Field",108);
        break;
    case 109 :
        sprintf(text,"%d Unsupported Field",109);
        break;
    case 110 :
        sprintf(text,"%d Unsupported Field",110);
        break;
    case 111 :
        sprintf(text,"%d Unsupported Field",111);
        break;
    case 112 :
        sprintf(text,"%d Unsupported Field",112);
        break;
    case 113 :
        sprintf(text,"%d Unsupported Field",113);
        break;
    case 114 :
        sprintf(text,"%d Unsupported Field",114);
        break;
    case 115 :
        sprintf(text,"%d Unsupported Field",115);
        break;
    case 116 :
        sprintf(text,"%d Unsupported Field",116);
        break;
    case 117 :
        sprintf(text,"117 Card Status Code....... %s",m_i.F117CardStatusUpdCode);
        break;
    case 118 :
        sprintf(text,"118 Cash Total Number...... %s",m_i.F118CashTotalNum);
        break;
    case 119 :
        sprintf(text,"119 Cash Total Amount...... %s",m_i.F119CashTotalAmt);
        break;
    case 120 :
        sprintf(text,"120 Record Data............ (%s) %.100s",m_i.F120Len,m_i.F120Private);
        break;
    case 121 :
        sprintf(text,"%d Unsupported Field",121);
        break;
    case 122 :
        sprintf(text,"%d Unsupported Field",122);
        break;
    case 123 :
        sprintf(text,"%d Unsupported Field",123);
        break;
    case 124 :
        sprintf(text,"%d Unsupported Field",124);
        break;
    case 125 :
        sprintf(text,"%d Unsupported Field",125);
        break;
    case 126 :
        sprintf(text,"%d Unsupported Field",126);
        break;
    case 127 :
        sprintf(text,"%d Unsupported Field",127);
        break;
    case 128 :
        sprintf(text,"128 MAC.................... %.02X%.02X%.02X%.02X%.02X%.02X%.02X%.02X",
                        m_i.F128Mac[0],m_i.F128Mac[1],m_i.F128Mac[2],m_i.F128Mac[3],
                        m_i.F128Mac[4],m_i.F128Mac[5],m_i.F128Mac[6],m_i.F128Mac[7]);
        break;
    }
}



void CAS2805Msg::UnpackMessage(BYTE *szSourceBuffer, int nBufLen)
{
    BYTE *p;
    char text[200];
    int  a;

    m_bIsValidISOMsg = false;
    m_bBitmapsValid = false;

    // since the message is valid, make a local copy of the buffer
    memset(m_i.BitMap,0,sizeof(m_i.BitMap));

    p = szSourceBuffer;

    p = UnpackNumericRight    (p, m_i.MsgType, 4, NULL, sizeof(m_i.MsgType), 0);

    memcpy(m_i.BitMap,p,8);
    p += 8;

    if (IsFieldPresent(1))
        {
        memcpy(m_i.BitMap+8, p, 8);
        p += 8;
        }

    if (IsFieldPresent(2))
        p = UnpackNumericLeft  (p, m_i.F2Pan, 0, m_i.F2Len, sizeof(m_i.F2Pan), 2);

    if (IsFieldPresent(3))
        p = UnpackNumericRight (p, m_i.F3ProcessingCode, 6, NULL, sizeof(m_i.F3ProcessingCode), 3);

    if (IsFieldPresent(4))
        p = UnpackNumericRight (p, m_i.F4Amount, 12, NULL, sizeof(m_i.F4Amount), 4);

    if (IsFieldPresent(5))
        p = UnpackNumericRight (p, m_i.F5AmountSettlement, 12, NULL, sizeof(m_i.F5AmountSettlement), 5);

    if (IsFieldPresent(6))
        p = UnpackNumericRight (p, m_i.F6AmountCardholder, 12, NULL, sizeof(m_i.F6AmountCardholder), 6);

    if (IsFieldPresent(7))
        p = UnpackNumericRight (p, m_i.F7DateTime, 10, NULL, sizeof(m_i.F7DateTime), 7);

    if (IsFieldPresent(8))
        p = UnpackNumericRight (p, m_i.F8AmountCardBillFee, 8, NULL, sizeof(m_i.F8AmountCardBillFee), 8);

    if (IsFieldPresent(9))
        p = UnpackNumericRight (p, m_i.F9ConvRateSettle, 8, NULL, sizeof(m_i.F9ConvRateSettle), 9);

    if (IsFieldPresent(10))
        p = UnpackNumericRight (p, m_i.F10ConversionRate, 8, NULL, sizeof(m_i.F10ConversionRate), 10);

    if (IsFieldPresent(11))
        p = UnpackNumericRight (p, m_i.F11Stan, 6, NULL, sizeof(m_i.F11Stan), 11);

    if (IsFieldPresent(12))
        p = UnpackNumericRight (p, m_i.F12TimeLocalTrans, 6, NULL, sizeof(m_i.F12TimeLocalTrans), 12);

    if (IsFieldPresent(13))
        p = UnpackNumericRight (p, m_i.F13DateLocalTrans, 4, NULL, sizeof(m_i.F13DateLocalTrans), 13);

    if (IsFieldPresent(14))
        p = UnpackNumericRight (p, m_i.F14ExpiryDate, 4, NULL, sizeof(m_i.F14ExpiryDate), 14);

    if (IsFieldPresent(15))
        p = UnpackNumericRight (p, m_i.F15DateSettlement, 4, NULL, sizeof(m_i.F15DateSettlement), 15);

    if (IsFieldPresent(16))
        p = UnpackNumericRight (p, m_i.F16DateConversion, 4, NULL, sizeof(m_i.F16DateConversion), 16);

    if (IsFieldPresent(17))
        p = UnpackNumericRight (p, m_i.F17DateCapture, 4, NULL, sizeof(m_i.F17DateCapture), 17);

    if (IsFieldPresent(18))
        p = UnpackNumericRight (p, m_i.F18MerchantType, 4, NULL, sizeof(m_i.F18MerchantType), 18);

    if (IsFieldPresent(19))
        p = UnpackNumericRight (p, m_i.F19AcqCtryCode, 3, NULL, sizeof(m_i.F19AcqCtryCode), 19);

    if (IsFieldPresent(20))
        p = UnpackNumericRight (p, m_i.F20PanCountryCode, 3, NULL, sizeof(m_i.F20PanCountryCode), 20);

    if (IsFieldPresent(21))
        p = UnpackNumericRight (p, m_i.F21FwdCtryCode, 3, NULL, sizeof(m_i.F21FwdCtryCode), 21);

    if (IsFieldPresent(22))
        p = UnpackNumericRight (p, m_i.F22PosEntryMode, 3, NULL, sizeof(m_i.F22PosEntryMode), 22);

    if (IsFieldPresent(23))
        p = UnpackNumericRight (p, m_i.F23CardSeqNumber, 3, NULL, sizeof(m_i.F23CardSeqNumber), 23);

    if (IsFieldPresent(24))
        p = UnpackNumericRight (p, m_i.F24NetworkIdentifier, 3, NULL, sizeof(m_i.F24NetworkIdentifier), 24);

    if (IsFieldPresent(25))
        p = UnpackNumericRight (p, m_i.F25PosCondCode, 2, NULL, sizeof(m_i.F25PosCondCode), 25);

    if (IsFieldPresent(26))
        p = UnpackNumericRight (p, m_i.F26PosCaptureCode, 2, NULL, sizeof(m_i.F26PosCaptureCode), 26);

    if (IsFieldPresent(27))
        p = UnpackUnsupported  (p, 27);

    if (IsFieldPresent(28))
        p = UnpackAmtSign      (p, m_i.F28TransactionFee, 8, sizeof(m_i.F28TransactionFee), 28);

    if (IsFieldPresent(29))
        p = UnpackAmtSign      (p, m_i.F29SettlementFee, 8, sizeof(m_i.F29SettlementFee), 29);

    if (IsFieldPresent(30))
        p = UnpackAmtSign      (p, m_i.F30TranProcFee, 8, sizeof(m_i.F30TranProcFee), 30);

    if (IsFieldPresent(31))
        p = UnpackAmtSign      (p, m_i.F31SettleProcFee, 8, sizeof(m_i.F31SettleProcFee), 31);

    if (IsFieldPresent(32))
        p = UnpackNumericLeft  (p, m_i.F32AcquiringId, 0, m_i.F32Len, sizeof(m_i.F32AcquiringId), 32);

    if (IsFieldPresent(33))
        p = UnpackNumericLeft  (p, m_i.F33ForwardingId, 0, m_i.F33Len, sizeof(m_i.F33ForwardingId), 33);

    if (IsFieldPresent(34))
        p = UnpackNumericLeft  (p, m_i.F34PanExtended, 0, m_i.F34Len, sizeof(m_i.F34PanExtended), 34);

    if (IsFieldPresent(35))
        p = UnpackNumericLeft  (p, m_i.F35Track2, 0, m_i.F35Len, sizeof(m_i.F35Track2), 35);

    if (IsFieldPresent(36))
        p = UnpackNumericLeft  (p, m_i.F36Track3, -1, m_i.F36Len, sizeof(m_i.F36Track3), 36);

    if (IsFieldPresent(37))
        p = UnpackAlpha        (p, m_i.F37RefNumber, 12, NULL, sizeof(m_i.F37RefNumber), 37);

    if (IsFieldPresent(38))
        p = UnpackAlpha        (p, m_i.F38AuthNumber, 6, NULL, sizeof(m_i.F38AuthNumber), 38);

    if (IsFieldPresent(39))
        p = UnpackAlpha        (p, m_i.F39ResponseCode, 2, NULL, sizeof(m_i.F39ResponseCode), 39);

    if (IsFieldPresent(40))
        p = UnpackAlpha        (p, m_i.F40ServiceCode, 3, NULL, sizeof(m_i.F40ServiceCode), 40);

    if (IsFieldPresent(41))
        p = UnpackAlpha        (p, m_i.F41TerminalId, 8, NULL, sizeof(m_i.F41TerminalId), 41);

    if (IsFieldPresent(42))
        p = UnpackAlpha        (p, m_i.F42CardAcceptId, 15, NULL, sizeof(m_i.F42CardAcceptId), 42);

    if (IsFieldPresent(43))
        p = UnpackAlpha        (p, m_i.F43CardAcceptName, 40, NULL, sizeof(m_i.F43CardAcceptName), 43);

    if (IsFieldPresent(44))
        p = UnpackAlpha        (p, m_i.F44AdditionalResp, 0, m_i.F44Len, sizeof(m_i.F44AdditionalResp), 44);

    if (IsFieldPresent(45))
        p = UnpackAlpha        (p, m_i.F45Track1, 0, m_i.F45Len, sizeof(m_i.F45Track1), 45);

    if (IsFieldPresent(46))
        p = UnpackAlpha        (p, m_i.F46AddDataIso, -1, m_i.F46Len, sizeof(m_i.F46AddDataIso), 46);

    if (IsFieldPresent(47))
        p = UnpackAlpha        (p, m_i.F47AddDataNational, -1, m_i.F47Len, sizeof(m_i.F47AddDataNational), 47);

    if (IsFieldPresent(48))
        p = UnpackAlpha        (p, m_i.F48AdditionalData, -1, m_i.F48Len, sizeof(m_i.F48AdditionalData), 48);

    if (IsFieldPresent(49))
        p = UnpackNumericRight (p, m_i.F49CurrencyCode, 3, NULL, sizeof(m_i.F49CurrencyCode), 49);

    if (IsFieldPresent(50))
        p = UnpackNumericRight (p, m_i.F50CurrencyCodeStl, 3, NULL, sizeof(m_i.F50CurrencyCodeStl), 50);

    if (IsFieldPresent(51))
        p = UnpackNumericRight (p, m_i.F51CurrencyCodeChb, 3, NULL, sizeof(m_i.F51CurrencyCodeChb), 51);

    if (IsFieldPresent(52))
        p = UnpackBinary       (p, m_i.F52PinBlock, 8, NULL, sizeof(m_i.F52PinBlock), 52);

    if (IsFieldPresent(53))
        p = UnpackNumericRight (p, m_i.F53SecurityInfo, 16, NULL, sizeof(m_i.F53SecurityInfo), 53);

    if (IsFieldPresent(54))
        p = UnpackAlpha        (p, m_i.F54BalanceInfo, -1, m_i.F54Len, sizeof(m_i.F54BalanceInfo), 54);

    if (IsFieldPresent(55))
        p = UnpackUnsupported  (p, 55);

    if (IsFieldPresent(56))
        p = UnpackUnsupported  (p, 56);

    if (IsFieldPresent(57))
        p = UnpackNumericRight (p, m_i.F57AmountCash, 12, NULL, sizeof(m_i.F57AmountCash), 57);

    if (IsFieldPresent(58))
        p = UnpackNumericRight (p, m_i.F58LedgerBalance, 12, NULL, sizeof(m_i.F58LedgerBalance), 58);

    if (IsFieldPresent(59))
        p = UnpackNumericRight (p, m_i.F59AvailBalance, 12, NULL, sizeof(m_i.F59AvailBalance), 59);

    if (IsFieldPresent(60))
        p = UnpackAlpha         (p, (char *) m_i.F60Private, -1, m_i.F60Len, sizeof(m_i.F60Private), 60);

    if (IsFieldPresent(61))
        p = UnpackAlpha         (p, (char *) m_i.F61Private, -1, m_i.F61Len, sizeof(m_i.F61Private), 61);

    if (IsFieldPresent(62))
        p = UnpackAlpha         (p, (char *) m_i.F62Private, -1, m_i.F62Len, sizeof(m_i.F62Private), 62);

    if (IsFieldPresent(63))
        p = UnpackAlpha         (p, (char *) m_i.F63Private, -1, m_i.F63Len, sizeof(m_i.F63Private), 63);

    if (IsFieldPresent(64))
        p = UnpackBinary       (p, m_i.F64Mac, 8, NULL, sizeof(m_i.F64Mac), 64);

    if (IsFieldPresent(65))
        p = UnpackUnsupported  (p, 65);

    if (IsFieldPresent(66))
        p = UnpackNumericRight (p, m_i.F66SettlementCode, 1, NULL, sizeof(m_i.F66SettlementCode), 66);

    if (IsFieldPresent(67))
        p = UnpackNumericRight (p, m_i.F67ExtendedPayCode, 2, NULL, sizeof(m_i.F67ExtendedPayCode), 67);

    if (IsFieldPresent(68))
        p = UnpackNumericRight (p, m_i.F68ReceiveCtryCode, 3, NULL, sizeof(m_i.F68ReceiveCtryCode), 68);

    if (IsFieldPresent(69))
        p = UnpackNumericRight (p, m_i.F69SettleCtryCode, 3, NULL, sizeof(m_i.F69SettleCtryCode), 69);

    if (IsFieldPresent(70))
        p = UnpackNumericRight (p, m_i.F70NetworkInfoCode, 3, NULL, sizeof(m_i.F70NetworkInfoCode), 70);

    if (IsFieldPresent(71))
        p = UnpackNumericRight (p, m_i.F71MessageNumber, 4, NULL, sizeof(m_i.F71MessageNumber), 71);

    if (IsFieldPresent(72))
        p = UnpackNumericRight (p, m_i.F72MessageNumberLast, 4, NULL, sizeof(m_i.F72MessageNumberLast), 72);

    if (IsFieldPresent(73))
        p = UnpackNumericRight (p, m_i.F73DateAction, 6, NULL, sizeof(m_i.F73DateAction), 73);

    if (IsFieldPresent(74))
        p = UnpackNumericRight (p, m_i.F74CreditsNbr, 10, NULL, sizeof(m_i.F74CreditsNbr), 74);

    if (IsFieldPresent(75))
        p = UnpackNumericRight (p, m_i.F75CreditsRevNbr, 10, NULL, sizeof(m_i.F75CreditsRevNbr), 75);

    if (IsFieldPresent(76))
        p = UnpackNumericRight (p, m_i.F76DebitsNbr, 10, NULL, sizeof(m_i.F76DebitsNbr), 76);

    if (IsFieldPresent(77))
        p = UnpackNumericRight (p, m_i.F77DebitsRevNbr, 10, NULL,
                                                            sizeof(m_i.F77DebitsRevNbr), 77);

    if (IsFieldPresent(78))
        p = UnpackNumericRight (p, m_i.F78TransferNumber, 10, NULL, sizeof(m_i.F78TransferNumber), 78);

    if (IsFieldPresent(79))
        p = UnpackNumericRight (p, m_i.F79TransferReversalNum, 10, NULL, sizeof(m_i.F79TransferReversalNum), 79);

    if (IsFieldPresent(80))
        p = UnpackNumericRight (p, m_i.F80InquiriesNumber, 10, NULL, sizeof(m_i.F80InquiriesNumber), 80);

    if (IsFieldPresent(81))
        p = UnpackNumericRight (p, m_i.F81AuthorisationsNumber, 10, NULL, sizeof(m_i.F81AuthorisationsNumber), 81);

    if (IsFieldPresent(82))
        p = UnpackNumericRight (p, m_i.F82CreditProcFeeAmt, 12, NULL, sizeof(m_i.F82CreditProcFeeAmt), 82);

    if (IsFieldPresent(83))
        p = UnpackNumericRight (p, m_i.F83CreditTranFeeAmt, 12, NULL, sizeof(m_i.F83CreditTranFeeAmt), 83);

    if (IsFieldPresent(84))
        p = UnpackNumericRight (p, m_i.F84DebitProcFeeAmt, 12, NULL, sizeof(m_i.F84DebitProcFeeAmt), 84);

    if (IsFieldPresent(85))
        p = UnpackNumericRight (p, m_i.F85DebitTranFeeAmt, 12, NULL, sizeof(m_i.F85DebitTranFeeAmt), 85);

    if (IsFieldPresent(86))
        p = UnpackNumericRight (p, m_i.F86CreditsAmt, 16, NULL, sizeof(m_i.F86CreditsAmt), 86);

    if (IsFieldPresent(87))
        p = UnpackNumericRight (p, m_i.F87CreditsRevAmt, 16, NULL, sizeof(m_i.F87CreditsRevAmt), 87);

    if (IsFieldPresent(88))
        p = UnpackNumericRight (p, m_i.F88DebitsAmt, 16, NULL, sizeof(m_i.F88DebitsAmt), 88);

    if (IsFieldPresent(89))
        p = UnpackNumericRight (p, m_i.F89DebitsRevAmt, 16, NULL, sizeof(m_i.F89DebitsRevAmt), 89);

    if (IsFieldPresent(90))
        p = UnpackNumericRight (p, m_i.F90OriginalData, 42, NULL, sizeof(m_i.F90OriginalData), 90);

    if (IsFieldPresent(91))
        p = UnpackAlpha        (p, m_i.F91FileUpdateCode, 1, NULL, sizeof(m_i.F91FileUpdateCode), 91);

    if (IsFieldPresent(92))
        p = UnpackAlpha        (p, m_i.F92FileSecurityCode, 2, NULL, sizeof(m_i.F92FileSecurityCode), 92);

    if (IsFieldPresent(93))
        p = UnpackAlpha        (p, m_i.F93ResponseId, 5, NULL, sizeof(m_i.F93ResponseId), 93);

    if (IsFieldPresent(94))
        p = UnpackAlpha        (p, m_i.F94ServiceIndicator, 7, NULL, sizeof(m_i.F94ServiceIndicator), 94);

    if (IsFieldPresent(95))
        p = UnpackAlpha        (p, m_i.F95ReplaceAmounts, 42, NULL, sizeof(m_i.F95ReplaceAmounts), 95);

    if (IsFieldPresent(96))
        p = UnpackBinary       (p, m_i.F96SecurityCode, 8, NULL, sizeof(m_i.F96SecurityCode), 96);

    if (IsFieldPresent(97))
        p = UnpackAmtSign      (p, m_i.F97AmtNetSettle, 16, sizeof(m_i.F97AmtNetSettle), 97);

    if (IsFieldPresent(98))
        p = UnpackAlpha        (p, m_i.F98Payee, 0, m_i.F98Len, sizeof(m_i.F98Payee), 98);

    if (IsFieldPresent(99))
        p = UnpackNumericLeft  (p, m_i.F99SettlementId, 0, m_i.F99Len, sizeof(m_i.F99SettlementId), 99);

    if (IsFieldPresent(100))
        p = UnpackNumericLeft  (p, m_i.F100ReceivingId, 0, m_i.F100Len, sizeof(m_i.F100ReceivingId), 100);

    if (IsFieldPresent(101))
        p = UnpackAlpha        (p, m_i.F101FileName, 0, m_i.F101Len, sizeof(m_i.F101FileName), 101);

    if (IsFieldPresent(102))
        p = UnpackAlpha        (p, m_i.F102Account1, 0, m_i.F102Len, sizeof(m_i.F102Account1), 102);

    if (IsFieldPresent(103))
        p = UnpackAlpha        (p, m_i.F103Account2, 0, m_i.F103Len, sizeof(m_i.F103Account2), 103);

    if (IsFieldPresent(104))
        p = UnpackAlpha        (p, m_i.F104Description, 0, m_i.F104Len, sizeof(m_i.F104Description), 104);

    if (IsFieldPresent(105))
        p = UnpackUnsupported  (p, 105);

    if (IsFieldPresent(106))
        p = UnpackUnsupported  (p, 106);

    if (IsFieldPresent(107))
        p = UnpackUnsupported  (p, 107);

    if (IsFieldPresent(108))
        p = UnpackUnsupported  (p, 108);

    if (IsFieldPresent(109))
        p = UnpackUnsupported  (p, 109);

    if (IsFieldPresent(110))
        p = UnpackUnsupported  (p, 110);

    if (IsFieldPresent(111))
        p = UnpackUnsupported  (p, 111);

    if (IsFieldPresent(112))
        p = UnpackUnsupported  (p, 112);

    if (IsFieldPresent(113))
        p = UnpackUnsupported  (p, 113);

    if (IsFieldPresent(114))
        p = UnpackUnsupported  (p, 114);

    if (IsFieldPresent(115))
        p = UnpackUnsupported  (p, 115);

    if (IsFieldPresent(116))
        p = UnpackUnsupported  (p, 116);

    if (IsFieldPresent(117))
        p = UnpackAlpha        (p, m_i.F117CardStatusUpdCode, 2, NULL, sizeof(m_i.F117CardStatusUpdCode), 117);

    if (IsFieldPresent(118))
        p = UnpackNumericRight (p, m_i.F118CashTotalNum, 10, NULL, sizeof(m_i.F118CashTotalNum), 118);

    if (IsFieldPresent(119))
        p = UnpackNumericRight (p, m_i.F119CashTotalAmt, 16, NULL, sizeof(m_i.F119CashTotalAmt), 119);

    if (IsFieldPresent(120))
        p = UnpackAlpha        (p, (char *) m_i.F120Private, -1, m_i.F120Len, sizeof(m_i.F120Private), 120);

    if (IsFieldPresent(121))
        p = UnpackUnsupported  (p, 121);

    if (IsFieldPresent(122))
        p = UnpackUnsupported  (p, 122);

    if (IsFieldPresent(123))
        p = UnpackUnsupported  (p, 123);

    if (IsFieldPresent(124))
        p = UnpackUnsupported  (p, 124);

    if (IsFieldPresent(125))
        p = UnpackUnsupported  (p, 125);

    if (IsFieldPresent(126))
        p = UnpackUnsupported  (p, 126);

    if (IsFieldPresent(127))
    p = UnpackUnsupported    (p, 127);

    if (IsFieldPresent(128))
        p = UnpackBinary       (p, m_i.F128Mac, 8, NULL, sizeof(m_i.F128Mac), 128);

    sprintf(_logbuf,"Incoming .................. %s",m_i.MsgType);
    WriteLogMessage(_logbuf);

    if (IsFieldPresent(1))
        {
        sprintf(_logbuf,"Bit Map. .................. %.02X%.02X%.02X%.02X%.02X%.02X%.02X%.02X%.02X%.02X%.02X%.02X%.02X%.02X%.02X%.02X",
            m_i.BitMap[0], m_i.BitMap[1], m_i.BitMap[2], m_i.BitMap[3],
            m_i.BitMap[4], m_i.BitMap[5], m_i.BitMap[6], m_i.BitMap[7],
            m_i.BitMap[8], m_i.BitMap[9], m_i.BitMap[10],m_i.BitMap[11],
            m_i.BitMap[12],m_i.BitMap[13],m_i.BitMap[14],m_i.BitMap[15]);
        WriteLogMessage(_logbuf);
        }
    else
        {
        sprintf(_logbuf,"Bit Map. .................. %.02X%.02X%.02X%.02X%.02X%.02X%.02X%.02X",
            m_i.BitMap[0], m_i.BitMap[1], m_i.BitMap[2], m_i.BitMap[3],
            m_i.BitMap[4], m_i.BitMap[5], m_i.BitMap[6], m_i.BitMap[7]);
        WriteLogMessage(_logbuf);
        }

    for (a = 2; a < 129; a++)
        {
        if (IsFieldPresent(a))
            {
            LogSwitchMsg (a, text);
            sprintf(_logbuf,"%s",text);
            WriteLogMessage(_logbuf);
            }
        }

    if (p == NULL)
        {
        strcpy(_logbuf,"Error Unpacking Message");
        WriteLogMessage(_logbuf);
        strcpy(_logbuf,m_UnpackError);
        WriteLogMessage(_logbuf);

        m_bBitmapsValid = false;
        m_bIsValidISOMsg = false;
        }
    else
        {
        if (ValidateBitmap())
            {
            m_bBitmapsValid = false;
            m_bIsValidISOMsg = false;
            }
        else
            {
            m_bBitmapsValid = true;  //we have a good message!!! :)
            m_bIsValidISOMsg = true;
            }
        }

    return;

}


BYTE *CAS2805Msg::UnpackNumericLeft(BYTE *p, char *field, int len, char *field_len, int max_len, int field_no)
{
char w1[5];
int a, length;

    memset(field,0,max_len);

    if (p == NULL)
        return NULL;

    length = len;

    if (length == 0)
        {
        sprintf(w1,"%.02X",p[0]);
        if (LenNotNum(w1,2))
            {
            sprintf(m_UnpackError,"Bit %d Length Attribute Not Numeric %.02X%.02X",field_no,w1[0],w1[1]);
            return NULL;
            }
        length = atoi(w1);
        sprintf(field_len,"%.02d",length);
        p++;
        }

    if (length == -1)
        {
        sprintf(w1,"%.02X%.02X",p[0],p[1]);
        if (LenNotNum(w1,4))
            {
            sprintf(m_UnpackError,"Bit %d Length Attribute Not Numeric %.02X%.02X",field_no,w1[0],w1[1]);
            return NULL;
            }
        length = atoi(w1);
        sprintf(field_len,"%.03d",length);
        p++;
        p++;
        }

    if (length > max_len)
        {
        sprintf(m_UnpackError,"Bit %d Length Error. Max=%d Actual=%d",field_no,max_len,length);
        return NULL;
        }

    for (a = 0; a < length; a += 2)
        {
        sprintf(w1,"%.02X",p[0]);
        p++;
        *field++ = w1[0];
        *field++ = w1[1];
        }

    if (length & 0x01)          // Length odd
        {
        field--;
        *field = 0;
        }

    return (p);

}

// Unpack a fixed length numeric BCD data. Length may be odd or even
// If odd length then skip the left half of the first byte

BYTE *CAS2805Msg::UnpackNumericRight(BYTE *p, char *field, int len, char *field_len, int max_len, int field_no)
{
char w1[5];
int  a, length;

    memset(field,0,max_len);
    if (p == NULL)
        return NULL;

    length = len;

    if (length == 0)
        {
        sprintf(w1,"%.02X",p[0]);
        if (LenNotNum(w1,2))
            {
            sprintf(m_UnpackError,"Bit %d Length Attribute Not Numeric %.02X%.02X",field_no,w1[0],w1[1]);
            return NULL;
            }
        length = atoi(w1);
        sprintf(field_len,"%.02d",length);
        p++;
        }

    if (length == -1)
        {
        sprintf(w1,"%.02X%.02X",p[0],p[1]);
        if (LenNotNum(w1,4))
            {
            sprintf(m_UnpackError,"Bit %d Length Attribute Not Numeric %.02X%.02X",field_no,w1[0],w1[1]);
            return NULL;
            }
        length = atoi(w1);
        sprintf(field_len,"%.03d",length);
        p++;
        p++;
        }

    if (length > max_len)
        {
        sprintf(m_UnpackError,"Bit %d Length Error. Max=%d Actual=%d",field_no,max_len,length);
        return NULL;
        }

    for (a = 0; a < length; a += 2)
        {
        sprintf(w1,"%.02X",p[0]);
        p++;
        if (a == 0 && (length & 0x01))        // First time through and length odd
            *field++ = w1[1];
        else
            {
            *field++ = w1[0];
            *field++ = w1[1];
            }
        }

    return (p);

}

// Unpack an amount field. This has an ASCII sign in the first byte

BYTE *CAS2805Msg::UnpackAmtSign(BYTE *p, char *field, int len, int max_len, int field_no)
{
char w1[3];
int  a;

    memset(field,0,max_len);
    if (p == NULL)
        return NULL;

    if (len > max_len)
        {
        sprintf(m_UnpackError,"Bit %d Length Error. Max=%d Actual=%d",field_no,max_len,len);
        return NULL;
        }

    *field = *p++;

    if (*field == 0)
        *field = 'C';

    field++;

    for (a = 0; a < len; a += 2)
        {
        sprintf(w1,"%.02X",p[0]);
        p++;
        *field++ = w1[0];
        *field++ = w1[1];
        }

    return (p);

}

// Unpack a character field for AS2805. If the length of the field is 0
// length is taken from first 2 bytes of field (LLVAR). If the length is -1
// then the length is first 3 bytes of field (LLLVAR).
// Convert from EBCDIC if required

BYTE *CAS2805Msg::UnpackAlpha (BYTE *p, char *field, int len, char *field_len, int max_len, int field_no)
{
int a, length;

    memset(field,0,max_len);

    if (p == NULL)
        return NULL;

    length = len;

    if (length == 0)
        {
        field_len[0] = *p++;
        field_len[1] = *p++;

        field_len[2] = 0;
        if (LenNotNum(field_len,2))
            {
            sprintf(m_UnpackError,"Bit %d Length Attribute Not Numeric %.02X%.02X",field_no,field_len[0],field_len[1]);
            return NULL;
            }
        length = atoi(field_len);
        }

    if (length == -1)
        {
        field_len[0] = *p++;
        field_len[1] = *p++;
        field_len[2] = *p++;

        field_len[3] = 0;
        if (LenNotNum(field_len,3))
            {
            sprintf(m_UnpackError,"Bit %d Length Attribute Not Numeric %.02X%.02X%.02X",field_no,field_len[0],field_len[1],field_len[2]);
            return NULL;
            }
        length = atoi(field_len);
        }

    if (length > max_len)
        {
        sprintf(m_UnpackError,"Bit %d Length Error. Max=%d Actual=%d",field_no,max_len,length);
        return NULL;
        }

    for (a = 0; a < length; a++)
        field[a] = *p++;

    return p;

}

// Unpack an unsupported field

BYTE *CAS2805Msg::UnpackUnsupported(BYTE *p, int field_no)
{

    sprintf(m_UnpackError,"Bit %d Unsupported Field In Message",field_no);

    return NULL;

}

// Unpack a binary field

BYTE *CAS2805Msg::UnpackBinary(BYTE *p, BYTE *field, int len, char *field_len, int max_len, int field_no)
{
int length;

    memset(field,0,max_len);
    if (p == NULL)
        return NULL;

    length = len;

    if (length == 0)
        {
        sprintf(field_len,"%.03d",*p++);
        length = atoi(field_len);
        }

    if (length > max_len)
        {
        sprintf(m_UnpackError,"Bit %d Length Error. Max=%d Actual=%d",field_no,max_len,length);
        return NULL;
        }

    memcpy(field, p, length);

    p += length;

    return p;

}


int CAS2805Msg::ValidateBitmap()
{
const char *test = NULL;
BYTE required[16];
BYTE bits[] = {0x80,0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
int  a, b, c, test1, test2, rc;

        {
        switch (atoi(m_i.MsgType))
            {
            case 100:
            case 101:
                test = POS_0100_MANDATORY_BITS;
                break;

            case 120:
                test = POS_0120_MANDATORY_BITS;
                break;

            case 121:
                test = POS_0121_MANDATORY_BITS;
                break;

            case 200:
            case 201:
                test = POS_0200_MANDATORY_BITS;
                break;

            case 220:
                test = POS_0220_MANDATORY_BITS;
                break;

            case 221:
                test = POS_0221_MANDATORY_BITS;
                break;

            case 420:
                test = POS_0420_MANDATORY_BITS;
                break;

            case 421:
                test = POS_0421_MANDATORY_BITS;
                break;

            case 800:
                test = NET_0800_MANDATORY_BITS;
                break;

            default:
                break;
            }
        }

    if (test == NULL)
        return 1;

    PackNumericRight(required, (char *) test, 32, NULL);

    rc = 0;
    for (c = 0; c < 128; c++)
        {
        a = (c / 8);
        b = (c % 8);

        if (m_i.BitMap[a] & bits[b])
            test1 = 1;
        else
            test1 = 0;

        if (required[a] & bits[b])
            test2 = 1;
        else
            test2 = 0;

        if (test2 == 1 && test1 == 0)
            {
            sprintf(_logbuf,"Mandatory Field %d Missing In Message %s\n",c + 1, m_i.MsgType);
            WriteLogMessage(_logbuf);
            rc = 1;
            }
        }

    return rc;

}

int CAS2805Msg::LenNotNum(char *len, int digits)
{
int a;

    for (a = 0; a < digits; a++)
        {
        if (len[0] < '0' ||
                len[0] > '9')
            break;
        }

 if (a == digits)
    return 0;
 else
    return 1;

}
