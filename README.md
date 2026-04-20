# hw-based-randomness
Randomness with x86_64 RDRAND and RDSEED instructions for Node.js. For more information: https://www.intel.com/content/www/us/en/developer/articles/guide/intel-digital-random-number-generator-drng-software-implementation-guide.html

## How to use this project? 

Install node-gyp with the following command:

```shell
r00t@mirad:~/test_project$ npm install -g node-gyp # run this command with sudo if you are not super-user.
```

And, install the library:

```shell
r00t@mirad:~/test_project$ npm init -y
r00t@mirad:~/test_project$ npm i https://github.com/mtkcnl/hw-based-randomness
```

Include this library to your project:
```js
const {fillBuffer, getRdrand, getRdseed} = require('hw-rng.js');
// Your code...
```