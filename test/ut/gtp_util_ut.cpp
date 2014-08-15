#include <limits.h>
#include <iostream>
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "types.hpp"
#include "logger.hpp"
#include "macros.hpp"
#include "gtp_macro.hpp"
#include "gtp_types.hpp"
#include "gtp_util.hpp"

TEST(gtpGetMsgNameTest, Negative)
{
   EXPECT_STREQ(NULL, gtpGetMsgName((GtpMsgType_E)0));
   EXPECT_STREQ(NULL, gtpGetMsgName((GtpMsgType_E)4));
   EXPECT_STREQ(NULL, gtpGetMsgName((GtpMsgType_E)31));
   EXPECT_STREQ(NULL, gtpGetMsgName((GtpMsgType_E)40));
   EXPECT_STREQ(NULL, gtpGetMsgName((GtpMsgType_E)63));
   EXPECT_STREQ(NULL, gtpGetMsgName((GtpMsgType_E)74));
   EXPECT_STREQ(NULL, gtpGetMsgName((GtpMsgType_E)94));
   EXPECT_STREQ(NULL, gtpGetMsgName((GtpMsgType_E)103));
   EXPECT_STREQ(NULL, gtpGetMsgName((GtpMsgType_E)127));
   EXPECT_STREQ(NULL, gtpGetMsgName((GtpMsgType_E)142));
   EXPECT_STREQ(NULL, gtpGetMsgName((GtpMsgType_E)148));
   EXPECT_STREQ(NULL, gtpGetMsgName((GtpMsgType_E)157));
   EXPECT_STREQ(NULL, gtpGetMsgName((GtpMsgType_E)159));
   EXPECT_STREQ(NULL, gtpGetMsgName((GtpMsgType_E)172));
   EXPECT_STREQ(NULL, gtpGetMsgName((GtpMsgType_E)175));
   EXPECT_STREQ(NULL, gtpGetMsgName((GtpMsgType_E)179));
   EXPECT_STREQ(NULL, gtpGetMsgName((GtpMsgType_E)199));
   EXPECT_STREQ(NULL, gtpGetMsgName((GtpMsgType_E)199));
   EXPECT_STREQ(NULL, gtpGetMsgName((GtpMsgType_E)202));
   EXPECT_STREQ(NULL, gtpGetMsgName((GtpMsgType_E)230));
   EXPECT_STREQ(NULL, gtpGetMsgName((GtpMsgType_E)237));
   EXPECT_STREQ(NULL, gtpGetMsgName((GtpMsgType_E)255));
   EXPECT_STREQ(NULL, gtpGetMsgName((GtpMsgType_E)300));
}

TEST(gtpGetMsgNameTest, Positive)
{
   EXPECT_STREQ("Create Sessn Req", gtpGetMsgName(GTPC_MSG_CS_REQ));
   EXPECT_STREQ("Change Notif Rsp", gtpGetMsgName(GTPC_MSG_CN_RSP));
   EXPECT_STREQ("Modify Bearer Cmd", gtpGetMsgName(GTPC_MSG_MB_CMD));
   EXPECT_STREQ("Create Bearer Req", gtpGetMsgName(GTPC_MSG_CB_REQ));
   EXPECT_STREQ("Delete PDN Conn Set Req",\
         gtpGetMsgName(GTPC_MSG_DEL_PDN_CON_SET_REQ));
   EXPECT_STREQ("Identification Req", gtpGetMsgName(GTPC_MSG_ID_REQ));
   EXPECT_STREQ("RAN Info Relay", gtpGetMsgName(GTPC_MSG_RAN_INFO_RELAY));
   EXPECT_STREQ("Detach Notif", gtpGetMsgName(GTPC_MSG_DETACH_NOTIF));
   EXPECT_STREQ("Suspend Notif", gtpGetMsgName(GTPC_MSG_SSPND_NOTIF));
   EXPECT_STREQ("Resume Notif", gtpGetMsgName(GTPC_MSG_RSME_NOTIF));
   EXPECT_STREQ("Create Fwding Tun Req", gtpGetMsgName(GTPC_MSG_CF_TUNN_REQ));
   EXPECT_STREQ("DL Data Notif", gtpGetMsgName(GTPC_MSG_DL_DATA_NOTIF));
   EXPECT_STREQ("Update PDN Conn Set Req",\
         gtpGetMsgName(GTPC_MSG_UPD_PDN_CON_SET_REQ));
   EXPECT_STREQ("MBMS Sessn Start Req",\
         gtpGetMsgName(GTPC_MSG_MBMS_SSN_START_REQ));
   EXPECT_STREQ("MBMS Sessn Start Rsp",\
         gtpGetMsgName(GTPC_MSG_MBMS_SSN_START_RSP));
   EXPECT_STREQ("MBMS Sessn Stop Rsp",\
         gtpGetMsgName(GTPC_MSG_MBMS_SSN_STOP_RSP));
}
