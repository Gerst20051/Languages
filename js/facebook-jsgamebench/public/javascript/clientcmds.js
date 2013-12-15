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

var ClientCmd = (function() {
    function resetSession() {
      client_user.unique_id = 0;
    }

    function startSession(id, app_id) {
      client_user.unique_id = id;
      client_user.app_id = app_id;
      Init.reset();
    }

    function clientTest(a, b) {
//      console.log('clienttest: ' + [a, b]);
    }

    function disconnected(id) {
//      console.log('disconnected: ' + id);
    }

    function add(idx,obj) {
      GridClient.recvAdd(client_user.grids[idx], obj);
    }

    function remove(idx,uuid,timestamp) {
      GridClient.recvRemove(client_user.grids[idx], uuid, timestamp);
    }

    function transform(idx, uuid, options) {
      GridClient.recvTransform(client_user.grids[idx], uuid, options);
    }

    var last_clock;
    function clock(curr_msecs, frame_msecs) {
      clock = parseInt(curr_msecs);
      GridClient.params.clock_skew = clock - (new Date).getTime();
      GridClient.params.frame_msecs = parseInt(frame_msecs);
      Grid.setClockSkew(GridClient.params.clock_skew);
      if (last_clock) {
        var push_latency = clock - last_clock;
      }
      last_clock = clock;
    }

    function logPerf(browser, result) {
      Xhr.toServer({cmd: 'logperf', args: [browser, result]});
    }

    function perfQuery(query) {
      Xhr.toServer({cmd: 'perfquery', args: [query]});
    }

    function startPerfTest() {
      PerfTest.init();
      PerfTest.doAll();
    }

    function stopPerfTest() {
      PerfTest.stop();
    }

    function hideDetails() {
      UI.del('details');
      UI.del('glossary');
    }

    function showDetails(data) {
      UI.del('details');
      UI.del('glossary');
      UI.addCollection('perf', 'details', {uiclass: 'perfblock', pos: [265, 0], width: 1000, height: 1200, command: {cmd:'hidedetails', args:[]}});
      var subscores = data.details;
      var stats = '';
      for (var rm in subscores) {
        for (var sp in subscores[rm]) {
          stats += (rm == 'canvas' ? '&lt;canvas&gt;' : '&lt;div&gt;') + ' ' + (sp == 'aa' ? 'axis-aligned images' : 'rotated images') + '<br />';

          var sorted = [];
          for (var s in  subscores[rm][sp]) {
            sorted.push({path:s, score:parseInt(subscores[rm][sp][s].total/subscores[rm][sp][s].count)});
          }

          sorted.sort(function(a,b) {return b.score - a.score;});

          var max = sorted[0].score;

          for (var i=0,len=sorted.length;i<len;i++) {
            var s = sorted[i].path;
            var sub = s.split(/[\: ]/);
            stats += (s == 0 ? '<b>' : '') + sorted[i].score + ' (-'+parseInt((max-sorted[i].score)/max*100)+'\%) ';

            for (var det = 0, dlen = sub.length; det < dlen; det += 2) {
              switch (sub[det]) {
                case 'canvas_background':
                  if (sub[det + 1] == 'true') {
                    stats += 'sprites drawn with ctx.drawImage in &lt;canvas&gt; over a &lt;canvas&gt; background ';
                  } else {
                    stats += 'sprites drawn with ctx.drawImage in &lt;canvas&gt; over a &lt;div&gt; background ';
                  }
                  break;
                case 'update_existing':
                  if (sub[det + 1] == 'true') {
                    stats += 'dom update ';
                  } else {
                    stats += 'innerHTML ';
                  }
                  break;
                case 'use_div_background':
                  if (sub[det + 1] == 'true') {
                    stats += 'div with background  ';
                  } else {
                    stats += 'div masking img ';
                  }
                  break;
                case 'rotate_only':
                  if (sub[det + 1] == 'true') {
                    stats += 'rotate ';
                  } else {
                    stats += 'matrix ';
                  }
                  break;
                case 'css_transitions':
                  if (sub[det + 1] == 'true') {
                    stats += 'css transition ';
                  }
                  break;
                case 'sprite_sheets':
                  if (sub[det + 1] == 'true') {
                    stats += 'sprite sheets ';
                  } else {
                    stats += 'individual sprites ';
                  }
                  break;
              }
            }
            stats += (s == 0 ? '</b>' : '') + '<br />';
          }
          stats += '<br />';
        }
      }
      var b = data.browser;
      var browser = b.match(/(\w+) \d+/);
      if (browser) {
        browser = browser[1];
      } else {
        browser = b;
      }
      UI.addHTML('details', 'dbrowserdet', {pos: [5, 4], uiclass: 'browsername', markup: b});
      var score = 'Max reported score: ' + parseInt(data.peak) + ' sprites';
      UI.addHTML('details', 'dperfscore', {pos: [3, 24], uiclass: 'perfscore', markup: score});
      score = parseInt(data.total / data.count);
      UI.addHTML('details', 'dperfavescore', {pos: [5, 70], uiclass: 'perfavescore', markup: 'Average reported score: ' + score + ' sprites'});
      stats += '<br />"dom update": update values in dom object when sprites move<br />"innerHTML": rebuild scene each frame when sprites move<br />"div with background": animating sprites are a div element with changing offsets on background image<br />';
      stats += '"div masking img": animating sprites are a div element masking img element<br />"rotate": use css transform property for rotation, left/top for position of sprites<br />"transform": use css transform property for rotation and position of sprites<br />';
      stats += '"css transition": use css transition to rather than updating every frame<br />"sprite sheets": combine animating sprites into sprite sheets';
      UI.addHTML('details', 'detailinfo', {pos: [5, 105], uiclass: 'renderdetails', markup: stats});
    }

    function perfResponse(data) {
      UI.del('fps');
      UI.del('perf');
      UI.addCollection('', 'buttons', {pos: [0, 0]});
      UI.addButton('buttons', 'perftest', {pos: [5, 5], width: 95, height: 20, text: 'Start Test', command: {cmd: 'startperftest', args: []}});
      UI.addButton('buttons', 'canvasdemo', {pos: [105, 5], width: 95, height: 20, text: 'Canvas Demo', command: {cmd: 'canvasdemo', args: []}});
      UI.addButton('buttons', 'htmldemo', {pos: [210, 5], width: 95, height: 20, text: 'HTML Demo', command: {cmd: 'htmldemo', args: []}});
      UI.addButton('buttons', 'rotdemo', {pos: [315, 5], width: 95, height: 20, text: 'Rotate Demo', command: {cmd: 'rotdemo', args: []}});
      UI.addButton('buttons', 'scrollableddemo', {pos: [420, 5], width: 95, height: 20, text: 'Scroll Demo', command: {cmd: 'scrolldemo', args: []}});
      UI.addButton('buttons', 'scaledcdemo', {pos: [525, 5], width: 95, height: 20, text: 'Scaled Canvas', command: {cmd: 'scanvasdemo', args: []}});
      UI.addButton('buttons', 'scaledhdemo', {pos: [630, 5], width: 95, height: 20, text: 'Scaled HTML', command: {cmd: 'shtmldemo', args: []}});
      UI.addCollection(null, 'perf', {pos: [100, 50], width: 260});
      if (JSGlobal.myscore) {
        UI.addHTML('perf', 'myscore', {pos: [350, 10], width:1000,uiclass: 'perfscore', markup: "Your score is " + JSGlobal.myscore + " sprites!"});
      }
      if (data) {
        for (var i = 0, len = data.length; i < len; i++) {
          UI.addCollection('perf', 'perfblock' + i, {uiclass: 'perfblock', pos: [0, 82 * i], height: 78, width: 260, command: {cmd:'showdetails', args:[data[i]]}});
          var b = data[i].browser;
          var browser = b.match(/(\w+) \d+/);
          if (browser) {
            browser = browser[1];
          } else {
            browser = b;
          }
          UI.addHTML('perfblock' + i, 'browserdet' + i, {pos: [5, 4], uiclass: 'browsername', markup: b});
          var score = parseInt(data[i].peak);
          UI.addHTML('perfblock' + i, 'perfscore' + i, {pos: [5, 24], uiclass: 'perfscore', markup: score});
        }
      }
    }

    function canvasDemo() {
      UI.del('buttons');
      UI.del('perf');
      PerfTest.addTest({viewport: 'fluid_width', settings: {render_mode: GameFrame.CANVAS_ONLY, canvas_background: true, sprite_sheets: false}, tfps: 30, background: 'world', sprites: 'aa', demo: true });
      PerfTest.doAll();
    }

    function htmlDemo() {
      UI.del('buttons');
      UI.del('perf');
      PerfTest.addTest({viewport: 'fluid_width', settings: {render_mode: GameFrame.HTML_ONLY, update_existing: false, use_div_background: false, rotate_only: false, css_transitions: false, sprite_sheets: false}, tfps: 30, background: 'world', sprites: 'aa', demo: true });
      PerfTest.doAll();
    }

    function rotDemo() {
      UI.del('buttons');
      UI.del('perf');
      PerfTest.addTest({viewport: 'normal', settings: {render_mode: GameFrame.CANVAS_ONLY, update_existing: true, canvas_background: true, sprite_sheets: true}, tfps: 20, background: 'world', sprites: 'rot', demo: true });
      PerfTest.doAll();
    }

    function scrollDemo() {
      UI.del('buttons');
      UI.del('perf');
      PerfTest.addTest({viewport: 'fluid_width', settings: {render_mode: GameFrame.CANVAS_ONLY, canvas_background: false, canvas_bake_background: false}, tfps: 30, background: 'scroll', sprites: 'aa', demo: true });
      PerfTest.doAll();
    }

    function sCanvasDemo() {
      UI.del('buttons');
      UI.del('perf');
      PerfTest.addTest({viewport: 'fluid_width', settings: {render_mode: GameFrame.CANVAS_ONLY, canvas_background: true, sprite_sheets: false}, tfps: 30, background: 'world', sprites: 'scale', demo: true });
      PerfTest.doAll();
    }

    function sHtmlDemo() {
      UI.del('buttons');
      UI.del('perf');
      PerfTest.addTest({viewport: 'fluid_width', settings: {render_mode: GameFrame.HTML_ONLY, update_existing: false, use_div_background: false, rotate_only: true, css_transitions: false, sprite_sheets: false}, tfps: 30, background: 'world', sprites: 'scale', demo: true });
      PerfTest.doAll();
    }

    function playGame() {
      UI.del('buttons');
      UI.del('perf');

      Game.init({viewport: 'fluid', settings: {render_mode: GameFrame.CANVAS_ONLY, canvas_background: false, canvas_bake_background: false}, tfps: 30, background: 'world', sprites: 'cute', demo: true, hack: true });

      PerfTest.doAll();
    }

    function playGameHTML() {
      UI.del('buttons');
      UI.del('perf');

      Game.init({viewport: 'fluid', settings: {render_mode: GameFrame.HTML_ONLY, update_existing: false, use_div_background: false, rotate_only: true}, tfps: 30, background: 'world', sprites: 'cute', demo: true, hack: true });

      PerfTest.doAll();
    }

    var client_cmds = {};

    var ClientCmd = {};

    function install(name, func) {
      client_cmds[name] = {func: func};
      ClientCmd[name] = function() {
        var command = {cmd: name, args: Array.prototype.slice.call(arguments)};
        var ret = Utils.cmd_exec(client_cmds, null, command, true);
        if (ret)
          return ret;
      }
    }

    install('resetsession', resetSession);
    install('startsession', startSession);
    install('clienttest', clientTest);
    install('disconnected', disconnected);
    install('add', add);
    install('remove', remove);
    install('transform', transform);
    install('logperf', logPerf);
    install('startperftest', startPerfTest);
    install('perfresp', perfResponse);
    install('perfquery', perfQuery);
    install('stopperftest', stopPerfTest);
    install('canvasdemo', canvasDemo);
    install('htmldemo', htmlDemo);
    install('rotdemo', rotDemo);
    install('scanvasdemo', sCanvasDemo);
    install('shtmldemo', sHtmlDemo);
    install('scrolldemo', scrollDemo);
    install('playgame', playGame);
    install('playgamehtml', playGameHTML);
    install('clock', clock);
    install('showdetails', showDetails);
    install('hidedetails', hideDetails);

    function exec(cmd) {
      //console.log("clientcmd: " + JSON.stringify(str));
      var err = Utils.cmd_exec(client_cmds, null, cmd, false);
      if (err)
        {
          console.log(err.args[0]);
        }
    }

    ClientCmd.exec = exec;
    ClientCmd.install = install;
    return ClientCmd;
  })();
