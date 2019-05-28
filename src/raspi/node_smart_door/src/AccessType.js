import {Enum} from 'enumify';

class AccessType extends Enum {}

AccessType.initEnum({
    ACCESS_SUCCESS: {
        get description() { return "Access succeeded" },
        get status() { return "success" },
    },
    ACCESS_FAILED: {
        get description() { return "Access failed. Wrong credentials or non-existing user" },
        get status() { return "fail" },
    },
    ACCESS_FAILED_PIR: {
        get description() { return "Access failed. PIR hasn't detected anything within the timeout." },
        get status() { return "fail_pir" },
    },
});

module.exports = AccessType;