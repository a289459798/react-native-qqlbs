package com.zzy.qqlbs;

import com.facebook.react.bridge.*;
import com.facebook.react.modules.core.DeviceEventManagerModule;
import com.tencent.map.geolocation.TencentLocation;
import com.tencent.map.geolocation.TencentLocationListener;
import com.tencent.map.geolocation.TencentLocationManager;
import com.tencent.map.geolocation.TencentLocationRequest;

/**
 * Created by zzy on 2017/3/9.
 * Date : 2017/3/9 17:10
 */
public class QQLBSModule extends ReactContextBaseJavaModule implements TencentLocationListener {

    private TencentLocationManager mTencentLocationManager;
    public QQLBSModule(ReactApplicationContext reactContext) {
        super(reactContext);
    }

    @Override
    public String getName() {
        return "QQLBSModule";
    }

    @ReactMethod
    public void requestLocationUpdates(Integer interval, Integer level, Boolean cache) {

        TencentLocationRequest tencentLocationRequest = TencentLocationRequest.create();
        tencentLocationRequest.setInterval(interval);
        tencentLocationRequest.setRequestLevel(level);
        tencentLocationRequest.setAllowCache(cache);
        mTencentLocationManager = TencentLocationManager.getInstance(getReactApplicationContext());
        mTencentLocationManager.requestLocationUpdates(tencentLocationRequest, this);
    }

    @ReactMethod
    public void stopLocationUpdates() {
        mTencentLocationManager.removeUpdates(this);
    }

    @Override
    public void onLocationChanged(TencentLocation tencentLocation, int i, String s) {

        if (i == TencentLocation.ERROR_OK) {
            WritableMap param = Arguments.createMap();
            param.putDouble("latitude", tencentLocation.getLatitude());
            param.putDouble("longitude", tencentLocation.getLongitude());
            param.putDouble("altitude", tencentLocation.getAltitude());
            param.putDouble("accuracy", tencentLocation.getAccuracy());
            param.putString("nation", tencentLocation.getNation());
            param.putString("province", tencentLocation.getProvince());
            param.putString("city", tencentLocation.getCity());
            param.putString("district", tencentLocation.getDistrict());
            param.putString("town", tencentLocation.getTown());
            param.putString("village", tencentLocation.getVillage());
            param.putString("street", tencentLocation.getStreet());
            param.putString("streetNo", tencentLocation.getStreetNo());

            QQLBSPackage.sReactContext.getJSModule(DeviceEventManagerModule.RCTDeviceEventEmitter.class).emit("lbs_success", param);
        } else {

            QQLBSPackage.sReactContext.getJSModule(DeviceEventManagerModule.RCTDeviceEventEmitter.class).emit("lbs_error", s);
        }
    }

    @Override
    public void onStatusUpdate(String s, int i, String s1) {
        WritableMap param = Arguments.createMap();
        param.putString("name", s);
        param.putInt("status", i);
        param.putString("desc", s1);

        QQLBSPackage.sReactContext.getJSModule(DeviceEventManagerModule.RCTDeviceEventEmitter.class).emit("lbs_update", param);
    }
}
