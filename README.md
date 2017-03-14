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

4. 在 `Build Phases` 中的 `Link Binary With Libraries` 中加入 `libz.1.2.5.tbd`

5. 在 `Build Setting` - `Linking - Other Linker Flags` 里添加 `-lstdc++.6.0.9` 和 `-lsqlite3`

### 腾讯定位IOS版本注意事项：
> iOS定位SDK引入了 IDFA，可能会造成您的应用提交AppStore审核失败，请您认真阅读下文

> 1、引入IDFA的目的

> 引入 IDFA 能够较精准的识别用户，能帮助我们最大程度的追踪您在使用 SDK 时的问题，确保SDK稳定性。

> 2、审核时关于 IDFA 选项的选择

> 以上4项代表的含义

> 1)在 App 内投放广告
> 服务应用中的广告。如果你的应用中集成了广告的时候，你需要勾选这一项。

> 2)将此 App 安装归因于先前投放的特定广告
> 跟踪广告带来的安装。如果你使用了第三方的工具来跟踪广告带来的激活以及一些其他事件，但是应用里并没有展示广告你需要勾选这一项。

> 3)将此 App 中发生的操作归因于先前投放的特定广告
> 跟踪广告带来的用户的后续行为。如果你使用了第三方的工具来跟踪广告带来的激活以及一些其他事件。

> 4)iOS 中的“限制广告跟踪”设置

> 对您的应用使用 IDFA 的目的做下确认，只要您获取了 IDFA，那么这一项都是需要勾选的
3、被 AppStore 拒绝的解决办法

> 如果您的应用里只是集成了广告，不追踪广告带来的激活行为，那么选择 1 和 4。
> 如果您的应用没有广告，而又获取了 IDFA。我们建议选择 2 和 4。

参考地址：[http://lbs.qq.com/iosgeo/faq.html](http://lbs.qq.com/iosgeo/faq.html)

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
