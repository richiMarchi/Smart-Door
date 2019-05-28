"use strict";

var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

var Led = function () {
    function Led(gpio, id) {
        _classCallCheck(this, Led);

        this._gpio = gpio;
        this._id = id;
    }

    _createClass(Led, [{
        key: "switchOn",
        value: function switchOn() {
            console.log("[LED " + this._id + "] ON");
        }
    }, {
        key: "switchOff",
        value: function switchOff() {
            console.log("[LED " + this._id + "] OFF");
        }
    }, {
        key: "blink",
        value: function blink(onTime) {
            var _this = this;

            this.switchOn();
            setTimeout(function () {
                return _this.switchOff();
            }, onTime);
        }
    }]);

    return Led;
}();

module.exports = Led;