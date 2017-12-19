"use-strict";

var isolate = require("isolate");
var cpp_rpc_tester = require("cpp_rpc_tester");



isolate.on("ping", function()
{
  console.log("got ping");
  var result = new Object();
  result.result = "pong";
  return result;
});

cpp_rpc_tester.start_test();

async.setTimeout(system.exit, 100, [0]);



