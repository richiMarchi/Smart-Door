"use strict";

var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

var Gpio = void 0;

try {
    Gpio = require('onoff').Gpio;
} catch (error) {
    throw new Error("Can't import onoff library. Maybe this device doesn't support GPIOs.");
}

var Led = function () {
    function Led(gpio, id) {
        _classCallCheck(this, Led);

        if (!Gpio.accessible) {
            throw new Error('GPIOs are not accessible in this device.');
        }
        this._gpio = gpio;
        this._id = id;
        this._led = new Gpio(this._gpio, 'out');
    }

    _createClass(Led, [{
        key: "switchOn",
        value: function switchOn() {
            this._led.writeSync(Gpio.HIGH);
        }
    }, {
        key: "switchOff",
        value: function switchOff() {
            this._led.writeSync(Gpio.LOW);
        }
    }, {
        key: "blink",
        value: function blink(timeOn) {
            var _this = this;

            this.switchOn();
            setTimeout(function () {
                return _this.switchOff();
            });
        }
    }]);

    return Led;
}();

module.exports = Led;