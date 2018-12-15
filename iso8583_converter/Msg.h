// AS2805Msg.h
//
// This file contains utility functions for encoding and decoding
// AS2805 messages in support of the IDebitC  (Fiserv) interface

#ifndef _AS2805_MSG_H_
#define _AS2805_MSG_H_

//Common ATTM dev library functions
//#include <PosTermRec.h>
//#include "Utilities/MessageQue.h"
//#include "Utilities/OracleConnection.h"
//#include "Utilities/DomLocationTable.h"
//#include "Utilities/NetInterfaceTable.h"
//#include "Utilities/InterTransTable.h"
//#include "Cto/TransGeneric.h"
//#include "Cto/FIServeTrans.h"
//#include "AS2805ResponseCodes.h"
//#include "AS2805MessageTypes.h"

#undef BYTE 
typedef unsigned char BYTE;

#define REPLY_CODE_APPROVED                 "00"
#define REPLY_CODE_CALL_ISSUER              "01"
#define REPLY_CODE_DEACTIVATED_CARD         "04"
#define REPLY_CODE_DO_NOT_HONOUR            "05"
#define REPLY_CODE_APPROVED_PARTIAL_AMT     "10"
#define REPLY_CODE_INVALID_CARD             "14"
#define REPLY_CODE_EXPIRED_CARD             "33"
#define REPLY_CODE_FORMAT_ERROR             "30"
#define REPLY_CODE_FUNC_NOT_SUPPORTED       "40"
#define REPLY_CODE_INSUFFICIENT_FUNDS       "51"
#define REPLY_CODE_INACTIVE_CARD            "54"
#define REPLY_CODE_REPLACED_CARD            "54"
#define REPLY_CODE_INCORRECT_PIN            "55"
#define REPLY_CODE_TRAN_NOT_PERM_CARDHOLDER "57"
#define REPLY_CODE_TRAN_NOT_PERM_TERMINAL   "58"
#define REPLY_CODE_EXCEED_DEBIT_AMOUNT      "61"
#define REPLY_CODE_EXCEED_DEBIT_COUNT       "65"
#define REPLY_CODE_HARD_CAPTURE             "67"


//these ISO codes taken from page C-2, on the advice of Les Pickford (CSFI) & Al Miller (Fiserv)
enum
{
  P3PROCCODE_POSPURCHASE             = 0, //this will come in a 0200
  P3PROCCODE_PREAUTH                 = 0, //this will come in a 0100
  P3PROCCODE_ATMWITHDRAWAL           = 1,
  P3PROCCODE_ADJUSTMENT_DEBIT        = 2,
  P3PROCCODE_GOODSWITHCASH           = 9,
  P3PROCCODE_QUASICASH               = 11,
  P3PROCCODE_DEFERRED_DEBIT          = 18,
  P3PROCCODE_REFUND                  = 20, //also known as a return
  P3PROCCODE_ADJUSTMENT_CREDITRETURN = 22,
  P3PROCCODE_FUNDS_INQUIRY           = 30,
  P3PROCCODE_BALANCE_INQUIRY         = 31,
  P3PROCCODE_BILLPAYMENT             = 50,
  P3PROCCODE_INTERBANK_TRANSFER      = 56
};

// ===================================================================

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

// ===================================================================

class CAS2805Msg
{
public:
  CAS2805Msg();
  CAS2805Msg(BYTE *szSourceBuffer, int nBufLen);
  CAS2805Msg(const CAS2805Msg& other);
  virtual ~CAS2805Msg();

  void Process(BYTE *szSourceBuffer, int nBufLen);
  virtual int ProcessMessageData();

  const BYTE* GetResponseBuf();
  int  GetResponseLen();

/*
  int GetPosTermRec(PosTermRec& posTermRec);
  bool SetMsgType(const std::string& val);
  bool SetTransmissionDateTime(const std::string& val);
  bool SetSystemTraceAuditNumber(const std::string& val);
  bool SetNetworkManagementCode(const std::string& code);
  bool SetForwardingInstitutionID(const std::string& institutionId);
  bool SetReceivingInstitutionID(const std::string& institutionId);
*/
  bool PackMessage(); 
  BYTE* Message();
  int   MessageLen();
  std::string GetCard();
  std::string GetMsgMTIF000();
//  bool CreateSignOffMessage();

//  static const char sDomesticCountryCode[];
//  static const char sDomesticCountryCodeLong[];
//  static const char sInternationalCountryCode[];
//  static const char CUSCAL_INSTITUTION_ID[];
//  static const char sManagementCodeSignOff[];

protected:
  bool  m_bIsValidISOMsg;
  bool  m_bBitmapsValid;
  bool  m_bReturnMessage;
  bool  m_bBadProcCode;
  char  m_sCountry[4];
  bool  m_bIsDomestic;
  CResponseCodes rCodes;
  CMessageTypes mTypes;

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

  char  m_UnpackError[100+1];

  BYTE  m_ReplyMessage[2000];
  int   m_ReplyLen;

  BYTE  m_RequestMessage[2000];
  int   m_RequestLen;

  void  CommonInit();

  // functions called by ProcessMessageData() to handle a particular
  // supported message type:
  //
  void EncodeGenericResponse();

  // Network Management Messages
  void ProcessNET0800_NetworkMgmtRequest();
  void EncodeNET0800_NetworkMgmtResponse();

  void HandleBadProcCode();
  std::string GetReferenceString();
  std::string GetExternalFee();
  std::string GetTransactionDate();
  std::string GetSettlementDate();
  std::string GetCAID();
  std::string GetCATID();
  std::string GetProcCodeF003();
  std::string GetBillingConvRateF010();
  std::string GetSTANF011();
  std::string GetPOSEntryModeF022();
  std::string GetPOSCondCodeF025();
  std::string GetAcquirerId();
  std::string GetOrigMsgMTI();
  std::string GetAcqCurrencyF049();
  std::string GetCurrencyCodeStlF050();
  std::string GetCurrencyCodeChbF051();
  std::string GetCashAmount();
  std::string GetPlasticExpiryDate();
  int PackReply(BYTE *Dest, int nBufLen);
  int IsFieldPresent(int field);
  void SetBitOn(int field);
  void SetBitOff(int field);
  BYTE* PackAlpha(BYTE *p, char *field, int len, char *field_len);
  BYTE* PackAmtSign(BYTE *p, char *field, int len);
  BYTE* PackBinary(BYTE *p, BYTE *field, int len, char *field_len);
  BYTE* PackNumericLeft(BYTE *p, char *field, int len, char *field_len, char pad);
  BYTE* PackNumericRight(BYTE *p, char *field, int len, char *field_len);
  BYTE* PackUnsupported(BYTE *p, int field_no);
  BYTE* UnpackAlpha(BYTE *p, char *field, int len, char *field_len, int max_len, int field_no);
  BYTE* UnpackAmtSign(BYTE *p, char *field, int len, int max_len, int field_no);
  BYTE* UnpackBinary(BYTE *p, BYTE *field, int len, char *field_len, int max_len, int field_no);
  BYTE* UnpackNumericLeft(BYTE *p, char *field, int len, char *field_len, int max_len, int field_no);
  BYTE* UnpackNumericRight(BYTE *p, char *field, int len, char *field_len, int max_len, int field_no);
  BYTE* UnpackUnsupported(BYTE *p, int field_no);
  int LenNotNum(char *len, int digits);
  void LogSwitchMsg(int field, char *text);
  void UnpackMessage(BYTE *szSourceBuffer, int nBufLen);

  int SetReplyBitmap();
  int ValidateBitmap();

  const char* isDomestic();
  virtual std::string GetBaseAmount(char debitCredit);
  bool IsRediATMBin(std::string card);

  bool IsATMTransaction();
  bool IsCashOutTransaction();

  //NpsiString ObtainResult(int nResult);
  //void LogErrDesc(CTransGeneric* pCTO, int nResult);
  BYTE  m_message[2000];
  int   m_message_len;

};

#endif // _AS2805_MSG_H_

