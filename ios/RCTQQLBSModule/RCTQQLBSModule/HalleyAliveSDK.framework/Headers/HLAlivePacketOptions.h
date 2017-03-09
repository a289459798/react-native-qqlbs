//
//  HLAlivePacketOptions.h
//  HalleyAliveSDK
//
//  Created by acewu on 7/5/16.
//  Copyright © 2016 Tencent. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <HalleyAliveSDK/HLPublicDefines.h>

@protocol HLReportDataProtocol;
@class HLCarrierInfo;


@interface HLAlivePacketOptions : NSObject

@property (nonatomic, readonly) uint32_t seq; // 唯一序号, 可以用来取消请求

- (nonnull instancetype)initWithAppID:(NSInteger)appID;

// ----------------------------- 必填数据 ----------------------------- //
// property
@property (nonatomic) NSInteger appID; // 应用申请的 APPID, 必须已经配置 session .
@property (nonatomic) NSTimeInterval timeout; // <0 永不超时. =0 使用云控默认值 >0 自定义值

// 无网环境时, 是否需要缓存等待有网(直至超时). YES=禁止缓存请求, 无网时直接返回.  默认 = NO.
@property (nonatomic) BOOL banCache;

// 是否允许重试:(PS: 开启可能会导致服务器收到两次一样的请求)		默认 = YES
//		代理模式下: < YES=允许网络断链或切换时, 进行重试 >
//		Http 直连模式: < YES=请求失败后, 在有效时间内进行自动重试 >
@property (nonatomic) BOOL canRetry;

@property (nonatomic, nullable, strong) id customTag;// 业务自定义标记 - 标记请求使用
@property (nonatomic, nullable, strong) NSString * customReportMark;// 业务自定义上报信息, 可以根据此标记进行质量统计

// 业务回调的 queue.
//		若填写, 则以指定线程回调;
//		若不指定, 调用时为主线程, 则以主线程回调, 否则以内置默认线程回调.
@property (nonatomic, nullable, strong) dispatch_queue_t callbackQueue;

@property (nonatomic, strong, nullable) HLRequestCallback callback; // 请求回调

// ----------------------------- 请求数据(两种模式根据后台配置只能选其一) ----------------------------- //
// ApplicationData - 常规模式
@property (nonatomic, copy, nullable) NSString *serviceID;
@property (nonatomic, copy, nullable) NSString *reqCmd;
@property (nonatomic, copy, nullable) NSData *reqContentData;
@property (nonatomic, copy, nullable) NSDictionary <NSString *, NSData *> *extra;



// ApplicationData - http 模式
@property (nonatomic, copy, nonnull) NSString *httpURL;// 请求的 http 地址. (需要携带 http:// )
@property (nonatomic, copy, nullable) NSString *httpMethod;// @"GET", @"POST", @"HEAD".  默认为 "POST"
@property (nonatomic, copy, nullable) NSDictionary <NSString *, NSString *> *httpReqHeaders;// 请求时附加的 http 头域数据
@property (nonatomic, copy, nullable) NSData *httpBody;// http 请求体



// ----------------------------- 请求信息 ----------------------------- //
// req info (read only)
@property (nonatomic, /*readonly*/) NSTimeInterval cacheCost; // 缓存耗时
@property (nonatomic, /*readonly*/) NSTimeInterval sslCost; // ssl握手耗时
@property (nonatomic, /*readonly*/) NSTimeInterval firstCost; // 首字数据耗时
@property (nonatomic, /*readonly*/) NSTimeInterval dataCost; // 接收数据耗时
@property (nonatomic, /*readonly*/) NSTimeInterval totolCost; // 总耗时
@property (nonatomic, /*readonly*/) NSInteger recvlength; //   接收数据长度
@property (nonatomic, /*readonly*/) NSInteger retryTimes; // canRetry=YES 时有效, >0表示重试的次数
@property (nonatomic, /*readonly*/ nullable, strong) NSString *lastRedirectURL; // http 直连模式下, 发生302跳转时, 最后跳转的网址记录
@property (nonatomic, /*readonly*/ nullable, strong) NSString *httpsKidnapCert; //  是否检测疑为劫持. "-":未找到证书
































// ------------------------------------------------------ 内部调用(业务勿用) ---------------------------------------------------------- //

#pragma mark - private (不开放使用)
@property (nonatomic) NSInteger currentSayHelloIndex; // 当前请求时的 sayHello 的序号(防止数据加解密篡位), 0=没有加密, !0=加密序号
@property (nonatomic) BOOL ignoreSecret; // YES=忽略是否已建立加密通道
@property (nonatomic) BOOL disableReport;// 禁用此请求的上报 - 如上报功能本身不需要上报
@property (nonatomic) BOOL isSayHello;
@property (nonatomic) BOOL isCache;
@property (nonatomic) BOOL isFinish;// 是否已完成
@property (nonatomic) BOOL isSpeedReq;// 属于加速通道请求

@property (nonatomic, nullable, strong) NSDate *firstInitDate;// 初始化的时间点
@property (nonatomic, nullable, strong) NSDate *startDate;// 请求发起的时间点
@property (nonatomic, nullable, strong) NSDate *sslDate;// ssl握手时间点
@property (nonatomic) BOOL isFirstSSLDate;// 是否是首次ssl握手时间点
@property (nonatomic, nullable, strong) NSDate *rspHeadDate;// 首个响应头时间点

@property (nonatomic, nullable, strong) NSError *reportError; //  发生错误时的错误
@property (nonatomic) uint32_t httpsSchedulerPort; //  https 请求时使用调度Port
@property (nonatomic, nullable, strong) NSString *realEndURL;// 最终发出的 URL
@property (nonatomic, nullable, strong) NSMutableData *cacheRspData;// 缓存的数据信息
@property (nonatomic, nullable, strong) NSURLResponse *urlRsp;// 返回的 Rsp

@property (nonatomic) BOOL returnOrgRecords; // 是否 直接返回原始的 Records data
@property (nonatomic, nullable, strong) NSMutableArray <NSDictionary <NSString *, id> *>* orgRecords;// 直接解码的数据放于此
@property (nonatomic) BOOL appendClientContent; // 是否附带 ClientContent
// Append records
@property (nonatomic, nullable, strong) NSMutableArray <NSDictionary <NSString *, id>*>*appendRecords; // 附带的额外数据 @{@"type":, @"content"}

@property (nonatomic, weak, nullable) id <HLReportDataProtocol> reportDelegate;
@property (nonatomic, nullable, strong) NSString *currentServerIP;
@property (nonatomic, nullable, strong) NSString *schCode;
@property (nonatomic, nullable, strong) HLCarrierInfo *currentCarrierInfo;
@property (nonatomic) NSInteger currentSessionType;

// 模式自动切换时剩余的 timeout
@property (nonatomic, readonly) NSTimeInterval leftTimeout;

// only for debug
@property (nonatomic) BOOL isHttpDirectModel;// 是否指定为 http 直连服务模式. YES=直连, NO=Halley代理 (Halley代理模式分为 '代理直连<需要在Halley服务器配置域名>' 和 '代理转发' ).  默认值 NO.

// for https ios 10
@property (nonatomic, nullable, strong) NSString *realPureHost;// 若替换过 domain, 则有此值.用来 ssl 校验 host
@property (nonatomic, nullable, strong) NSString *userHost;// 用户指定的 host

@property (nonatomic) NSInteger redirectCount;// 当前重定向次数
@property (nonatomic, nullable, strong) NSString *redirectFromURL;//  代理模式下进行了跳转, 为了简化编程逻辑, 这里记录原地址, 上报时检查这个是否为空, 若是, 则进行置换

// for debug: iOS 10 之 http 模式才有效
@property (nonatomic) BOOL disableHalley;// 禁用哈雷调度

@property (nonatomic) eHTTPModel httpModel;// 当前的 http 请求模式
@property (nonatomic) eHTTPModel tryHttpModel;// 下次准备重试的模式 != eHTTPModelUndef 时, 表示进行尝试
@property (nonatomic, strong, nullable) HLRequestCallback backUpcallback;
- (void)doReport;
- (BOOL)canDoHTTPDirect;




// --- ban ----//
//// 指定请求模式. 一般使用默认值. 涉及第三方调用(非 Tencent 内部服务器) 以及 ATS 开启下的 HTTP 请求时才需要关注.
////		eHTTPModelUndef(推荐): 默认值, 内部自动切换.
////		eHTTPModelAgent: 只能使用代理模式(开启 ATS 的 HTTP 请求需要使用此项).
////		eHTTPModelDirect: 不能使用内部调用的第三方 http 请求使用此项.
@property (nonatomic) eHTTPModel onlyHttpModel;//! 已被弃用, 不再生效
@end
