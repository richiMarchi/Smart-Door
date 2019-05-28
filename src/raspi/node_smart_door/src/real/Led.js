"use strict";

let Gpio;

try {
    Gpio = require('onoff').Gpio;
} catch (error) {
    throw new Error("Can't import onoff library. Maybe this device doesn't support GPIOs.");
}

class Led {
    constructor(gpio, id) {
        if (!Gpio.accessible) {
            throw new Error('GPIOs are not accessible in this device.');
        }
        this._gpio = gpio;
        this._id = id;
        this._led = new Gpio(this._gpio, 'out');
    }

    switchOn() {
        this._led.writeSync(Gpio.HIGH);
    }

    switchOff() {
        this._led.writeSync(Gpio.LOW);
    }

    blink(timeOn) {
        this.switchOn();
        setTimeout(() => this.switchOff(), timeOn);
    }

}

module.exports = Led;