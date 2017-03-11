# 集成腾讯地图定位功能

请先到腾讯地图开放平台申请key，申请地址[http://lbs.qq.com/index.html](http://lbs.qq.com/index.html)

##作者

QQ: 289459798
QQ群: 161263093

欢迎更多的喜欢开源的小伙伴加入

### 安装

```
> npm install react-native-qqlbs --save
> react-native link
```

### android

请在项目的`AndroidManifest.xml`文件中加入

```
<meta-data android:name="TencentMapSDK" android:value="您的key"/>
```

### IOS

1. 首先在`Info.plist`文件中添加字段 `TencentMapSDK` 值为您在腾讯地图开放平台申请的key

```
<key>TencentMapSDK</key>
<string>您的key</string>
```

2. 在项目的的`Build Settings` 中搜索 `header` 找到 `Framework Search Paths` 添加 `$(SRCROOT)/../node_modules/react-native-qqlbs/ios/RCTQQLBSModule`

3. 将 `Libraries` -> `RCTQQLBSModule.xcodeproj` -> `RCTQQLBSModule` 下面的 `HalleyAliveSDK.framework` 和 `TencentLBS.framework` 文件拖到您的主工程下，不需要勾选 `Copy items if needed`

4. 在 `Build Phases` 中的 `Link Binary With Libraries` 中加入 `libz.1.2.3.tbd`

5. 在 `Build Setting` - `Linking - Other Linker Flags` 里添加 `-lstdc++.6.0.9` 和 `-lsqlite3`

### 使用方式

```
componentDidMount() {

    QQLBS.addListener("lbs_success", (data) => {

        console.log("lbs_success", data);
    });

    QQLBS.addListener("lbs_error", (data) => {

        console.log("lbs_error", data);
    });

    QQLBS.addListener("lbs_update", (data) => {

        console.log("lbs_update", data);
    });
    
    QQLBS.requestLocationUpdates();
}

componentWillUnmount() {

    QQLBS.removeListener("lbs_success");
    QQLBS.removeListener("lbs_error");
    QQLBS.removeListener("lbs_update");
}

```

### 方法列表

```
/**
 * 注册定位监听
 * lbs_success 定位成功
 * lbs_error 定位失败
 * lbs_update 位置更新
 * @param type
 * @param handler
 */
addListener(type, handler) {

    super.addListener(type, handler);

}

removeListener(type) {

    super.removeListener(type);
}

/**
 * 设置定位周期(位置监听器回调周期), 单位为 ms (毫秒)
 * @param interval
 * @returns {QQLBS}
 */
setInterval(interval) {
    this.interval = interval;
    return this;
}

/**
 * 设置定位的 request level
 * 0	包含经纬度
 * 1	包含经纬度, 位置名称, 位置地址
 * 3	包含经纬度，位置所处的中国大陆行政区划
 * 4	包含经纬度，位置所处的中国大陆行政区划及周边POI列表
 * @param level
 * @returns {QQLBS}
 */
setRequestLevel(level) {
    this.level = level;
    return this;
}

/**
 * 设置是否允许使用缓存, 连续多次定位时建议允许缓存
 * @param cache
 * @returns {QQLBS}
 */
setAllowCache(cache) {
    this.cache = cache;
    return this;
}

/**
 * 发起定位
 */
requestLocationUpdates() {

    QQLBSModule.requestLocationUpdates(this.interval, this.level, this.cache);

}

/**
 * 结束定位
 */
stopLocationUpdates() {
    QQLBSModule.stopLocationUpdates();
}
```