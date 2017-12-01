#ifndef __RG4_NET_EASYIV_EVENT_H_
#define __RG4_NET_EASYIV_EVENT_H_

//#pragma warning(disable:4996)

#include <stdlib.h>
#include "rstype.h"
#include "rsmacro.h"
#include "osp.h"
//#include "pu/ezbase.h"

#ifdef _MSC_VER
#include <Windows.h>
#include <WinSock2.h>	//for ntohs and htons
#endif

#include "pu/ezbase.h"
#include "ivs/eziv_opencvdef.h"
#include "ivs/eziv_commondef.h"

const int SERVER_PORT_SMART_STATION		= 20140;	//Old TCP port
const int EZIV_TCPPORT_SERVER			= 20000;	//OSP TCP port
const int EZIV_TCPPORT_ALARM			= 20001;	//OSP TCP port

const int MAX_BUFFER_SIZE_LOG		= 2*1024;
const int MAX_BUFFER_SIZE_CMD		= 32*1024;			//32k
const int MAX_BUFFER_SIZE_SSCMD		= 8*1024;

#define SS_CMD_MAGIC 0xff00ff00

enum EM_EZIV_APP
{
	EM_EZIV_AID_CLIENT = 1,
	EM_EZIV_AID_SERVER,
	EM_EZIV_AID_ALARMSRV,
	EM_EZIV_AID_ALARMMOC,
};

//Daemon Event ID
enum EM_EZIV_CMD_DAEMON
{
	EM_EZIV_POWERON = 0x400,
	EM_EZIV_TIMERMAIN,
	EM_EZIV_SRC_CONNECT,
	EM_EZIV_SRC_DISCONNECT,
	EM_EZIV_SRC_CHECKRECV,
};

//Instance Event ID
enum EZIV_CMD_ID
{
	SS_CMD_INVALID = 0,
	SS_CMD_LOGIN,
	SS_ACK_LOGIN,		//Send to client if LOGIN is OK
	SS_ACK_CMSLIST,		//Send to client if LOGIN is OK
	SS_ACK_CONF,		//Send to client if LOGIN is OK

	SS_CMD_GET_PARAM,
	SS_CMD_SET_PARAM,
	SS_CMD_ALARM_SUBSCRIBOR,

	//////////////////////////////////////////////////////////////////////////
	//EZIV_CLIENTREG_REQ			= 100,
	//EZIV_CLIENTREG_ACK,
	//EZIV_CLIENTREG_NACK,

	EZIV_CMD_LOGIN_REQ			= 100,
	EZIV_CMD_LOGIN_ACK,
	EZIV_CMD_LOGIN_NACK,

	EZIV_CMD_CLIENT_DISCONNECT,
	EZIV_CMD_LOGOUT_REQ = EZIV_CMD_CLIENT_DISCONNECT,

	EZIV_CMD_GETVIDEOLIST_REQ	= 120,
	EZIV_CMD_GETVIDEOLIST_ACK,
	EZIV_CMD_GETVIDEOLIST_NACK,

	EZIV_CMD_GETCMSLIST_REQ,
	EZIV_CMD_GETCMSLIST_ACK,
	EZIV_CMD_GETCMSLIST_NACK,

	EZIV_CMD_GET_PARAM_REQ,
	EZIV_CMD_GET_PARAM_ACK,
	EZIV_CMD_GET_PARAM_NACK,

	EZIV_CMD_SET_PARAM_REQ,
	EZIV_CMD_SET_PARAM_ACK,
	EZIV_CMD_SET_PARAM_NACK,

	EZIV_CMD_ALARM_SUBSCRIBOR_REQ	= 200,
	EZIV_CMD_ALARM_SUBSCRIBOR_ACK,
	EZIV_CMD_ALARM_SUBSCRIBOR_NACK,

	EZIV_CMD_ALARM_PROCESS_FINISHED,

	//Alarms
	EZIV_CMD_ALARM_TRIGGERED		= 220,
	EZIV_CMD_ALARM_TARGET_ENTER,
	EZIV_CMD_ALARM_TARGET_LEAVE,

	//Rules
	EZIV_CMD_RULE_ADD_REQ			= 300,
	EZIV_CMD_RULE_ADD_ACK,
	EZIV_CMD_RULE_REMOVE_REQ,
	EZIV_CMD_RULE_REMOVE_ACK,
	EZIV_CMD_RULE_EDIT_REQ,
	EZIV_CMD_RULE_EDIT_ACK,

	EZIV_CMD_RULE_START_REQ,
	EZIV_CMD_RULE_START_ACK,
	EZIV_CMD_RULE_STOP_REQ,
	EZIV_CMD_RULE_STOP_ACK,

	//CMS
	EZIV_CMD_CMS_ADD_REQ,
	EZIV_CMD_CMS_ADD_ACK,
	EZIV_CMD_CMS_REMOVE_REQ,
	EZIV_CMD_CMS_REMOVE_ACK,

	//Live Video
	EZIV_CMD_START_VIDEO_REQ,
	EZIV_CMD_START_VIDEO_ACK,

	TIMER_CLT_WAIT_REG_ACK,
};

/*
enum SS_CMD_CONTENT_TYPE
{
	SS_CMD_CONTENT_BYTES = 0,
//	UPLOAD_ADD,
//	UPLOAD_REMOVE,
//	UPLOAD_EDIT,
//	UPLOAD_ADDCMS,
//	UPLOAD_REMOVECMS,
//	UPLOAD_ALARMRECV,
	SS_CMD_CONTENT_XML,
};

typedef struct T_SS_CMD_HEADER
{
	s32					m_nCmdStartCode;	//const value 0xff00ff00
	EZIV_CMD_ID			m_emCmdID;			//enum SS_CMD_ID
	SS_CMD_CONTENT_TYPE	m_emContentType;			//0 --> XML, 1 --> struct XXX
	s32					m_nCmdSize;
	s8					m_szUser[33];
	s8					m_szPass[33];

	T_SS_CMD_HEADER()
	{
		Clear();
	}

	void Clear()
	{
		m_emCmdID = SS_CMD_INVALID;
		m_emContentType = SS_CMD_CONTENT_BYTES;
		m_nCmdSize = 0;
		SetMagic(SS_CMD_MAGIC);
		memset(m_szUser, 0, 33);
		memset(m_szPass, 0, 33);
	}

	s32 GetMagic()
	{
		return m_nCmdStartCode;
	}

	void SetMagic(s32 nMagic)
	{
		m_nCmdStartCode = nMagic;
	}

}SS_CMD_HEADER;

typedef struct T_SS_CMD
{
	SS_CMD_HEADER m_vCmdHeader;
	s8	m_szContent[MAX_BUFFER_SIZE_SSCMD];
}SS_CMD;
*/

//FIXME Jacky: there are endian type issue here
typedef struct tagEZIVServer
{
	s8					m_achStationIP[128];
	u16					m_wStationPort;
	s8					m_achStationUser[32];
	s8					m_achStationPwd[32];
}TEZIVServer;

typedef struct tagEZIVClientLoginREQ
{
	s8					m_achStationUser[32];
	s8					m_achStationPwd[32];
}TEZIVClientLoginREQ;

//FIXME Jacky: there are endian type issue here
typedef struct tagEZIVClientLoginAck
{
	u16					m_wClientId;	// Osp NodeID assigned by server
	s32					m_nResult;		// 0: success, otherwise fail.
}TEZIVClientLoginAck;

//Live video streaming related
typedef struct tagEZIVRecvVideoREQ
{
	LIVECMD				m_tLiveCmd;
	u32					m_dwIPAddr;
	u16					m_wRecvRtpPort;
	u16					m_wRecvRtcpPort;
}TEZIVRecvVideoREQ;

typedef struct tagEZIVRecvVideoACK
{
	s32					m_nResult;
	u16					m_wSvrRtpPort;
	u16					m_wSvrRtcpPort;
}TEZIVRecvVideoACK;

//EZIV_CMD_RULE_START_REQ,
typedef struct tagEZIVRuleStartREQ
{
	CAMINFO				m_tCamInfo;
	s32					m_nRuleID;
}TEZIVRuleStartREQ;

//EZIV_CMD_RULE_START_ACK,
typedef struct tagEZIVRuleStartACK
{
	s32					m_nResult;
}TEZIVRuleStartACK;

//EZIV_CMD_RULE_STOP_REQ,
typedef struct tagEZIVRuleStopREQ
{
	CAMINFO				m_tCamInfo;
	s32					m_nRuleID;
}TEZIVRuleStopREQ;

//EZIV_CMD_RULE_STOP_ACK,
typedef struct tagEZIVRuleStopACK
{
	s32					m_nResult;
}TEZIVRuleStopACK;

//for EZIV_CMD_RULE_ADD_REQ, EZIV_CMD_RULE_REMOVE_REQ, EZIV_CMD_RULE_EDIT_REQ,
// the real data will be appended after this structure according to the m_emIVType
typedef struct tagEZIVRule
{
	CAMINFO				m_tCamInfo;
	s32					m_nRuleID;
	//EZIV_TYPE			m_emIVType;
	s32					m_emRuleType;
}TEZIVRule;

//for EZIV_CMD_RULE_ADD_ACK, EZIV_CMD_RULE_REMOVE_ACK, EZIV_CMD_RULE_EDIT_ACK,
typedef struct tagEZIVRuleACK
{
	s32					m_nRuleID;
	s32					m_nRuleProcessType;
	s32					m_nResult;
}TEZIVRuleACK;

typedef struct tagEZIVMsg
{
public:
	enum
	{
		PROXY_MSG_HEAD_LEN     = 24,
		MAX_PROXY_MSGBODY_LEN  = 1024*32,
		MAX_PROXY_MSG_LEN      = 1024*32 + 24,
	};

private:
	u16	m_wEventId;         //事件ID号  (暂时不用,以备扩展)
	u16 m_wMsgSrc;          //事件来源  =  emMsgSrcOSP/emMsgSrcStack,

	// 信令句柄（协议栈填写）
	u32 m_hsCall;           //呼叫句柄
	u32 m_hsRAS;            //呼叫句柄

	// 目的OSP地址(在呼叫信令中使用,为本方和对等Server方的呼叫对的IID)
	u32 m_dwSrcCallIId; //AppId+InstId 本方呼叫对的IID
	u32 m_dwDstCallIId; //AppId+InstId 对等方呼叫对的IID

	u16 m_wReserved;  
	u16 m_wMsgBodyLen;          //消息体的长度
	u8  m_abyBuffer[ MAX_PROXY_MSGBODY_LEN ]; //消息体的标准载荷   

public:
	tagEZIVMsg()
	{
		Clear();	
	}

	tagEZIVMsg( u8 * const pbyMsg, u16 wMsgLen )
	{
		if( wMsgLen < PROXY_MSG_HEAD_LEN || pbyMsg == NULL )
			return;

		Clear();

		u16 wCurMsgLen = MIN( wMsgLen, (u16)MAX_PROXY_MSG_LEN );
		memcpy( this, pbyMsg, wCurMsgLen );

		//set length
		SetMsgBodyLen( wCurMsgLen - PROXY_MSG_HEAD_LEN );
	}

	void Clear()
	{
		memset( this, 0, MAX_PROXY_MSG_LEN );	//清零
	}

	// Src Relative Func 
	void SetMsgSrc( u16 wMsgSrc )
	{
		m_wMsgSrc = htons( wMsgSrc );
	}

	u16 GetMsgSrc() const 
	{
		return ntohs( m_wMsgSrc );
	}

	void SetMsgEvent( u16 wEvent ) 
	{
		m_wEventId = htons( wEvent );
	}

	u16 GetMsgEvent() const
	{
		return ntohs(m_wEventId);
	}

	void SetMsgBodyLen( u16 wMsgBodyLen )
	{
		m_wMsgBodyLen = htons( wMsgBodyLen );
	}

	u16 GetMsgBodyLen( ) const 
	{
		return ntohs( m_wMsgBodyLen );
	}

	u16 GetMsgLen( ) const
	{
		return ( GetMsgBodyLen() + PROXY_MSG_HEAD_LEN );
	}

	void SetMsgBody( const u8 *pbyMsgBody, u16 wLen ) 
	{
		u16 wCurLen = MIN( wLen, (u16)MAX_PROXY_MSGBODY_LEN );
		memset( m_abyBuffer, 0, MAX_PROXY_MSGBODY_LEN );

		if ( pbyMsgBody != NULL && wLen > 0 )
		{
			memcpy( m_abyBuffer, pbyMsgBody, wCurLen );
		}

		SetMsgBodyLen( wCurLen );

	}

	u16 GetMsgBody( u8 *pbyMsgBodyBuf, u16 wBufLen )  const
	{
		u16 wCurLen = MIN( GetMsgBodyLen(), wBufLen );
		memset( pbyMsgBodyBuf, 0, wBufLen );
		memcpy( pbyMsgBodyBuf, m_abyBuffer, wCurLen );
		return( wCurLen );
	}

	const u8 *const GetMsgBody( void ) const
	{
		return( ( u8 *const )m_abyBuffer );
	}

	void CatMsgBody( const u8 *pbyMsgBody, u16 wLen ) 
	{
		u16 wCurBodyLen = GetMsgBodyLen();
		u16 wCurAddLen = MIN(wLen, (u16)(MAX_PROXY_MSGBODY_LEN - wCurBodyLen));
		memcpy( m_abyBuffer + wCurBodyLen, pbyMsgBody, wCurAddLen );
		SetMsgBodyLen( wCurBodyLen + wCurAddLen );
	}

	void SetProxyMsg( const u8* pbyMsg, u16 wMsgLen )
	{
		if( wMsgLen < PROXY_MSG_HEAD_LEN || pbyMsg == NULL || wMsgLen <=0 )
			return;

		Clear();

		u16 wCurMsgLen = MIN(wMsgLen, (u16)MAX_PROXY_MSG_LEN);
		memcpy( this, pbyMsg, wCurMsgLen );

		//set length
		SetMsgBodyLen( wCurMsgLen - PROXY_MSG_HEAD_LEN );
	}

	//获取消息
	const u8* GetProxyMsg() const
	{
		return (u8*)this;
	}

}
#if defined(_VXWORKS_) || (defined(_LINUX_) && !defined(_EQUATOR_))
__attribute__ ((packed)) 
#endif
TEZIVMsg, *PTEZIVMsg;
//////////////////////////////////////////////////////////////////////////

typedef void (*EZIVCMDCallback)(MemId hParam, EZIV_CMD_ID emCmdID, u8* pbyReply, u32 dwBytes);
typedef void (*EZIV_ALARMCB)(MemId hParam, EZIV_CMD_ID emCmdID, u8* pbyReply, u32 dwBytes);

typedef struct tagEZIVClientConf
{
	MemId			m_lpParam;
	EZIVCMDCallback m_fpCmdCallback;
	EZIV_ALARMCB	m_fpAlarmCB;
}TEZIVClientConf;

#endif // __RG4_NET_EASYIV_EVENT_H_
