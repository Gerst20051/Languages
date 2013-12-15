// Copyright 2004-present Facebook. All Rights Reserved.

// Licensed under the Apache License, Version 2.0 (the "License"); you may
// not use this file except in compliance with the License. You may obtain
// a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
// License for the specific language governing permissions and limitations
// under the License.

function svrtest(user, a, b) {
  console.log('test: a,b = ' + [a, b]);
  Comm.toClient(user, {cmd: 'clienttest', args: ['a', 'b']});
  Comm.toClient(user, {cmd: 'clienttest', args: [1, 2]});
}

var server_cmds = {};

function install(name, func) {
  server_cmds[name] = {func: func};
  exports[name] = function() {
    var command = {cmd: name, args: Array.prototype.slice.call(arguments)};
    var ret = Utils.cmd_exec(server_cmds, user, command);
    if (ret)
      return ret;
  }
}

function logPerf(name, browser, result) {
  LogProc.addNewEntry({browser: browser, score: result.score, details: result});
  Log.info('stats', name, browser + '\t' + result.score + '\t' + JSON.stringify(result));
}

function perfQuery(user, query) {
  LogProc.retPerf(function(response) {
      Comm.toClient(user, {cmd: 'perfresp', args: [response]});
    });
}

install('linktouser', Users.linkToUser);
install('setview', Users.setView);
install('svrtest', svrtest);
install('add', GridSvr.add);
install('remove', GridSvr.remove);
install('transform', GridSvr.transform);
install('logout', Users.logout);
install('logperf', logPerf);
install('perfquery', perfQuery);
install('userid', Server.userId);
install('tagtile', SvrGame.tagTile);
install('shoot', SvrGame.shoot);
install('explode', SvrGame.explode);

exports.exec = function(user, command) {
  //console.log('cmd: ' + JSON.stringify(command));
  var ret = Utils.cmd_exec(server_cmds, user, command);
  if (ret)
    return ret;
};

exports.install = install;
