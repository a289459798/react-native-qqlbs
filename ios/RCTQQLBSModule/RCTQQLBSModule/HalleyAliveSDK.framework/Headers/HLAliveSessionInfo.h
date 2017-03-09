//
//  HLAliveSessionInfo.h
//  HalleyAliveSDK
//
//  Created by acewu on 4/29/16.
//  Copyright © 2016 Tencent. All rights reserved.
//

#import <Foundation/Foundation.h>

#pragma mark - HLAliveSessionInfo
@interface HLAliveSessionInfo : NSObject
@property (nonatomic, readonly) NSUInteger appID;
// propertys
@property (nonatomic, copy, nullable) NSString *qua;
@property (nonatomic, copy, nullable) NSString *uuid;
@property (nonatomic, copy, nullable) NSString *appVersion;
@property (nonatomic, copy, nullable) NSString *beaconKey;
@property (nonatomic, copy, nullable) NSString *channelID;
@property (nonatomic) BOOL banBeaconEventReport;// 是否禁用自上报的灯塔用户统计. 默认值= NO
@property (nonatomic) BOOL banATSCheck;//  是否自动检测 ATS 状态. 默认值 = NO, 自动进行检测

@property (nonatomic, copy, nullable) NSArray <NSString *> *speedURLs;// 需要预热(加速)请求的 URL, 如 https://www.qq.com
@property (nonatomic) BOOL tryConnectWhenLoad;// 第一次启动时, 直接是否进行连接. 默认值 NO.

@property (nonatomic, nullable) dispatch_queue_t callbackQueue;

- (nonnull instancetype)initWithAppID:(NSUInteger)appID;






#pragma mark - private & unsupport
// ------------------ private(不开放) ------------------ //
@property (nonatomic) BOOL configedMsgPush; // 是否已配置 msg Push
@end
