//
//  TencentLBSLocationManager.h
//  TencentLBS
//
//  Created by mirantslu on 16/4/19.
//  Copyright © 2016年 Tencent. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>
#import "TencentLBSLocation.h"

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, TencentLBSRequestLevel) {
    TencentLBSRequestLevelGeo       = 0,
    TencentLBSRequestLevelName      = 1,
    TencentLBSRequestLevelAdminName = 3,
    TencentLBSRequestLevelPoi       = 4,
};

typedef NS_ENUM(NSUInteger, TencentLBSLocationCoordinateType) {
    TencentLBSLocationCoordinateTypeGCJ02 = 0,          //!< 火星坐标，即国测局坐标
    TencentLBSLocationCoordinateTypeWGS84 = 1,          //!< 地球坐标，注：如果是海外，无论设置的是火星坐标还是地球坐标，返回的都是地球坐标
};

typedef NS_ENUM(NSUInteger, TencentLBSLocationError) {
    TencentLBSLocationErrorUnknown = 0,                 //!< 错误码，表示目前位置未知，但是会一直尝试获取
    TencentLBSLocationErrorDenied = 1,                  //!< 错误码，表示定位权限被禁止
    TencentLBSLocationErrorNetwork = 2,                 //!< 错误码，表示网络错误
    TencentLBSLocationErrorHeadingFailure = 3,          //!< 错误码，表示朝向无法确认
    TencentLBSLocationErrorOther = 4,                   //!< 错误码，表示未知错误
};

/**
 *  TencentLBSLocatingCompletionBlock 单次定位返回Block
 *
 *  @param location  位置信息 
 *  @param error     错误信息 参考 TencentLBSLocationError
 */
typedef void (^TencentLBSLocatingCompletionBlock)(TencentLBSLocation * _Nullable location, NSError  * _Nullable error);

@protocol TencentLBSLocationManagerDelegate;

@interface TencentLBSLocationManager : NSObject

/**
 *  API Key, 在使用定位SDK服务之前需要先绑定key。
 */
@property (nonatomic, copy) NSString* apiKey;

/**
 *  实现了 TencentLBSLocationManagerDelegate 协议的类指针。
 */
@property (nonatomic, weak) id<TencentLBSLocationManagerDelegate> delegate;

/**
 *  设定定位的最小更新距离。默认为 kCLDistanceFilterNone 。
 */
@property (nonatomic, assign) CLLocationDistance distanceFilter;

/**
 *  设定定位精度。默认为 kCLLocationAccuracyBest 。
 */
@property (nonatomic, assign) CLLocationAccuracy desiredAccuracy;

/**
 *  指定定位是否会被系统自动暂停。默认为 YES 。
 */
@property (nonatomic, assign) BOOL pausesLocationUpdatesAutomatically;

/**
 *  是否允许后台定位。默认为 NO。
 *  iOS 9.0 以上用户需要设置该选项并且在info.list里面Background Modes 中的 Location updates 处于选中状态才可以使用后台定位权限。iOS 9.0之前可以直接申请总是使用的权限来获得后台定位。
 *
 *  设置为 YES 的时候必须保证 Background Modes 中的 Location updates 处于选中状态，否则会抛出异常。
 */
@property (nonatomic, assign) BOOL allowsBackgroundLocationUpdates;

/**
 *  用户的活动类型
 *
 *  设置用户的活动类型。默认值为 CLActivityTypeOther
 */
@property (nonatomic, assign) CLActivityType activityType;

/**
 * 设置当朝向改变时，每隔多少度调用一次
 * 只有当设备方向的改变值超过该属性值时才激发delegate的方法。
 */
@property(nonatomic, assign) CLLocationDegrees headingFilter;

/**
 * 设置设备当前的朝向
 */
@property(nonatomic, assign) CLDeviceOrientation headingOrientation;

/**
 * 连续定位的逆地理信息请求的Level。默认为TencentLBSRequestLevelGeo
 */
@property (nonatomic, assign) TencentLBSRequestLevel requestLevel;

/**
 *  返回的TencentLBSLocation的location字段的坐标类型。默认为TencentLBSLocationCoordinateTypeGCJ02。
 *
 *  在一次定位过程中，只允许设置一次，不允许重复设置
 */
@property (nonatomic, assign) TencentLBSLocationCoordinateType coordinateType;

/**
 *  指定POI的更新间隔。 默认是10s
 */
@property(nonatomic, assign) NSInteger poiUpdateInterval;

#pragma mark -

- (void)requestWhenInUseAuthorization;

- (void)requestAlwaysAuthorization;

#pragma mark -

/**
 *  单次定位
 *
 *  该方法为下面方法的一层封装。
 *  level默认是TencentLBSRequestLevelPoi
 *  timeout默认是10s
 */
- (BOOL)requestLocationWithCompletionBlock:(TencentLBSLocatingCompletionBlock)completionBlock;

/**
 *  单次定位
 *
 *  注意：不能连续调用该接口，需在上一次返回之后才能再次发起调用。该接口兼容iOS 7.0及以上，因iOS 9.0系统提供单次定位能力，故在9.0以上会调用系统单次定位接口，9.0之前SDK完成封装。可以通过调用stopUpdatingLocation来取消。
 *
 *  @param level 可以根据此参数来对应的获取POI信息
 *  @param timeout 表示获取POI的超时时间。
 *  @param completionBlock 单次定位完成后的Block
 */
- (BOOL)requestLocationWithRequestLevel:(TencentLBSRequestLevel)level
                        locationTimeout:(NSTimeInterval)timeout
                        completionBlock:(TencentLBSLocatingCompletionBlock)completionBlock;

/**
 * 取消单次定位
 **/
- (void)cancelRequestLocation;

/**
 *  开始连续定位
 *
 *  调用此方法会取消掉所有的单次定位请求，可以用来取消单次定位。
 */
- (void)startUpdatingLocation;

/**
 *  停止连续定位
 *
 *  调用此方法会取消掉所有的单次定位请求，可以用来取消单次定位。
 */
- (void)stopUpdatingLocation;

/**
 *  开启更新定位朝向
 */
- (void)startUpdatingHeading;

/**
 *  结束更新定位朝向
 */
- (void)stopUpdatingHeading;

/**
 *  停止展示定位朝向校准提示
 */
- (void)dismissHeadingCalibrationDisplay;

// 测试使用
#if TENCENTLBS_DEBUG
+ (void)upLoadData;
+ (NSData *)getLocationLog;
+ (void)newLocationLog;
#endif

@end


#pragma mark - TencentLBSLocationManagerDelegate

/**
 *  TencentLBSLocationManagerDelegate
 *  定义了发生错误时的错误回调方法，连续定位的回调方法等。
 */
@protocol TencentLBSLocationManagerDelegate <NSObject>
@optional

/**
 *  当定位发生错误时，会调用代理的此方法
 *
 *  @param manager 定位 TencentLBSLocationManager 类
 *  @param error 返回的错误，参考 TencentLBSLocationError
 */
- (void)tencentLBSLocationManager:(TencentLBSLocationManager *)manager
                 didFailWithError:(NSError *)error;

/**
 *  连续定位回调函数
 *
 *  @param manager 定位 TencentLBSLocationManager 类
 *  @param location 定位结果
 */
- (void)tencentLBSLocationManager:(TencentLBSLocationManager *)manager
                didUpdateLocation:(TencentLBSLocation *)location;

/**
 *  定位权限状态改变时回调函数
 *
 *  @param manager 定位 TencentLBSLocationManager 类
 *  @param status  定位权限状态
 */
- (void)tencentLBSLocationManager:(TencentLBSLocationManager *)manager
     didChangeAuthorizationStatus:(CLAuthorizationStatus)status;

/**
 *  定位朝向改变时回调函数
 *
 *  @param manager 定位 TencentLBSLocationManager 类
 *  @param newHeading  新的定位朝向
 */
- (void)tencentLBSLocationManager:(TencentLBSLocationManager *)manager
                 didUpdateHeading:(CLHeading *)newHeading;

/**
 *  是否展示定位朝向校准提示的回调函数
 *
 *  @param manager 定位 TencentLBSLocationManager 类
 */
- (BOOL)tencentLBSLocationManagerShouldDisplayHeadingCalibration:(TencentLBSLocationManager *)manager;

@end

NS_ASSUME_NONNULL_END
