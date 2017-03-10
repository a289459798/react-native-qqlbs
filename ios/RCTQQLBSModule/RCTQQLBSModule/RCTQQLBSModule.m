//
//  RCTQQLBSModule.m
//  RCTQQLBSModule
//
//  Created by zhangzy on 2017/3/9.
//  Copyright © 2017年 zzy. All rights reserved.
//

#import "RCTQQLBSModule.h"
#import <TencentLBS/TencentLBS.h>

@interface RCTQQLBSModule ()<TencentLBSLocationManagerDelegate>

@property (readwrite, nonatomic, strong) TencentLBSLocationManager *locationManager;

@end

@implementation RCTQQLBSModule

RCT_EXPORT_MODULE();


RCT_EXPORT_METHOD(requestLocationUpdates:(NSInteger) interval level:(NSInteger) level cache:(BOOL)cache)
{

    dispatch_async(dispatch_get_main_queue(), ^{
        
        NSString* File = [[NSBundle mainBundle] pathForResource:@"Info" ofType:@"plist"];
        NSMutableDictionary* dict = [[NSMutableDictionary alloc] initWithContentsOfFile:File];
        NSString *key = [dict objectForKey:@"TencentMapSDK"];
        
        if(key == NULL) {
            [self sendEventWithName:@"lbs_error" body:@"请在Info.plist文件中设置TencentMapSDK"];
            return;
        }
        
        self.locationManager = [[TencentLBSLocationManager alloc] init];
        [self.locationManager setDelegate:self];
        [self.locationManager setApiKey:key];
        [self.locationManager setPausesLocationUpdatesAutomatically:NO];
        [self.locationManager setRequestLevel:level];
        [self.locationManager setPoiUpdateInterval:interval];
        CLAuthorizationStatus authorizationStatus = [CLLocationManager authorizationStatus];
        if (authorizationStatus == kCLAuthorizationStatusNotDetermined) {
            [self.locationManager requestWhenInUseAuthorization];
        }
        
        [self.locationManager startUpdatingLocation];
    });
    
}

RCT_EXPORT_METHOD(stopLocationUpdates)
{
    
    [self.locationManager stopUpdatingLocation];
}

- (NSArray<NSString *> *)supportedEvents {
    NSMutableArray *arr = [[NSMutableArray alloc] init];
    
    [arr addObject:@"lbs_success"];
    [arr addObject:@"lbs_error"];
    [arr addObject:@"lbs_update"];
    
    return arr;
}

#pragma mark - TencentLBSLocationManagerDelegate

- (void)tencentLBSLocationManager:(TencentLBSLocationManager *)manager
                 didFailWithError:(NSError *)error {
    
    [self sendEventWithName:@"lbs_error" body:error];
}

- (void)tencentLBSLocationManager:(TencentLBSLocationManager *)manager
                didUpdateLocation:(TencentLBSLocation *)location {
    
    NSMutableDictionary *param = [[NSMutableDictionary alloc] init];
    [param setValue:[NSString stringWithFormat:@"%f", location.location.coordinate.latitude] forKey:@"latitude"];
    [param setValue:[NSString stringWithFormat:@"%f", location.location.coordinate.longitude] forKey:@"longitude"];
    [param setValue:[NSString stringWithFormat:@"%f", location.location.altitude] forKey:@"altitude"];
    [param setValue:[NSString stringWithFormat:@"%f", location.location.verticalAccuracy] forKey:@"accuracy"];
    [param setValue:location.nation forKey:@"nation"];
    [param setValue:location.province forKey:@"province"];
    [param setValue:location.city forKey:@"city"];
    [param setValue:location.district forKey:@"district"];
    [param setValue:location.town forKey:@"town"];
    [param setValue:location.village forKey:@"village"];
    [param setValue:location.street forKey:@"street"];
    [param setValue:location.street_no forKey:@"streetNo"];
    
    [self sendEventWithName:@"lbs_success" body:param];
}

@end
