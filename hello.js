console.log('123')
var addon = require('bindings')('build-node-addon-api-with-cmake');
console.log(addon.hello)
console.log(addon.hello()); // 'world'


