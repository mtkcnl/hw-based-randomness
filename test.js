const crypto = require('crypto');
const hwRng = require('./build/Release/hw_rng');

function getChaChaHardwareRandom(byteLength = 32) {
    const key = Buffer.alloc(32);
    hwRng.fillBuffer(key);

    const iv = Buffer.alloc(16);
    hwRng.fillBuffer(iv);

    const cipher = crypto.createCipheriv('chacha20', key, iv);

    return cipher.update(Buffer.alloc(byteLength));
}

const random = getChaChaHardwareRandom(32);
console.log("ChaCha20 Output:", random.toString('hex'));
