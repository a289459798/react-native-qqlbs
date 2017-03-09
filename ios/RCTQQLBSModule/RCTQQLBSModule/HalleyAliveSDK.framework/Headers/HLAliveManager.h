//
//  HLAliveManager.h
//  HalleyAliveSDK
//
//  Created by acewu on 3/28/16.
//  Copyright © 2016 Tencent. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol HLCustomAliveNetSessionArgsProtocol;
@protocol HLCustomAliveNetSessionProtocol;

@class HLTask;
@class HLAlivePacketOptions;
@class HLAliveSessionInfo;



#pragma mark - HLBusinessAliveProtocol
// 业务长连接协议
@protocol HLBusinessAliveProtocol <NSObject>
@optional
// 长连接状态改变
- (void)hlAliveAppID:(NSInteger)appID stateChange:(HLNetState)state;
// 收到的服务端推送数据
- (void)hlAliveAppID:(NSInteger)appID recvPushServiceID:(nonnull NSString *)serviceID Cmd:(nonnull NSString *)cmd andData:(nullable NSData *)data;

// push msg
// 服务器推送的 Notification 信息
- (void)hlAliveAppID:(NSInteger)appID recvPushNotificationTitle:(nullable NSString *)title body:(nullable NSString *)body theme:(nullable NSDictionary *)theme action:(nullable NSDictionary *)action;
// 服务器推送的 Data 信息
- (void)hlAliveAppID:(NSInteger)appID recvPushDatas:(nonnull NSDictionary <NSString *, NSData *>*)datas;
// 服务器推送的 Command 信息
- (void)hlAliveAppID:(NSInteger)appID recvPushCommandType:(NSInteger)type args:(nullable NSDictionary <NSString *, NSData *>*)args;
@end






#pragma mark - HLAliveManager
@interface HLAliveManager : NSObject
+ (nonnull instancetype)shareInstance;

- (nonnull instancetype)initWithUniName:(nonnull NSString *)uinName;

- (void)sessionInfoFromAppID:(NSInteger)appid withCallback:(void(^_Nonnull)( HLAliveSessionInfo *_Nonnull info))callback;
// 必选配置
- (void)configSessionWithInfo:(nonnull HLAliveSessionInfo *)info;
/**
 *  设置当前SDK的打印日志级别 -- 默认为 eHLLogLevelWarn 级别
 *
 *  @param level
 */
+ (void)setLogLevel:(eHLLogLevel)level;


// 开启测试模式
// - 只能在 SDK 初始化前调用.
// - 正式环境不可调用.
+ (void)enableDebugModel:(BOOL)enable;

// Config aliveDelegate
- (void)updateAliveDelegate:(nullable id <HLBusinessAliveProtocol>)aliveDelegate forAppID:(NSInteger)appID;
// Config aliveDecallbackQueuelegate . 业务回调的 queue(若主线程调用, 则主线程返回; 否则使用该 queue 返回)
- (void)updateCallbackQueue:(nullable dispatch_queue_t)callbackQueue forAppID:(NSInteger)appID;

// API 请求
- (void)apiSendPacket:(nonnull HLAlivePacketOptions *)packet;

// 异步取消正在进行中的请求 (不能阻止数据发送到服务器, 但会立即回调)
- (void)apiAsyncCancelReqPacket:(nonnull HLAlivePacketOptions *)packet;

// 更新 deviceToken, 用于 APNs
- (void)updateDeviceToken:(nullable NSData *)deviceToken;
// 重置连接
- (void)apiResetNetForAppID:(NSInteger)appID;
- (void)apiResetNetForAppID:(NSInteger)appID forceReconnect:(BOOL)forceReconnect;
// 获取 UDID (不保证唯一性)
- (nonnull NSString *)udid;
// 获取具有唯一性的 deviceID (保证唯一性, 但可能为空, 也可能变化)
- (nullable NSString *)uniDeviceID;





#ifdef kHalleyModelEnableDownload
#pragma mark - download & upload
- (nonnull NSArray <HLTask *> *) apiQuaryAllExistTasksWithAppID:(NSInteger)appID;
- (nullable HLTask *)apiQuaryTaskWithAppID:(NSInteger)appID userTag:(nonnull NSString *)userTag;

// 注意: 返回错误的情况: 指定了 unitag, 并且与之前传入的数据参数不一致, 就会返回空.
//		若要指定 unitag, 需要先查询是否已存在. - apiQuaryTaskWithAppID:userTag:
- (nullable NSError *)apiCreateNewTask:(nonnull HLTask *)task;

- (nullable HLTask *)hlHandleEventsForBackgroundURLSession:(nonnull NSString *)identifier completionHandler:(void (^_Nonnull)())completionHandler;
#endif

#pragma mark - custom model with tcp
// 自定义 session 模式(暂不开放)
- (nonnull id <HLCustomAliveNetSessionProtocol>)createCustomSessionWithHost:(nonnull NSString *)host
																		tag:(nullable id)tag
																argDelegate:(nonnull id <HLCustomAliveNetSessionArgsProtocol>)customArgDelegate
																  dealQueue:(nullable dispatch_queue_t)dealQueue
														   andCallbackQueue:(nullable dispatch_queue_t)callbackQueue
																 andOptions:(nullable NSDictionary *)options;

// for debug only
+ (void)setDebugDict:(nullable NSDictionary <NSString *, id>*)dict;
@end
