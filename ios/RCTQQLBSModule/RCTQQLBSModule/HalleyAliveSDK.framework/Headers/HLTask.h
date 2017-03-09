//
//  HLDownloadTask.h
//  HalleyAliveSDK
//
//  Created by acewu on 7/12/16.
//  Copyright © 2016 Tencent. All rights reserved.
//

#import <Foundation/Foundation.h>

// 当前任务的类型
typedef enum : NSUInteger {
	eHLTaskTypeDownload, // 下载
	eHLTaskTypeBackgroundDownload, // 后台下载
	eHLTaskTypeUpload, // 上传
	eHLTaskTypeBackgroundUpload, // 后台上传
} eHLTaskType;

// 当前 Task 的状态
typedef enum : NSUInteger {
	eHLTaskStatePause,		// 暂停中
	eHLTaskStateWaiting,	// 等待中 - 在上传/下载队列里面, 只是前面有更多任务正在下载 或者当前无网
	eHLTaskStateDoing,		// 上传/下载中
	eHLTaskStateBroken,		// 上传/下载过程中出现了故障 - last error 里面有错误信息
	eHLTaskStateDelete,		// 已经被删除
	eHLTaskStateFinish,		// 已经完成上传/下载, 等待进一步处理
} eHLTaskState;


// 处理过程中是否进行重试的原因
typedef enum : NSInteger {
	eHLTaskBrokenReasonBackgroundClientCleanCache = 2,	//  后台下载过程中, 缓存文件被系统因某些原因自动清除了 - 来源于 NSPOSIXErrorDomain
	
	eHLTaskBrokenReasonFileSystem = -13,				//	文件系统异常
	eHLTaskBrokenReasonClientModifyedWhileDoing = -14,	//  处理过程中本地文件改变
	eHLTaskBrokenReasonNoNet = -15,						//	无网
	eHLTaskBrokenReasonUserCancel = -17,				//	主动取消
	
	eHLTaskBrokenReasonInvalidArg = -39,				//	非法参数调用
	eHLTaskBrokenReasonStatusCode404 = -46,				//	4XX
	eHLTaskBrokenReasonStatusCode502 = -47,				//	5XX
	eHLTaskBrokenReasonServerModifyedWhileDoing = -50,	//  处理过程中服务器文件改变
	eHLTaskBrokenReasonServerRspInvalid = -52,			//  服务器返回非法数据次数过多
	eHLTaskBrokenReasonOther = -53,						//  其他类型错误次数过多
	eHLTaskBrokenReasonRspSizeInvalid = -54,			//  服务器返回的数据大小不正确 - 可能被劫持
	eHLTaskBrokenReasonLogicBug = -55,					//	内部逻辑异常
	eHLTaskBrokenReasonRecvDoneDataMD5Invalid = -56,	//  校验完整数据时 MD5 异常
	eHLTaskBrokenReasonRecvDoneDataSizeInvalid = -57,	//  校验完整数据时 Size 异常
	eHLTaskBrokenReasonUserKill = -999,					//	用户杀死进程  NSURLErrorCancelled
} eHLTaskBrokenReason;


@class HLTask;
@class HLTaskDB;

@protocol HLTaskProtocol <NSObject>
@optional
#pragma mark - state api
// 任务状态改变 - 异步调用
- (void)hlTask:(nonnull HLTask *)task stateChange:(eHLTaskState)state error:(nullable NSError *)error;
// 任务进度更新 - 异步调用
- (void)hlTask:(nonnull HLTask *)task totalBytesDone:(uint64_t)totolDone totalBytesExpectedToDo:(int64_t)totolExpect;

#pragma mark - modify api
// 任务即将进行 NSURLSessionTask - 同步调用(注意不可进行阻塞/耗时操作)
- (void)hlTask:(nonnull HLTask *)task session:(nonnull NSURLSession *)session willDoRequset:(nonnull NSMutableURLRequest*)requset;

// 任务收到 Response - 同步调用(注意不可进行阻塞/耗时操作)
- (NSURLSessionResponseDisposition)hlTask:(nonnull HLTask *)task session:(nonnull NSURLSession *)session recvResponseWithTask:(nonnull NSURLSessionTask*)sessionTask;
@end

@interface HLTask : NSObject
#pragma mark - property write when create only
@property (nonatomic, /*readonly*/) NSInteger appID;// 任务专属的 AppID
@property (nonatomic, /*readonly,*/ copy, nonnull) NSString *orgUrl;// 原始服务器地址
@property (nonatomic, /*readonly,*/ copy, nonnull) NSString *uniTag; // 唯一标记码
@property (nonatomic, /*readonly*/) eHLTaskType taskType;

// only for upload
@property (nonatomic, /*readonly, */ copy, nullable) NSString *orgFilePath;// 上传文件时指定的文件地址
@property (nonatomic, /*readonly, */ copy, nullable) NSData *orgData;// 上传数据时指定的文件地址 - 优先使用orgFilePath. Create 动作完成后,就可以手动设置 nil 释放.

#pragma mark - property read only
@property (nonatomic, readonly, nonnull) NSDate *createDate;// 任务创建时间
@property (nonatomic, readonly, nonnull) NSString *cacheFilePath;// 任务完成后服务端返回的数据文件路径
@property (nonatomic, readonly) int64_t totolSize; //  文件总大小
@property (nonatomic, readonly) int64_t doneSize; // 当前 已经完成的大小
@property (nonatomic, readonly) eHLTaskState currentState;
@property (nonatomic, readonly, nullable) NSError *lastError;// 出现异常状态时的错误信息
@property (nonatomic, readonly) uint64_t realSpeed; // 实时速度  B/s
// to support
// - (uint64_t)avgSpeed; // 启动应用期间的下载平均速度


#pragma mark - property read & write
@property (nonatomic) int64_t expectTotolSize;// (可选)预期的 下载文件长度 - 指定后若不一致, 则认为是劫持并停止下载  (默认值 -1 表示不指定)
@property (nonatomic, copy, nullable) NSString *expectMD5;// 预期的下载文件 MD5, 指定后若不一致, 则认为是劫持并返回错误
@property (nonatomic) BOOL keepStateWhenRelaunch;  // 程序重启后, 是否保持之前的运行状态 - (只对<前台模式> 和 <后台模式之用户主动退出>有效, <后台模式之系统杀死>自动保持之前的运行状态) Default=YES
@property (nonatomic) BOOL allowsCellularAccess;// 是否允许使用蜂窝网络, 默认 YES = 允许 (PS: 后台任务只有在启动前设置才有笑, 而且运行期间修改无效)

#pragma mark - api

- (void)updateDelegate:(nullable id <HLTaskProtocol>)delegate andDelegateCallbackQueue:(nullable dispatch_queue_t)callbackQueue;
- (void)asyncPause;	// 暂停下载
- (void)asyncResume; // 恢复下载
- (void)asyncDelete; // 删除任务 - 会删除已下载的文件和所有配置信息
- (void)asyncRestart; //  清空缓存, 重新下载

// private - (不开放)
@property (nonatomic, weak, nullable) HLTaskDB *taskDB;
@end
