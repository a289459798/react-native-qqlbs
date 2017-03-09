//
//  TencentLBSLocationUtils.h
//  TencentLBS
//
//  Created by mirantslu on 16/8/11.
//  Copyright © 2016年 Tencent. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>

@class TencentLBSLocation;

NS_ASSUME_NONNULL_BEGIN

@interface TencentLBSLocationUtils : NSObject

+ (double)distanceBetweenTwoCoordinate2D:(const CLLocationCoordinate2D *)coordinate coordinateTwo:(const CLLocationCoordinate2D *)coordinate2;

+ (double)distanceBetweenTwoCLLocations:(const CLLocation *)location locationTwo:(const CLLocation *)location2;

+ (double)distanceBetweenTwoTencentLBSLocations:(const TencentLBSLocation *)location locationTwo:(const TencentLBSLocation *)location2;

@end

NS_ASSUME_NONNULL_END
