//
//  HLCustomAliveNetSessionProtocol.h
//  HalleyAliveSDK
//
//  Created by acewu on 7/11/16.
//  Copyright © 2016 Tencent. All rights reserved.
//

#import <Foundation/Foundation.h>
// session 遵从的协议
@protocol HLCustomAliveNetSessionProtocol <NSObject>
// 发送数据包
- (void)customSendPacket:(NSData *)packetData;
// 当前状态
- (HLNetState)customCurrentNetState;
// 重置连接
- (void)customAsyncResetNet;
// 断开连接
- (void)customAsyncDisconnect;
// 连接
- (void)customAsyncConnect;
@end

// session 需要的参数
@protocol HLCustomAliveNetSessionArgsProtocol <NSObject>
// 收到了数据包
- (void)hlCustomTag:(id)tag recvPacket:(NSData *)packetData;
@optional
// 连接的 IP 列表(支持域名) - 不实现则使用默认的 domain
- (NSMutableArray <NSString *>*)hlCustomTag:(id)tag IPListToHost:(NSString *)host;
// 网络状态改变
- (void)hlCustomTag:(id)tag netStateChange:(HLNetState)netState;
// 重连间隔
- (NSTimeInterval)hlCustomTagMinReconnectInterval:(id)tag;
// 心跳间隔
- (NSTimeInterval)hlCustomTagHeartBeatInterval:(id)tag;
// 心跳数据
- (NSData *)hlCustomTagHeartBeatData:(id)tag;

@end
