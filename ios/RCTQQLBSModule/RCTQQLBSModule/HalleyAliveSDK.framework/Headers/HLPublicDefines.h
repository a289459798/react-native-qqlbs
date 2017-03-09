//
//  HLPublicDefines.h
//  HalleyAliveSDK
//
//  Created by acewu on 3/24/16.
//  Copyright © 2016 Tencent. All rights reserved.
//

#ifndef HLPublicDefines_h
#define HLPublicDefines_h



#pragma mark - model define
//#define kHalleyModelEnableDownload		// 下载模块
//#define kHalleyModelEnableLog				// 日志模块

#pragma mark -

#import <Foundation/Foundation.h>


typedef void (^HLRequestCallback)(NSInteger code, id extInfo, NSData *rspData, NSError *error);// http 模式时, extInfo 为返回的 http headfields
typedef void (^HLModulePushMsgCallback)(NSString *serviceID, NSString *cmd, NSData *rspData);
typedef void (^HLResourceCallback)(id resourceInfo, NSError *error);

typedef void (^HLConnectCallback)(NSNumber *bestSocketID, NSError *error);
typedef void (^HLDataCallback)(NSNumber *socketID, NSData *rspData, NSTimeInterval cost, NSDate *firstDate);
typedef void (^HLPingCallback)(NSInteger reachRate, NSString *hostIP, NSTimeInterval timeDNS, NSTimeInterval timePing, NSTimeInterval timeTcp);


typedef enum : NSUInteger {
	eHLLogLevelDebug = 0,
	eHLLogLevelInfo,
	eHLLogLevelWarn,
	eHLLogLevelError,
	eHLLogLevelTodo,
	eHLLogLevelBug,
	eHLLogLevelDisable,
} eHLLogLevel;

extern volatile eHLLogLevel g_halleyCurrentLogLevel;// 当前SDK日志级别

#pragma mark -
typedef enum : NSUInteger {
	eHTTPModelUndef = 0,//  未定义
	eHTTPModelAgent,// 代理模式. 这里为1, 是为了后台更好的统计 isAgent. 便于以后扩展
	eHTTPModelDirect,//直连模式
} eHTTPModel;

#pragma mark -

typedef enum : NSInteger {
	// 原系统(兼容)
	eCInvalidWupDecode = -2,		// 服务器返回的数据异常
	eCNetPingFailed = -3,			// 网络不可用 - 有网标记, 但是无法 ping 通
	eCNetUnreachable = -4,			// 网络不可用 - 无网络标记
	eCRedirectTooMany = -5,			// 跳转次数过多
	eCHttpRedirectNull = -6,		// HTTP重定向到空地址
	eCNetChanged = -9,				// 过程中网络发生变化
	eCConnectTimeout = -10,			// TCP/http 连接超时
	eCSocketRemoteClose = -12,		// 远端关闭链接
	eCReadTimeout = -13,			// http/tcp 读超时
	eCCancel = -20,					// 用户取消请求
	
	// 新增
	eCInvalidHost = -32,			// 非法TCP连接服务器地址
	eCConfigWriteError = -33,		// 文件系统写错误 - 现在不会上报
	eCHttpWrongStatusCode = -35,	// http请求正常,Http 返回码异常
	eCHttpWrongStatusCode4XX = -36,	// http请求正常,但返回状态码4XX
	eCHttpWrongStatusCode5XX = -37,	// http请求正常,但返回状态码5XX
	eCNoChanel = -38,				// 通道未建立(超时期间一直没有建立成功)
	eCInvalidArg = -39,				// 非法参数调用
	eCChanNoActive = -40,			// 通道未激活
	eCSocketErr = -41,				// socket 错误
	eCServerReject = -42,			// 服务器拒绝
	eCSSLInvalid = -43,				// SSL握手异常
	eCFileSystem = -44,				// 文件系统异常
	eCDBError = -48,				//  数据库异常
	eCModelDisable = -49,			//  模块被禁用
} eErrorCode;



extern NSString * kErrorDomainNetUnreachable;		// 网络不可达
extern NSString * kErrorDomainNetChanged;			// 请求过程中网络变化
extern NSString * kErrorDomainSocket;				// socket错误
extern NSString * kErrorDomainInvalidArg;			// 非法参数
extern NSString * kErrorDomainTimeout;				// 请求超时
extern NSString * kErrorDomainRspInvalid;			// 服务器返回异常
extern NSString * kErrorDomainFileSystem;			// 文件系统错误
extern NSString * kErrorDomainUser;					// 用户操作
extern NSString * kErrorServerReject;				// 平台返回错误
extern NSString * kErrorAppServerReject;			// 业务服务器返回错误



typedef enum : NSUInteger {
	HLNetStateDisconnect,
	HLNetStateConnecting,
	HLNetStateConnected,
	HLNetStateSecretBuild,
} HLNetState;


#endif /* HLPublicDefines_h */
