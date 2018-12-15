// AS2805Msg.cpp
//
// This file contains the implementation of the CAS2805Msg
// class, which is used to extract data from a AS2805 ISO financial
// transaction message.

//#include "Utilities/POS_AcquirersTable.h"
//#include "Utilities/BIN_Table.h"

#include "Msg.h"
//#include "AS2805Globals.h"

//extern timespec _gLastEcho;
//extern unsigned int _gFailedConnectionsCounter;

// Main AS2805 Message Parser

const char CAS2805Msg::sDomesticCountryCode[] = "AU";
const char CAS2805Msg::sDomesticCountryCodeLong[] = "AUS";
const char CAS2805Msg::sInternationalCountryCode[] = "IN";

const char CAS2805Msg::CUSCAL_INSTITUTION_ID[]   = "461958";
const char CAS2805Msg::sManagementCodeSignOff[]    = "002";

CAS2805Msg::CAS2805Msg()
{
    CommonInit();
}

CAS2805Msg::CAS2805Msg(BYTE *szSourceBuffer, int nBufLen)
{
    CommonInit();
    UnpackMessage(szSourceBuffer, nBufLen);

    const char* sCountry = isDomestic();
    strncpy(m_sCountry, sCountry, sizeof(m_sCountry) - 1);
    m_sCountry[sizeof(m_sCountry) - 1] = 0;

    // This logic is overloaded in the derived classes
    if (strcmp( sDomesticCountryCode, m_sCountry ) == 0)
        m_bIsDomestic = true;
    else
        m_bIsDomestic = false;
}

// -------------------------------------------------------------------
// CAS2805Msg:: Copy Constructor for CAS2805Msg message type
// -------------------------------------------------------------------
CAS2805Msg::CAS2805Msg(const CAS2805Msg& other)
{
    if (this == &other)
        return;

    memcpy(&m_i, &other.m_i, sizeof(m_i));

    m_bIsValidISOMsg = other.m_bIsValidISOMsg;
    m_bBitmapsValid = other.m_bBitmapsValid;
    m_bReturnMessage = other.m_bReturnMessage;
    m_bBadProcCode = other.m_bBadProcCode;
    m_bIsDomestic = other.m_bIsDomestic;

    memcpy(m_sCountry, other.m_sCountry, sizeof(m_sCountry));
    memcpy(m_UnpackError, other.m_UnpackError, sizeof(m_UnpackError));
    memcpy(m_ReplyMessage, other.m_ReplyMessage, sizeof(m_ReplyMessage));
    m_ReplyLen = other.m_ReplyLen;
    memcpy(m_RequestMessage, other.m_RequestMessage, sizeof(m_RequestMessage));
    m_RequestLen = other.m_RequestLen;
    memcpy(m_message, other.m_message, sizeof(m_message));
    m_message_len = other.m_message_len;
    memcpy(m_sCountry, other.m_sCountry, sizeof(m_sCountry));
}


void CAS2805Msg::CommonInit(void)
{
    memset(&m_i,0,sizeof(m_i));
    m_bIsValidISOMsg = false;
    m_bBitmapsValid = false;
    m_bReturnMessage = true;                  // by default, return the outgoing message.
    m_bBadProcCode = false;
}

CAS2805Msg::~CAS2805Msg()
{

}

// Called by the normal constructor. Unpack the message for processing
//
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
    WriteLogMessage(_logbuf, LOGALL);

    if (IsFieldPresent(1))
        {
        sprintf(_logbuf,"Bit Map. .................. %.02X%.02X%.02X%.02X%.02X%.02X%.02X%.02X%.02X%.02X%.02X%.02X%.02X%.02X%.02X%.02X",
            m_i.BitMap[0], m_i.BitMap[1], m_i.BitMap[2], m_i.BitMap[3],
            m_i.BitMap[4], m_i.BitMap[5], m_i.BitMap[6], m_i.BitMap[7],
            m_i.BitMap[8], m_i.BitMap[9], m_i.BitMap[10],m_i.BitMap[11],
            m_i.BitMap[12],m_i.BitMap[13],m_i.BitMap[14],m_i.BitMap[15]);
        WriteLogMessage(_logbuf, LOGALL);
        }
    else
        {
        sprintf(_logbuf,"Bit Map. .................. %.02X%.02X%.02X%.02X%.02X%.02X%.02X%.02X",
            m_i.BitMap[0], m_i.BitMap[1], m_i.BitMap[2], m_i.BitMap[3],
            m_i.BitMap[4], m_i.BitMap[5], m_i.BitMap[6], m_i.BitMap[7]);
        WriteLogMessage(_logbuf, LOGALL);
        }

    for (a = 2; a < 129; a++)
        {
        if (IsFieldPresent(a))
            {
            LogSwitchMsg (a, text);
            sprintf(_logbuf,"%s",text);
            WriteLogMessage(_logbuf, LOGALL);
            }
        }

    if (p == NULL)
        {
        strcpy(_logbuf,"Error Unpacking Message");
        WriteLogMessage(_logbuf, LOGALL);
        strcpy(_logbuf,m_UnpackError);
        WriteLogMessage(_logbuf, LOGALL);

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
bool CAS2805Msg::SetMsgType(const std::string& val)
{
    if (val.length() != 4) {
        std::string message = "CAS2805Msg::SetMsgType - could not set message type " + val;
        WriteLogMessage( message.c_str(),LOGALL);
        return false;
    }
    strcpy(m_i.MsgType, val.c_str());
    return true;
}

bool CAS2805Msg::SetTransmissionDateTime(const std::string& val)
{
    if (val.length() != 10) {
        std::string message = "CAS2805Msg::SetTransmissionDateTime - could not set transmission date/time "  + val;
         WriteLogMessage( message.c_str(),LOGALL);
        return false;
    }
    strcpy(m_i.F7DateTime, val.c_str());
    SetBitOn(7);
    return true;
}

bool CAS2805Msg::SetSystemTraceAuditNumber(const std::string& val)
{
    if (val.length() != 6) {
        std::string message = "CAS2805Msg::SetSystemTraceAuditNumber - could not set System Trace Audit Number "  + val;
         WriteLogMessage( message.c_str(),LOGALL);
        return false;
    }
    strcpy(m_i.F11Stan, val.c_str());
    SetBitOn(11);
    return true;
}

bool CAS2805Msg::SetNetworkManagementCode(const std::string& code) 
{ 
    if (code.length() != 3) { 
        std::string errormsg; 
        errormsg += "CAS2805Msg::SetNetoworkManagementCode - could not set code ";
        errormsg += code; 
        WriteLogMessage(errormsg.c_str(), LOGALL);
        return false; 
    } 
    sprintf(m_i.F70NetworkInfoCode, "%s", code.c_str()); 
    SetBitOn(70); 
    return true; 
} 


// -------------------------------------------------------------------
// CAS2805Msg::SetForwardingInstitutionID
// -------------------------------------------------------------------
bool CAS2805Msg::SetForwardingInstitutionID(const std::string& institutionId)
{
    if ((institutionId.length() == 0) || (institutionId.length() >= sizeof(m_i.F33ForwardingId)))
    {
        std::string errormsg;
        errormsg += "CAS2805Msg::SetForwardingInstitutionID - could not set institution id ";
        errormsg += institutionId;
        WriteLogMessage(errormsg.c_str(), LOGALL);
        return false;
    }
    sprintf(m_i.F33ForwardingId, "%s", institutionId.c_str());
    sprintf(m_i.F33Len, "%03d", (int)strlen(m_i.F33ForwardingId));
    SetBitOn(33);
    return true;
}

// -------------------------------------------------------------------
// CAS2805Msg::SetReceivingInstitutionID
// -------------------------------------------------------------------
bool CAS2805Msg::SetReceivingInstitutionID(const std::string& institutionId)
{
    if ((institutionId.length() == 0) || (institutionId.length() >= sizeof(m_i.F100ReceivingId)))
    {
        std::string errormsg;
        errormsg += "CAS2805Msg::SetReceivingInstitutionID - could not set institution id ";
        errormsg += institutionId;
        WriteLogMessage(errormsg.c_str(), LOGALL);
        return false;
    }
    sprintf(m_i.F100ReceivingId, "%s", institutionId.c_str());
    sprintf(m_i.F100Len, "%03d", (int)strlen(m_i.F100ReceivingId));
    SetBitOn(100);
    // Ensure that the second bitmap is switched "On"
    SetBitOn(1);
    return true;
}


bool CAS2805Msg::PackMessage() 
    { 
        m_message_len = CAS2805Msg::PackReply(m_message, sizeof(m_message)); 
        if (!m_bIsValidISOMsg) { 
            std::string errormsg; 
            errormsg += "CAS2805Msg::PackMessage - could not pack messsage"; 
            WriteLogMessage(errormsg.c_str(), LOGALL);
            return false; 
        } 
        return m_bIsValidISOMsg; 
    } 

BYTE* CAS2805Msg::Message()
{
    return m_message;
}

int CAS2805Msg::MessageLen()
{
    return m_message_len;
}

int CAS2805Msg::GetPosTermRec(PosTermRec& posTermRec)
{

    memset(&posTermRec, 0, sizeof(posTermRec));
    if (!IsFieldPresent(32) ||
        !IsFieldPresent(41) ||
        !IsFieldPresent(42) ||
        !IsFieldPresent(43))
    {
        return 0;
    }
    if (IsFieldPresent(2)) {
        memcpy(&posTermRec.CardNumber,     &m_i.F2Pan, sizeof(posTermRec.CardNumber)-1);
    }
    if (IsFieldPresent(4)) {
        posTermRec.TransAmtCents = atoi(m_i.F4Amount);
    }
    if (IsFieldPresent(18)) {
        memcpy(&posTermRec.MerchantType,   &m_i.F18MerchantType, sizeof(posTermRec.MerchantType)-1);
    }
    if (IsFieldPresent(19)) {
        memcpy(&posTermRec.AcqCtryCode,    &m_i.F19AcqCtryCode, sizeof(posTermRec.AcqCtryCode)-1);
    }
    if (IsFieldPresent(25)) {
        memcpy(&posTermRec.PosCondCode,    &m_i.F25PosCondCode, sizeof(posTermRec.PosCondCode)-1);
    }
    memcpy(&posTermRec.AcquiringId,    &m_i.F32AcquiringId, sizeof(posTermRec.AcquiringId)-1);
    memcpy(&posTermRec.TerminalId,     &m_i.F41TerminalId, sizeof(posTermRec.TerminalId)-1);
    memcpy(&posTermRec.CardAcceptId,   &m_i.F42CardAcceptId, sizeof(posTermRec.CardAcceptId)-1);
    memcpy(&posTermRec.CardAcceptName, &m_i.F43CardAcceptName, sizeof(posTermRec.CardAcceptName)-1);
    return 1;
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
            WriteLogMessage(_logbuf, LOGALL);
            rc = 1;
            }
        }

    return rc;

}

const BYTE* CAS2805Msg::GetResponseBuf(void)
{
    return m_ReplyMessage;
}

int  CAS2805Msg::GetResponseLen(void)
{
    return m_ReplyLen;
}

void CAS2805Msg::EncodeGenericResponse(void)
{
    // the generic response is used to send back a response
    // to the financial network for data messages that are not used
    // by our software.
//  m_nResponseBufLen = max(m_nDataSrcBufLen,12);
//  m_pszResponseBuffer = new BYTE[ m_nResponseBufLen ];
//  memset(m_pszResponseBuffer,(int) ' ',m_nResponseBufLen);
//  memcpy(m_pszResponseBuffer, m_szDataBuffer, m_nDataSrcBufLen);

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

// Unpack a numeric field. If len = 0 the length is held as packed data
// in the first byte of the field. If len = -1 then length is held in
// the first two bytes of the field

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

// Log each field that is present in the message. Routine is called repeatdly
// And will return 1 field at a time. Returns -1 at the end

void CAS2805Msg::LogSwitchMsg (int field, char *text)
{
    switch (field)
    {
    case   2 :
                // Ticket #142
        sprintf(text,"2 Primary Account Number .. (%s) %s",m_i.F2Len, ObfuscatePAN(m_i.F2Pan).c_str());
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
        sprintf(text,"35 Track 2 Data............ (%s) %s",m_i.F35Len, ObfuscatePAN(m_i.F35Track2).c_str());
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

void CAS2805Msg::HandleBadProcCode()
{
    NpsiString  strLogResult;

    std::string baseAmount = GetBaseAmount('C');
    std::string card = GetCard();
    std::string cardx = ObfuscatePAN(card);

    strLogResult.Format("Bad processing code!  Card No. %s  Amt: %s  Code: %s ", cardx.c_str(), baseAmount.c_str(), m_i.F3ProcessingCode);
    strLogResult += " Result:  Invalid Transaction";
    strcpy(m_i.F39ResponseCode, REPLY_CODE_DO_NOT_HONOUR);

    WriteLogMessage((const char*)strLogResult, LOGALL);

    m_bBadProcCode = true;

}

void CAS2805Msg::LogErrDesc(CTransGeneric* pCTO, int nResult)
{
    if (nResult != TSOK)
    {
        char errbuf[1024];
        // Ticket #142
        // If the card number is invalid, output it, otherwise don't
        if (nResult == TSCardDeactivated)
        {
            sprintf(errbuf,"Error in transaction [%s]: %s",pCTO->GetCardNumber(), pCTO->GetErrDesc());
        }
        else
        {
            sprintf(errbuf,"Error in transaction [%s]: %s", ObfuscatePAN(pCTO->GetCardNumber()).c_str(), pCTO->GetErrDesc());
        }
        WriteLogMessage(errbuf,LOGALL);
    }
}

NpsiString CAS2805Msg::ObtainResult(int nResult)
{
    NpsiString strLogResult;

    switch (nResult)
    {
    case TSOK:
        strcpy(m_i.F39ResponseCode, REPLY_CODE_APPROVED);
        strLogResult = " Result:  Complete  ";
        break;

    case TSCardInvalid:
        strLogResult = " Result:  Invalid Card";
        strcpy(m_i.F39ResponseCode, REPLY_CODE_INVALID_CARD);
        break;

    case TSCardInUse:
        strLogResult = " Result:  Card in use";
        strcpy(m_i.F39ResponseCode, REPLY_CODE_DO_NOT_HONOUR);
        break;

    case TSCardInsufFunds:
        strLogResult = " Result:  Insufficient Funds";
        strcpy(m_i.F39ResponseCode, REPLY_CODE_INSUFFICIENT_FUNDS);
        break;

    case TSCardExpired:
        strLogResult = " Result:  Card Expired";
        strcpy(m_i.F39ResponseCode, REPLY_CODE_INACTIVE_CARD);
        break;

    case TSCardDeactivated:
        strLogResult = " Result:  Card Deactivated";
        strcpy(m_i.F39ResponseCode, REPLY_CODE_DEACTIVATED_CARD);
        break;

    case TSCardReplaced:
        strLogResult = " Result:  Card Replaced";
        strcpy(m_i.F39ResponseCode, REPLY_CODE_REPLACED_CARD);
        break;

    case TSCardInactive:
        strLogResult = " Result:  Card Inactive";
        strcpy(m_i.F39ResponseCode, REPLY_CODE_INACTIVE_CARD);
        break;

    case TSCardPreActive:
        strLogResult = " Result:  Card PreActive";
        strcpy(m_i.F39ResponseCode, REPLY_CODE_INACTIVE_CARD);
        break;

    case TSSysErr:
        strLogResult = " Result:  System Error ";
        strcpy(m_i.F39ResponseCode, REPLY_CODE_DO_NOT_HONOUR);
        break;

    case TSInvalidTrans:
        strLogResult = " Result:  Invalid Transaction";
        strcpy(m_i.F39ResponseCode, REPLY_CODE_DO_NOT_HONOUR);
        break;

    case TSExceedBalance:
        strLogResult = " Result: Exceeded Balance";
        strcpy(m_i.F39ResponseCode, REPLY_CODE_CALL_ISSUER);
        break;

    case TSExceedDailyCredit:
        strLogResult = " Result: Exceeded Daily Credit Amount";
        strcpy(m_i.F39ResponseCode, REPLY_CODE_CALL_ISSUER);
        break;

    case TSExceedDailyDebit:
        strLogResult = " Result: Exceeded Daily Debit Amount";
        strcpy(m_i.F39ResponseCode, REPLY_CODE_EXCEED_DEBIT_AMOUNT);
        break;

    case TSExceedDailyCreditCount:
        strLogResult = " Result: Exceeded Credit Count";
        strcpy(m_i.F39ResponseCode, REPLY_CODE_CALL_ISSUER);
        break;

    case TSExceedDailyDebitCount:
        strLogResult = " Result: Exceeded Debit Count";
        strcpy(m_i.F39ResponseCode, REPLY_CODE_EXCEED_DEBIT_COUNT);
        break;

    case TSExceedDailyATMDebitCount:
        strLogResult = " Result: Exceeded Daily ATM Debit Count";
        strcpy(m_i.F39ResponseCode, REPLY_CODE_EXCEED_DEBIT_COUNT);
        break;

    case TSExceedATMDebit:
        strLogResult = " Result: Exceeded ATM Debit Amount";
        strcpy(m_i.F39ResponseCode, REPLY_CODE_EXCEED_DEBIT_AMOUNT);
        break;

    case TSExceedPOSDebit:
        strLogResult = " Result: Exceeded POS Debit Amount";
        strcpy(m_i.F39ResponseCode, REPLY_CODE_EXCEED_DEBIT_AMOUNT);
        break;

    case TSExceedTransferTo:
        strLogResult = " Result: Exceeded Credit Transfer";
        strcpy(m_i.F39ResponseCode, REPLY_CODE_CALL_ISSUER);
        break;

    case TSCashNotAllowed:
        strLogResult = " Result: Cash Not Allowed";
        strcpy(m_i.F39ResponseCode, REPLY_CODE_TRAN_NOT_PERM_CARDHOLDER);
        break;

    case TSNotApprovedTerminal:
        strLogResult = " Result: Not Approved Terminal";
        strcpy(m_i.F39ResponseCode, REPLY_CODE_TRAN_NOT_PERM_TERMINAL);
        break;

    case TSBarterAuthorisation:
        strLogResult = " Result: Barter approval";
        strcpy(m_i.F39ResponseCode, REPLY_CODE_DO_NOT_HONOUR);
        break;

    case TSLostStolenCard:
    case TSFraudulentCard:
        strLogResult = " Result: Hard Capture";
        strcpy(m_i.F39ResponseCode, REPLY_CODE_HARD_CAPTURE);
        break;

    case TSReverseFail:
        strLogResult = " Result: Reversal failed";
        strcpy(m_i.F39ResponseCode, REPLY_CODE_APPROVED);
        break;

    case TSExceedDailyATMDebit:
        strLogResult = " Result: Exceeded Daily ATM Debit Amount";
        strcpy(m_i.F39ResponseCode, REPLY_CODE_EXCEED_DEBIT_AMOUNT);
        break;

    case TSExceedDailyPOSDebit:
        strLogResult = " Result: Exceeded Daily POS Debit Amount";
        strcpy(m_i.F39ResponseCode, REPLY_CODE_EXCEED_DEBIT_AMOUNT);
        break;

    default:
        strLogResult.Format(" Result:  Unknown Error %d", nResult);
        strcpy(m_i.F39ResponseCode, REPLY_CODE_DO_NOT_HONOUR);
        break;
    }

    return strLogResult;

}

// -------------------------------------------------------------------
//this function fills in the reference field sent to the FiServetrans object which puts this
//string into the vcReference field of TRANSACTIONCACHE, TRANSACTIONHISTORY, and AUTHORIZATIONS
std::string CAS2805Msg::GetReferenceString()
{
    NpsiString strTmp;
    std::string refStr = "";
    if (strlen(m_i.F41TerminalId) > 0)
    {
        strTmp = m_i.F41TerminalId;
        strTmp.TrimRight();
        strTmp += " ";
        refStr = (const char*)strTmp;
    }

    if (strlen(m_i.F43CardAcceptName) > 0)
    {
        strTmp = m_i.F43CardAcceptName;
        strTmp.TrimRight();
        refStr += (const char*)strTmp;
    }

    int len = refStr.size();
    for (int i=0; i<len; ++i) {
        if (refStr[i] == ',') {
            refStr[i] = ' ';
        }
    }
    return refStr;
}

// -------------------------------------------------------------------
std::string CAS2805Msg::GetExternalFee()
{
    int         nFeeCents = 0;
    double      dExternalFee;
    NpsiString  tmp;
    if (strlen(m_i.F28TransactionFee) > 0)
    {
        nFeeCents = atoi(m_i.F28TransactionFee+1);

        dExternalFee =((double)nFeeCents) / 100.0;

        if (m_i.F28TransactionFee[0] == 'D')
            dExternalFee *= -1.0;  // if fee is a debit

        tmp.Format("%8.2f", dExternalFee); // External fee in dollars
        return (const char*)tmp;
    }
    return "";
}

// -------------------------------------------------------------------
std::string CAS2805Msg::GetCard()
{
    if (strlen(m_i.F2Pan) == 0) {
        int a;
        for (a = 0; a < (int)sizeof(m_i.F2Pan)-1; a++) {
            if (m_i.F35Track2[a] < '0' || m_i.F35Track2[a] > '9') {
                break;
            }
            else {
                m_i.F2Pan[a] = m_i.F35Track2[a];
            }
        }
        m_i.F2Pan[a] = '\0';
    }

    return m_i.F2Pan;
}

// -------------------------------------------------------------------
std::string CAS2805Msg::GetBaseAmount(char debitCredit)
{
    int nTransAmtCents = atoi(m_i.F4Amount);
    double dBaseAmount =((double)nTransAmtCents) / 100.0;
    NpsiString strBaseAmount;
    if (debitCredit == 'D') {
        dBaseAmount *= -1.0;
    }
    strBaseAmount.Format("%8.2f", dBaseAmount); // Base amount in dollars.
    return (const char*)strBaseAmount;
}

// -------------------------------------------------------------------
// Check the Card Acquirer ID and Card Acceptor Name to determine if the
// transaction is domestic.
// return The 2-letter country code. e.g. "AU" for domestic (Australia).
// -------------------------------------------------------------------
const char *CAS2805Msg::isDomestic()
{
    char errbuf[256];

    // Check using POS Acquirers table to see if Aust Acquirer
    if (POS::AcquirersTable::Instance()->IsDomestic(
                                                    atoll((const char*)m_i.F32AcquiringId)))
    {
        // check country code in Card Acceptor Name to raise flags for errors.
        if (strcmp(m_i.F43CardAcceptName + 38, sDomesticCountryCode) == 0 ||
            strcmp(m_i.F43CardAcceptName + 37, sDomesticCountryCodeLong) == 0)
        {
            // This is what we would expect
            // So don't raise any flags
        }
        else
        {
            // We have an accepted aus acquirer but no AU country code.
            // This is the initial problem and should raise lots of flags.
            // We'll flag it anyway.
            sprintf(errbuf,"Intnl query: Acq32 %s CAN43 %s", m_i.F32AcquiringId, m_i.F43CardAcceptName);
            WriteLogMessage(errbuf,LOGALL);
        }

        sprintf(errbuf,"Flagging transaction as AU");
        WriteLogMessage(errbuf,LOGALL);
        return(sDomesticCountryCode);
    }

    // Also raise a flag if we have an "AU" at the end and we
    // get to here. Ideally these should all be caught, the
    // acquirer IDs added to the AUS list, and the catch-all
    // below removed.
    if (strcmp(m_i.F43CardAcceptName + 38, sDomesticCountryCode) == 0 ||
        strcmp(m_i.F43CardAcceptName + 37, sDomesticCountryCodeLong) == 0)
    {
        sprintf(errbuf,"Intnl unknown AU acquirer: Acq32 %s CAN43 %s", m_i.F32AcquiringId, m_i.F43CardAcceptName);
        WriteLogMessage(errbuf,LOGALL);
    }

    // Currently we are returning the last two letters of
    // the Card Acceptor Name, which SHOULD be the 2-letter
    // country code ("AU"). We need to use the 3-digit
    // Acquiring Institution field instead. If this
    // field is 036 (?), then we again return "AU" hard.
    // This isn't used, as we never GET the country code
    // if (strcmp(m_i.F19AcqCtryCode,"036") == 0)
    //  return ("AU");
    if ((strlen(m_i.F43CardAcceptName) < 40) || (m_i.F43CardAcceptName[38] == ' ') || (m_i.F43CardAcceptName[39] == ' '))
    {
        const char* result;
        if (IsFieldPresent(19) && strcmp(m_i.F19AcqCtryCode,"036") != 0 )
        {
            // International acquirer
            result = sInternationalCountryCode;
            sprintf(errbuf,"Acquirer country is present in the message: Flagging transaction as IN");
        }
        else if (IsFieldPresent(49) && strcmp(m_i.F49CurrencyCode,"036") != 0 )
        {
            // Non-AUD transaction currency
            result = sInternationalCountryCode;
            sprintf(errbuf,"Transaction currency is present in the message: Flagging transaction as IN");
        }
        else if (IsFieldPresent(6))
        {
            // Billing amount different from transaction amount
            result = sInternationalCountryCode;
            sprintf(errbuf,"Billing amount is present in the message: Flagging transaction as IN");
        }
        else
        {
            // No traces of transaction country or currency => domestic
            result = sDomesticCountryCode;
            sprintf(errbuf,"No country or currency data present in the message: Flagging transaction as AU");
        }
        WriteLogMessage(errbuf,LOGALL);
        return(result);
    }
    sprintf(errbuf,"Flagging transaction as %s", m_i.F43CardAcceptName + 38);
    WriteLogMessage(errbuf,LOGALL);

    // And leave this as a fail-safe.
    return (m_i.F43CardAcceptName + 38);
}


// -------------------------------------------------------------------
std::string CAS2805Msg::GetCAID()
{
    NpsiString strTmp;
    std::string ret = "";
    if (strlen(m_i.F42CardAcceptId) > 0)
    {
        strTmp = m_i.F42CardAcceptId;
        strTmp.TrimRight();
        ret = (const char*)strTmp;
    }

    int len = ret.size();
    for (int i=0; i<len; ++i) {
        if (ret[i] == ',') {
            ret[i] = ' ';
        }
    }
    return ret;
}

// -------------------------------------------------------------------
std::string CAS2805Msg::GetCATID()
{
    NpsiString strTmp;
    std::string ret = "";
    if (strlen(m_i.F41TerminalId) > 0)
    {
        strTmp = m_i.F41TerminalId;
        strTmp.TrimRight();
        ret = (const char*)strTmp;
    }

    int len = ret.size();
    for (int i=0; i<len; ++i) {
        if (ret[i] == ',') {
            ret[i] = ' ';
        }
    }
    return ret;
}

// -------------------------------------------------------------------
std::string CAS2805Msg::GetTransactionDate()
{
    char buf[20];
    time_t now;
    struct tm *date;
    const char* transDateTime;
    time(&now);
    date = localtime(&now);
    if (strlen(m_i.F90OriginalData) >= 20)
    {
        transDateTime = &(m_i.F90OriginalData[10]);
    }
    else
    {
        transDateTime = (const char*)m_i.F7DateTime;
    }
    int tranMonth = ((transDateTime[0]-'0')*10) + (transDateTime[1]-'0');
    int thisMonth = date->tm_mon + 1;
    int thisYear = date->tm_year+1900;
    int tranYear = 0;

    // This an edge case where a transaction spans midnight New Year eve
    if (thisMonth == 1 && tranMonth == 12) { // Dec last year
        tranYear = thisYear-1;
    }
    else {
        tranYear = thisYear;
    }

    sprintf(buf,"%.04d%.*s",tranYear, (int)sizeof(m_i.F7DateTime) - 1, transDateTime);
    return (const char*)buf;
}

// -------------------------------------------------------------------
std::string CAS2805Msg::GetSettlementDate()
{
    // Date Settlement is in MMDD format. The Settlement Date may be up to two days
    // later than the transaction date. Need to cater for this difference when
    // calculating the YYYY portion of the date for FSV.
    char buf[20];
    time_t now;
    struct tm *date;

    time(&now);
    date = localtime(&now);
    int settlementMonth = ((m_i.F15DateSettlement[0]-'0')*10) + (m_i.F15DateSettlement[1]-'0');
    int thisMonth = date->tm_mon + 1;
    int thisYear = date->tm_year+1900;
    int settlementYear = 0;

    if (thisMonth == 1 && settlementMonth == 12) { // Dec last year
        settlementYear = thisYear-1;
    }
    else if (thisMonth == 12 && settlementMonth == 1) { // Jan next year
        settlementYear = thisYear+1;
    }
    else {
        settlementYear = thisYear;
    }

    sprintf(buf,"%.04d%s",settlementYear, (const char*)m_i.F15DateSettlement);
    return (const char*)buf;
}

// -------------------------------------------------------------------
// CAS2805Msg::GetMsgMTIF000
// -------------------------------------------------------------------
std::string CAS2805Msg::GetMsgMTIF000()
{
    NpsiString strTmp;
    std::string ret = "";
    if (strlen(m_i.MsgType) > 0)
    {
        strTmp = m_i.MsgType;
        strTmp.TrimRight();
        ret = (const char*)strTmp;
    }

    int len = ret.size();
    for (int i=0; i<len; ++i) {
        if (ret[i] == ',') {
            ret[i] = ' ';
        }
    }
    return ret;
}

// -------------------------------------------------------------------
// CAS2805Msg::GetProcCodeF003
// -------------------------------------------------------------------
std::string CAS2805Msg::GetProcCodeF003()
{
    NpsiString strTmp;
    std::string ret = "";
    if (strlen(m_i.F3ProcessingCode) > 0)
    {
        strTmp = m_i.F3ProcessingCode;
        strTmp.TrimRight();
        ret = (const char*)strTmp;
    }

    int len = ret.size();
    for (int i=0; i<len; ++i) {
        if (ret[i] == ',') {
            ret[i] = ' ';
        }
    }
    return ret;
}

// -------------------------------------------------------------------
// CAS2805Msg::GetBillingConvRateF010
// -------------------------------------------------------------------
std::string CAS2805Msg::GetBillingConvRateF010()
{
    NpsiString strTmp;
    std::string ret = "";
    if (strlen(m_i.F10ConversionRate) > 0)
    {
        strTmp = m_i.F10ConversionRate;
        strTmp.TrimRight();
        ret = (const char*)strTmp;
    }

    int len = ret.size();
    for (int i=0; i<len; ++i) {
        if (ret[i] == ',') {
            ret[i] = ' ';
        }
    }
    return ret;
}
// -------------------------------------------------------------------
// CAS2805Msg::GetPOSEntryModeF022
// -------------------------------------------------------------------
std::string CAS2805Msg::GetPOSEntryModeF022()
{
    NpsiString strTmp;
    std::string ret = "";
    if (strlen(m_i.F22PosEntryMode) > 0)
    {
        strTmp = m_i.F22PosEntryMode;
        strTmp.TrimRight();
        ret = (const char*)strTmp;
    }

    int len = ret.size();
    for (int i=0; i<len; ++i) {
        if (ret[i] == ',') {
            ret[i] = ' ';
        }
    }
    return ret;
}

// -------------------------------------------------------------------
// CAS2805Msg::GetPOSCondCodeF025
// -------------------------------------------------------------------
std::string CAS2805Msg::GetPOSCondCodeF025()
{
    NpsiString strTmp;
    std::string ret = "";
    if (strlen(m_i.F25PosCondCode) > 0)
    {
        strTmp = m_i.F25PosCondCode;
        strTmp.TrimRight();
        ret = (const char*)strTmp;
    }

    int len = ret.size();
    for (int i=0; i<len; ++i) {
        if (ret[i] == ',') {
            ret[i] = ' ';
        }
    }
    return ret;
}

// -------------------------------------------------------------------
// CAS2805Msg::GetAcqCurrencyF049
// -------------------------------------------------------------------
std::string CAS2805Msg::GetAcqCurrencyF049()
{
    NpsiString strTmp;
    std::string ret = "";
    if (strlen(m_i.F49CurrencyCode) > 0)
    {
        strTmp = m_i.F49CurrencyCode;
        strTmp.TrimRight();
        ret = (const char*)strTmp;
    }

    int len = ret.size();
    for (int i=0; i<len; ++i) {
        if (ret[i] == ',') {
            ret[i] = ' ';
        }
    }
    return ret;
}

// -------------------------------------------------------------------
// CAS2805Msg::GetCurrencyCodeStlF050
// -------------------------------------------------------------------
std::string CAS2805Msg::GetCurrencyCodeStlF050()
{
    NpsiString strTmp;
    std::string ret = "";
    if (strlen(m_i.F50CurrencyCodeStl) > 0)
    {
        strTmp = m_i.F50CurrencyCodeStl;
        strTmp.TrimRight();
        ret = (const char*)strTmp;
    }

    int len = ret.size();
    for (int i=0; i<len; ++i) {
        if (ret[i] == ',') {
            ret[i] = ' ';
        }
    }
    return ret;
}

// -------------------------------------------------------------------
// CAS2805Msg::GetCurrencyCodeChbF051
// -------------------------------------------------------------------
std::string CAS2805Msg::GetCurrencyCodeChbF051()
{
    NpsiString strTmp;
    std::string ret = "";
    if (strlen(m_i.F51CurrencyCodeChb) > 0)
    {
        strTmp = m_i.F51CurrencyCodeChb;
        strTmp.TrimRight();
        ret = (const char*)strTmp;
    }

    int len = ret.size();
    for (int i=0; i<len; ++i) {
        if (ret[i] == ',') {
            ret[i] = ' ';
        }
    }
    return ret;
}

// -------------------------------------------------------------------
// CAS2805Msg::GetCashAmount
// -------------------------------------------------------------------
std::string CAS2805Msg::GetCashAmount()
{
    int nCashAmtCents = atoi(m_i.F57AmountCash);
    double dCashAmount =((double)nCashAmtCents) / 100.0;
    NpsiString strCashAmount;
    strCashAmount.Format("%8.2f", dCashAmount); // Cash amount in dollars.
    return (const char*)strCashAmount;
}

// -------------------------------------------------------------------
// CAS2805Msg::GetPlasticExpiryDate
// -------------------------------------------------------------------
std::string CAS2805Msg::GetPlasticExpiryDate()
{
    //Plastic Expiry Date is not presented on transaction
    std::string plasticExpiryDate = "";
    if (IsFieldPresent(14))
    {
        plasticExpiryDate = m_i.F14ExpiryDate;
    }
    else if (IsFieldPresent(35) && strlen(m_i.F35Track2) >= 21)
    {
        plasticExpiryDate = m_i.F35Track2;
        plasticExpiryDate = plasticExpiryDate.substr(17, 4);
    }

    return plasticExpiryDate;
}

// -------------------------------------------------------------------
// CAS2805Msg::GetSTANF011
// -------------------------------------------------------------------
std::string CAS2805Msg::GetSTANF011()
{
    std::string stan = m_i.F11Stan;
    if (strlen(m_i.F90OriginalData) >= 10)
    {
        stan = m_i.F90OriginalData;
        stan = stan.substr(4, 6);
    }
    return stan;
}

// -------------------------------------------------------------------
// CAS2805Msg::GetAcquirerId
// -------------------------------------------------------------------
std::string CAS2805Msg::GetAcquirerId()
{
    std::string acquirerId = m_i.F32AcquiringId;
    if (strlen(m_i.F90OriginalData) >= 31)
    {
        acquirerId = m_i.F90OriginalData;
        acquirerId = acquirerId.substr(20, 11);
        acquirerId.erase(acquirerId.find_last_not_of("F")+1);
    }
    return acquirerId;
}


// -------------------------------------------------------------------
// CAS2805Msg::GetOrigMsgMTI
// -------------------------------------------------------------------
std::string CAS2805Msg::GetOrigMsgMTI()
{
    std::string origMTI = GetMsgMTIF000();
    if (strlen(m_i.F90OriginalData) >= 4)
    {
        origMTI = m_i.F90OriginalData;
        origMTI = origMTI.substr(0, 4);
    }
    return origMTI;
}


// -------------------------------------------------------------------
int CAS2805Msg::ProcessMessageData()
{
    int msgType;
    int msgVersion;
    int msgClass;
    int msgFunction;

//    clock_gettime(CLOCK_REALTIME,&_gLastEcho);
//    _gFailedConnectionsCounter = 0;
    // the messages will be processed according to msg type
    switch ((msgType = atoi(m_i.MsgType)))
    {
    case 800:
        strcpy(m_i.F39ResponseCode,REPLY_CODE_APPROVED);
        strcpy(m_i.MsgType,"0810");
        break;

    default:
        // notify an operator that an unsupported message type was sent!
        fprintf(stderr,"\nUnsupported AS2805 message type: %s!\n", m_i.MsgType);
        sprintf(_logbuf, "Unsupported AS2805 message type: %s", m_i.MsgType);
        WriteLogMessage(_logbuf, LOGALL);

        if (msgType < 100)
            return 0;

        // Parse message type to determine correct response type
        msgVersion = msgType / 1000;
        msgClass = (msgType - msgVersion * 1000) / 100;
        msgFunction = (msgType - msgVersion * 1000 - msgClass * 100) / 10;
        // Don't reply anything if it's already a response message
        if (msgFunction % 2)
            return 0;
        // Add 10 to message type to get a proper response type (ignore message origin character)
        msgType = msgVersion * 1000 + msgClass * 100 + (msgFunction + 1) * 10;
        sprintf(m_i.MsgType, "%04d", msgType);
        strcpy(m_i.F39ResponseCode, REPLY_CODE_FORMAT_ERROR);
    }

    SetBitOn(39);
    m_ReplyLen = PackReply(m_ReplyMessage, sizeof(m_ReplyMessage));
    return (m_ReplyLen);
}

// -------------------------------------------------------------------
// CAS2805Msg::IsRediATMBin
// -------------------------------------------------------------------
bool CAS2805Msg::IsRediATMBin(std::string card)
{
    return BIN::Table::Instance()->IsRediATMEnrolled(card.substr(0, 6));
}

// -------------------------------------------------------------------
// CAS2805Msg::IsATMTransaction
// -------------------------------------------------------------------
bool CAS2805Msg::IsATMTransaction()
{
    return (strcmp("6011", m_i.F18MerchantType) == 0);
}

// -------------------------------------------------------------------
// CAS2805Msg::IsCashOutTransaction
// -------------------------------------------------------------------
bool CAS2805Msg::IsCashOutTransaction()
{
    return (atoi(m_i.F57AmountCash) != 0);
}


bool CAS2805Msg::CreateSignOffMessage()
{
    time_t now;
    time(&now);
    tm* local = localtime(&now);
    char tdt[11];
    char tlt[7];

    sprintf(tdt, "%02d%02d%02d%02d%02d",
            local->tm_mon+1,
            local->tm_mday,
            local->tm_hour,
            local->tm_min,
            local->tm_sec);
     sprintf(tlt, "%02d%02d%02d",
            local->tm_hour,
            local->tm_min,
            local->tm_sec);

     if (!SetMsgType("0800"))
         return false;

     // YYMMDDhhmmss
     if (!SetTransmissionDateTime(tdt))
         return false;

     //hhmmss
     if (!SetSystemTraceAuditNumber(tlt))
         return false;

     if (!SetNetworkManagementCode(sManagementCodeSignOff))
         return false;

     if (!SetForwardingInstitutionID(CUSCAL_INSTITUTION_ID))
         return false;

     if (!SetReceivingInstitutionID(CUSCAL_INSTITUTION_ID))
         return false;

     return PackMessage();
}


