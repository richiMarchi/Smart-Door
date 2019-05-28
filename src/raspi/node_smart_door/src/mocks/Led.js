"use strict";

class Led {
    constructor(gpio, id) {
        this._gpio = gpio;
        this._id = id;
    }

    switchOn() {
        console.log("[LED " + this._id + "] ON");
    }

    switchOff() {
        console.log("[LED " + this._id + "] OFF");
    }

    blink(timeOn) {
        this.switchOn();
        setTimeout(() => this.switchOff(), timeOn);
    }

}

module.exports = Led;