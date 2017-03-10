/**
 * Created by zhangzy on 16/7/27.
 */

'use strict';

import {
    NativeModules,
    Platform,
    NativeEventEmitter
} from 'react-native';

const QQLBSModule = NativeModules.QQLBSModule;

/**
 * 获取app的版本名称\版本号和渠道名
 */
class QQLBS extends NativeEventEmitter {

    // 构造
    constructor(props) {
        super(QQLBSModule);

        // 初始状态
        this.state = {};

        this.interval = 10000;
        this.level = 3;
        this.cache = true;
    }


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

}

QQLBS = new QQLBS();

module.exports = QQLBS;