
typedef unsigned char BYTE;

// AS2805 Bit Map Mandatory patterns    Bit Numbers below
//                                      00000000000000000000000011111111
//                                      00112223344455666778889900011222
//                                      48260482604826048260482604826048
const char POS_0100_MANDATORY_BITS[] = "323A048188C000000000000000000000";
const char POS_0110_MANDATORY_BITS[] = "322A00018EC000600000000000000000";
const char POS_0120_MANDATORY_BITS[] = "323A04818EC000000000000000000000";
const char POS_0121_MANDATORY_BITS[] = "323A04818EC000000000000000000000";
const char POS_0130_MANDATORY_BITS[] = "323A00018EC000000000000000000000";

const char POS_0200_MANDATORY_BITS[] = "323A048108E000000000000000000000";
const char POS_0210_MANDATORY_BITS[] = "322200010EC000600000000000000000";
const char POS_0220_MANDATORY_BITS[] = "323A048108E000000000000000000000";
const char POS_0221_MANDATORY_BITS[] = "323A048108E000000000000000000000";
const char POS_0230_MANDATORY_BITS[] = "322200010EC000000000000000000000";

const char POS_0420_MANDATORY_BITS[] = "B23A048108E000000000004000000000";
const char POS_0421_MANDATORY_BITS[] = "B23A048108E000000000004000000000";
const char POS_0430_MANDATORY_BITS[] = "322200010EC000000000000000000000";

// Network Management messages
const char NET_0800_MANDATORY_BITS[] = "82200000800000000400000010000000";
const char NET_0810_MANDATORY_BITS[] = "82200000820000000400000010000000";



class CAS2805Msg
{
public:
    CAS2805Msg(const char*);
    bool PackMessage();
    int  PackReply(BYTE *Dest, int nBufLen);
    void UnpackMessage(BYTE *szSourceBuffer, int nBufLen);

private:
    BYTE* PackBinary(BYTE *p, BYTE *field, int len, char *field_len);
    BYTE* PackAlpha(BYTE *p, char *field, int len, char *field_length);
    BYTE* PackNumericRight(BYTE *p, char *field, int len, char *field_len);
    BYTE* PackNumericLeft(BYTE *p, char *field, int len, char *field_len, char pad);
    BYTE* PackAmtSign(BYTE *p, char *field, int len);
    BYTE* PackUnsupported(BYTE *Dest, int field);

    BYTE* UnpackAlpha(BYTE *p, char *field, int len, char *field_len, int max_len, int field_no);
    BYTE* UnpackAmtSign(BYTE *p, char *field, int len, int max_len, int field_no);
    BYTE* UnpackBinary(BYTE *p, BYTE *field, int len, char *field_len, int max_len, int field_no);
    BYTE* UnpackNumericLeft(BYTE *p, char *field, int len, char *field_len, int max_len, int field_no);
    BYTE* UnpackNumericRight(BYTE *p, char *field, int len, char *field_len, int max_len, int field_no);
    BYTE* UnpackUnsupported(BYTE *p, int field_no);

    int SetReplyBitmap();
    int IsFieldPresent(int field);
    void SetBitOn(int bit);
    void SetBitOff(int bit);
    void WriteLogMessage(const char* message);
    int ValidateBitmap();
    int LenNotNum(char *len, int digits);

    BYTE m_message[2000];
    char _logbuf[4000];
    int   m_message_len;
    bool m_bBitmapsValid;
    bool m_bIsValidISOMsg;
    char  m_UnpackError[100+1];


    void LogSwitchMsg (int field, char *text);


    struct
    {
    char  MsgType[4+1];
    BYTE  BitMap[32];
    char  F2Len[3+1];
    char  F2Pan[19+1];
    char  F3ProcessingCode[6+1];
    char  F4Amount[12+1];
    char  F5AmountSettlement[12+1];
    char  F6AmountCardholder[12+1];
    char  F7DateTime[10+1];
    char  F8AmountCardBillFee[8+1];
    char  F9ConvRateSettle[8+1];
    char  F10ConversionRate[8+1];
    char  F11Stan[6+1];
    char  F12TimeLocalTrans[6+1];
    char  F13DateLocalTrans[4+1];
    char  F14ExpiryDate[4+1];
    char  F15DateSettlement[4+1];
    char  F16DateConversion[4+1];
    char  F17DateCapture[4+1];
    char  F18MerchantType[4+1];
    char  F19AcqCtryCode[3+1];
    char  F20PanCountryCode[3+1];
    char  F21FwdCtryCode[3+1];
    char  F22PosEntryMode[3+1];
    char  F23CardSeqNumber[3+1];
    char  F24NetworkIdentifier[3+1];
    char  F25PosCondCode[2+1];
    char  F26PosCaptureCode[2+1];
    char  F27AuthIdRespLen[1+1];
    char  F28TransactionFee[9+1];
    char  F29SettlementFee[9+1];
    char  F30TranProcFee[9+1];
    char  F31SettleProcFee[9+1];
    char  F32Len[3+1];
    char  F32AcquiringId[11+1];
    char  F33Len[3+1];
    char  F33ForwardingId[11+1];
    char  F34Len[3+1];
    char  F34PanExtended[28+1];
    char  F35Len[3+1];
    char  F35Track2[37+1];
    char  F36Len[3+1];
    char  F36Track3[104+1];
    char  F37RefNumber[12+1];
    char  F38AuthNumber[6+1];
    char  F39ResponseCode[2+1];
    char  F40ServiceCode[3+1];
    char  F41TerminalId[8+1];
    char  F42CardAcceptId[15+1];
    char  F43CardAcceptName[40+1];
    char  F44Len[3+1];
    char  F44AdditionalResp[27+1];
    char  F45Len[3+1];
    char  F45Track1[76+1];
    char  F46Len[3+1];
    char  F46AddDataIso[100+1];
    char  F47Len[3+1];
    char  F47AddDataNational[100+1];
    char  F48Len[3+1];
    char  F48AdditionalData[400+1];
    char  F49CurrencyCode[3+1];
    char  F50CurrencyCodeStl[3+1];
    char  F51CurrencyCodeChb[3+1];
    BYTE  F52PinBlock[8+1];
    char  F53SecurityInfo[16+1];
    char  F54Len[3+1];
    char  F54BalanceInfo[120+1];
    char  F57Len[3+1];
    char  F57AmountCash[12+1];
    char  F58Len[3+1];
    char  F58LedgerBalance[12+1];
    char  F59Len[3+1];
    char  F59AvailBalance[12+1];
    char  F60Len[3+1];
    BYTE  F60Private[500+1];
    char  F61Len[3+1];
    BYTE  F61Private[500+1];
    char  F62Len[3+1];
    BYTE  F62Private[500+1];
    char  F63Len[3+1];
    BYTE  F63Private[500+1];
    BYTE  F64Mac[8+1];
    BYTE  F65BitmapExtended[8+1];
    char  F66SettlementCode[1+1];
    char  F67ExtendedPayCode[2+1];
    char  F68ReceiveCtryCode[3+1];
    char  F69SettleCtryCode[3+1];
    char  F70NetworkInfoCode[3+1];
    char  F71MessageNumber[4+1];
    char  F72MessageNumberLast[4+1];
    char  F73DateAction[6+1];
    char  F74CreditsNbr[10+1];
    char  F75CreditsRevNbr[10+1];
    char  F76DebitsNbr[10+1];
    char  F77DebitsRevNbr[10+1];
    char  F78TransferNumber[10+1];
    char  F79TransferReversalNum[10+1];
    char  F80InquiriesNumber[10+1];
    char  F81AuthorisationsNumber[10+1];
    char  F82CreditProcFeeAmt[12+1];
    char  F83CreditTranFeeAmt[12+1];
    char  F84DebitProcFeeAmt[12+1];
    char  F85DebitTranFeeAmt[12+1];
    char  F86CreditsAmt[16+1];
    char  F87CreditsRevAmt[16+1];
    char  F88DebitsAmt[16+1];
    char  F89DebitsRevAmt[16+1];
    char  F90OriginalData[42+1];
    char  F91FileUpdateCode[1+1];
    char  F92FileSecurityCode[2+1];
    char  F93ResponseId[5+1];
    char  F94ServiceIndicator[7+1];
    char  F95ReplaceAmounts[42+1];
    BYTE  F96SecurityCode[8+1];
    char  F97AmtNetSettle[17+1];
    char  F98Len[3+1];
    char  F98Payee[25+1];
    char  F99Len[3+1];
    char  F99SettlementId[11+1];
    char  F100Len[3+1];
    char  F100ReceivingId[11+1];
    char  F101Len[3+1];
    char  F101FileName[17+1];
    char  F102Len[3+1];
    char  F102Account1[28+1];
    char  F103Len[3+1];
    char  F103Account2[28+1];
    char  F104Len[3+1];
    char  F104Description[100+1];
    char  F117CardStatusUpdCode[2+1];
    char  F118CashTotalNum[10+1];
    char  F119CashTotalAmt[16+1];
    char  F120Len[3+1];
    BYTE  F120Private[500+1];
    BYTE  F128Mac[8+1];
    } m_i;

};
