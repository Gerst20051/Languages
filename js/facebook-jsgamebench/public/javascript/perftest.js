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

var PerfTest = (function() {
    var ninja_sprites = [
      'ninja1',
      'ninja2',
      'box1',
      'box2',
      'board1',
      'board2'
    ];

    var ship_sprites = [
      'ship',
      'rock',
      'boom',
      'powerup'
    ];

    function incShip(count, x, y) {
      var sprite = ship_sprites[count % ship_sprites.length];
      Gob.add(Utils.uuidv4(), sprite, parseInt(Math.random() * 8), [Math.random() * x, Math.random() * y], [Math.random() * 10 + 1, 0], Math.random()*2000);
    }

    function incShipScaled(count, x, y) {
      var sprite = ship_sprites[count % ship_sprites.length];
      Gob.add(Utils.uuidv4(), sprite, parseInt(Math.random() * 8), [Math.random() * x, Math.random() * y], [Math.random() * 10 + 1, 0], Math.random()*2000, Math.random()*2+0.25);
    }

    function incShipRot(count, x, y) {
      var sprite = ship_sprites[count % ship_sprites.length];
      Gob.add(Utils.uuidv4(), sprite, parseInt(Math.random() * 8), [Math.random() * x, Math.random() * y], [Math.random() * 10 - 5, Math.random() * 10 - 5]);
    }

    function incNinja(count, x, y) {
      var sprite = ninja_sprites[count % ninja_sprites.length];
      Gob.add(Utils.uuidv4(), sprite, parseInt(Math.random() * 8), [Math.random() * x, Math.random() * y], [Math.random() * 10 + 1, 0]);
    }

    function incNinjaRot(count, x, y) {
      var sprite = ninja_sprites[count % ninja_sprites.length];
      Gob.add(Utils.uuidv4(), sprite, parseInt(Math.random() * 8), [Math.random() * x, Math.random() * y], [Math.random() * 10 - 5, Math.random() * 10 - 5]);
    }

    function dec(count) {
      for (var id in Gob.gobs) {
        Gob.del(id);
        return;
      }
    }

    function ninjas() {
      Sprites.add('ninja1', {url: '/images/ninja_01.png', frames: 24,
            framepos: [[0, 0], [1, 0], [2, 0], [3, 0], [4, 0], [5, 0], [6, 0], [7, 0],
                       [0, 1], [1, 1], [2, 1], [3, 1], [4, 1], [5, 1], [6, 1], [7, 1],
                       [0, 2], [1, 2], [2, 2], [3, 2], [4, 2], [5, 2], [6, 2], [7, 2]],
            width: 128, height: 128});

      Sprites.add('ninja2', {url: '/images/ninja_02.png', frames: 24,
            framepos: [[0, 0], [1, 0], [2, 0], [3, 0], [4, 0], [5, 0], [6, 0], [7, 0],
                       [0, 1], [1, 1], [2, 1], [3, 1], [4, 1], [5, 1], [6, 1], [7, 1],
                       [0, 2], [1, 2], [2, 2], [3, 2], [4, 2], [5, 2], [6, 2], [7, 2]],
            width: 128, height: 128});

      Sprites.add('box1', {url: '/images/box_01.png', frames: 24,
            framepos: [[0, 0], [1, 0], [2, 0], [3, 0], [4, 0], [5, 0], [6, 0], [7, 0],
                       [0, 1], [1, 1], [2, 1], [3, 1], [4, 1], [5, 1], [6, 1], [7, 1],
                       [0, 2], [1, 2], [2, 2], [3, 2], [4, 2], [5, 2], [6, 2], [7, 2]],
            width: 128, height: 128});

      Sprites.add('box2', {url: '/images/box_02.png', frames: 24,
            framepos: [[0, 0], [1, 0], [2, 0], [3, 0], [4, 0], [5, 0], [6, 0], [7, 0],
                       [0, 1], [1, 1], [2, 1], [3, 1], [4, 1], [5, 1], [6, 1], [7, 1],
                       [0, 2], [1, 2], [2, 2], [3, 2], [4, 2], [5, 2], [6, 2], [7, 2]],
            width: 128, height: 128});

      Sprites.add('board1', {url: '/images/board_01.png', frames: 24,
            framepos: [[0, 0], [1, 0], [2, 0], [3, 0], [4, 0], [5, 0], [6, 0], [7, 0],
                       [0, 1], [1, 1], [2, 1], [3, 1], [4, 1], [5, 1], [6, 1], [7, 1],
                       [0, 2], [1, 2], [2, 2], [3, 2], [4, 2], [5, 2], [6, 2], [7, 2]],
            width: 128, height: 128});

      Sprites.add('board2', {url: '/images/board_02.png', frames: 24,
            framepos: [[0, 0], [1, 0], [2, 0], [3, 0], [4, 0], [5, 0], [6, 0], [7, 0],
                       [0, 1], [1, 1], [2, 1], [3, 1], [4, 1], [5, 1], [6, 1], [7, 1],
                       [0, 2], [1, 2], [2, 2], [3, 2], [4, 2], [5, 2], [6, 2], [7, 2]],
            width: 128, height: 128});
    }

    function ship() {
      if (GameFrame.settings.sprite_sheets) {
        Sprites.add('ship', {url: '/images/ship_fbmark.png', frames: 2,
              framepos: [[0, 0], [1, 0]],
              width: 128, height: 128});
        Sprites.add('rock', {url: '/images/asteroid.png', frames: 60,
              framepos: [[0, 0], [1, 0], [2, 0], [3, 0], [4, 0], [5, 0], [6, 0], [7, 0],
                         [0, 1], [1, 1], [2, 1], [3, 1], [4, 1], [5, 1], [6, 1], [7, 1],
                         [0, 2], [1, 2], [2, 2], [3, 2], [4, 2], [5, 2], [6, 2], [7, 2],
                         [0, 3], [1, 3], [2, 3], [3, 3], [4, 3], [5, 3], [6, 3], [7, 3],
                         [0, 4], [1, 4], [2, 4], [3, 4], [4, 4], [5, 4], [6, 4], [7, 4],
                         [0, 5], [1, 5], [2, 5], [3, 5], [4, 5], [5, 5], [6, 5], [7, 5],
                         [0, 6], [1, 6], [2, 6], [3, 6], [4, 6], [5, 6], [6, 6], [7, 6],
                         [0, 7], [1, 7], [2, 7], [3, 7]],
              width: 128, height: 128});
        Sprites.add('boom', {url: '/images/explosion.png', frames: 59,
              framepos: [[0, 0], [1, 0], [2, 0], [3, 0], [4, 0], [5, 0], [6, 0], [7, 0],
                         [0, 1], [1, 1], [2, 1], [3, 1], [4, 1], [5, 1], [6, 1], [7, 1],
                         [0, 2], [1, 2], [2, 2], [3, 2], [4, 2], [5, 2], [6, 2], [7, 2],
                         [0, 3], [1, 3], [2, 3], [3, 3], [4, 3], [5, 3], [6, 3], [7, 3],
                         [0, 4], [1, 4], [2, 4], [3, 4], [4, 4], [5, 4], [6, 4], [7, 4],
                         [0, 5], [1, 5], [2, 5], [3, 5], [4, 5], [5, 5], [6, 5], [7, 5],
                         [0, 6], [1, 6], [2, 6], [3, 6], [4, 6], [5, 6], [6, 6], [7, 6],
                         [0, 7], [1, 7], [2, 7]],
              width: 256, height: 256});
        Sprites.add('powerup', {url: '/images/powerup.png', frames: 40,
              framepos: [[0, 0], [1, 0], [2, 0], [3, 0], [4, 0], [5, 0], [6, 0], [7, 0],
                         [0, 1], [1, 1], [2, 1], [3, 1], [4, 1], [5, 1], [6, 1], [7, 1],
                         [0, 2], [1, 2], [2, 2], [3, 2], [4, 2], [5, 2], [6, 2], [7, 2],
                         [0, 3], [1, 3], [2, 3], [3, 3], [4, 3], [5, 3], [6, 3], [7, 3],
                         [0, 4], [1, 4], [2, 4], [3, 4], [4, 4], [5, 4], [6, 4], [7, 4]],
              width: 64, height: 64});
      } else {
        Sprites.add('ship0', {url: '/images/ship/Test_Ship_Thrust_Frame1_128_00018.png', frames: 2,
              framepos: [[0, 0]],
              width: 128, height: 128});
        Sprites.add('ship1', {url: '/images/ship/Test_Ship_Thrust_Frame2_128_00018.png', frames: 2,
              framepos: [[0, 0]],
              width: 128, height: 128});

        for (var i=0;i<60;i++) {
          Sprites.add('rock'+i, {url: '/images/asteroid/Test_Asteroid_128_000'+(i<10?'0'+i:i)+'.png', frames: 60,
                framepos: [[0, 0]],
                width: 128, height: 128});
        }

        // FIXMECORY - this should be a magical loader thingy
        for (var i=6;i<59;i++) {
          var s = i-6;
          Sprites.add('boom'+s, {url: '/images/explosion/Test_Explosion_0000'+(i<10?'0'+i:i)+'.png', frames: 53,
                framepos: [[0, 0]],
                width: 256, height: 256});
        }

        for (var i=0;i<40;i++) {
          Sprites.add('powerup'+i, {url: '/images/powerup/Test_Powerup_Star_64_0000'+(i<10?'0'+i:i)+'.png', frames: 40,
                framepos: [[0, 0]],
                width: 64, height: 64});
        }
      }
    }

    var sprites = {aa: {sp: ship, inc: incShip, num: 20},
                   ninja: {sp: ninjas, inc: incNinja, num: 15},
                   ninjarot: {sp: ninjas, inc: incNinjaRot, num: 5},
                   rot: {sp: ship, inc: incShipRot, num: 5},
                   scale: {sp: ship, inc: incShipScaled, num: 5}};

    function scrollBackground() {
      Sprites.add('world', {url: '/images/stars.png', frames: 1,
            framepos: [[0, 0]],
            width: 512, height: 511});

      World.initScrollable({scrollx:-10,eachx: 512, eachy: 511,
            tilelist: ['world']});
    }

    function worldBackground() {
      Sprites.add('world', {url: '/images/stars.jpg', frames: 1,
            framepos: [[0, 0]],
            width: 512, height: 511});

      World.initScrollable({eachx: 512, eachy: 511,
            tilelist: ['world']});
    }

    function ninjaBackground() {
      Sprites.add('ninja', {url: '/images/Background_01_1024_0000.png', frames: 1,
            framepos: [[0, 0]],
            width: 1024, height: 768});

      World.add('ninja', 'ninja', [0.5 * JSGlobal.w, 0.5 * JSGlobal.h]);
    }

    function noBackground() {
    }

    var background_loaders = {world: worldBackground, scroll: scrollBackground, none: noBackground, ninja: ninjaBackground};

    var PerfTest = {};

    var tests = [];
    PerfTest.pushTest = function(test) {
      tests.push(test);
    }

    var stops = [];
    PerfTest.pushStop = function(stop) {
      stops.push(stop);
    }

    var viewports = {fluid: 1, fluid_width: 1, normal: 1};


    function addTest(test) {
      tests.push(function() {
          Gob.delAll();
          World.reset();

          var tid = {};
          tid.useragent = JSGlobal.user_agent;
          tid.browser = JSGlobal.browser;
          tid.viewport = test.viewport;
          tid.render_path = '';
          for (var id in test.settings) {
            if (id == 'render_mode') {
              tid.render_mode = test.settings[id];
            } else {
              tid.render_path += id + ':' + test.settings[id] + ' ';
            }
          }

          GameFrame.updateSettings(test.settings, true);
          GameFrame.setXbyY(test.viewport);

          if (test.background) {
            background_loaders[test.background]();
            tid.background = test.background;
          }
          sprites[test.sprites].sp();
          tid.sprites = test.sprites;

          Benchmark.setup({inc: sprites[test.sprites].inc,
                dec: dec,
                tfps: test.tfps,
                num: sprites[test.sprites].num,
                w: JSGlobal.w,
                h: JSGlobal.h,
                tid: tid,
                demo: test.demo});
          if (!test.hack) {
            UI.addButton('', 'stoptest', {pos: [5, 5], width: 100, height: 20, text: 'Stop Perf Test', command: {cmd: 'stopperftest', args: []}});
          }
        });
    }

    PerfTest.addTest = addTest;

    var current = 0;

    PerfTest.init = function() {
      UI.del('buttons');
      UI.del('perf');
      Benchmark.reset();
      tests = [];
      current = 0;
      var spar = {'aa':1, 'rot':1};
      var vp = 'normal';
      var bg = 'world';

      for (var sp in spar) {
        if (JSGlobal.browser == JSGlobal.CHROME || JSGlobal.browser == JSGlobal.WEBKIT) {
          addTest({viewport: vp, settings: {render_mode: GameFrame.HTML_ONLY, update_existing: true, use_div_background: true, rotate_only: true, css_transitions: true, sprite_sheets: true}, tfps: 30, background: bg, sprites: sp });
          addTest({viewport: vp, settings: {render_mode: GameFrame.HTML_ONLY, update_existing: true, use_div_background: true, rotate_only: false, css_transitions: true, sprite_sheets: true}, tfps: 30, background: bg, sprites: sp });
        }

        if (JSGlobal.browser != JSGlobal.IE9) {
          addTest({viewport: vp, settings: {render_mode: GameFrame.HTML_ONLY, update_existing: true, use_div_background: true, rotate_only: true, css_transitions: false, sprite_sheets: true}, tfps: 30, background: bg, sprites: sp });
          addTest({viewport: vp, settings: {render_mode: GameFrame.HTML_ONLY, update_existing: true, use_div_background: true, rotate_only: false, css_transitions: false, sprite_sheets: true}, tfps: 30, background: bg, sprites: sp });
          addTest({viewport: vp, settings: {render_mode: GameFrame.HTML_ONLY, update_existing: false, use_div_background: true, rotate_only: false, css_transitions: false, sprite_sheets: true}, tfps: 30, background: bg, sprites: sp });
          addTest({viewport: vp, settings: {render_mode: GameFrame.HTML_ONLY, update_existing: true, use_div_background: false, rotate_only: false, css_transitions: false, sprite_sheets: true}, tfps: 30, background: bg, sprites: sp });
          addTest({viewport: vp, settings: {render_mode: GameFrame.HTML_ONLY, update_existing: false, use_div_background: false, rotate_only: false, css_transitions: false, sprite_sheets: true}, tfps: 30, background: bg, sprites: sp });
          addTest({viewport: vp, settings: {render_mode: GameFrame.HTML_ONLY, update_existing: true, use_div_background: true, rotate_only: true, css_transitions: false, sprite_sheets: false}, tfps: 30, background: bg, sprites: sp });
          addTest({viewport: vp, settings: {render_mode: GameFrame.HTML_ONLY, update_existing: false, use_div_background: true, rotate_only: true, css_transitions: false, sprite_sheets: false}, tfps: 30, background: bg, sprites: sp });
          addTest({viewport: vp, settings: {render_mode: GameFrame.HTML_ONLY, update_existing: true, use_div_background: true, rotate_only: false, css_transitions: false, sprite_sheets: false}, tfps: 30, background: bg, sprites: sp });
          addTest({viewport: vp, settings: {render_mode: GameFrame.HTML_ONLY, update_existing: false, use_div_background: true, rotate_only: false, css_transitions: false, sprite_sheets: false}, tfps: 30, background: bg, sprites: sp });
          addTest({viewport: vp, settings: {render_mode: GameFrame.HTML_ONLY, update_existing: false, use_div_background: false, rotate_only: false, css_transitions: false, sprite_sheets: false}, tfps: 30, background: bg, sprites: sp });
          addTest({viewport: vp, settings: {render_mode: GameFrame.HTML_ONLY, update_existing: false, use_div_background: false, rotate_only: true, css_transitions: false, sprite_sheets: true}, tfps: 30, background: bg, sprites: sp });
          addTest({viewport: vp, settings: {render_mode: GameFrame.HTML_ONLY, update_existing: false, use_div_background: false, rotate_only: true, css_transitions: false, sprite_sheets: false}, tfps: 30, background: bg, sprites: sp });
        }
        addTest({viewport: vp, settings: {render_mode: GameFrame.HTML_ONLY, update_existing: true, use_div_background: false, rotate_only: true, css_transitions: false, sprite_sheets: false}, tfps: 30, background: bg, sprites: sp });

        if (!!document.createElement('canvas').getContext) {
          addTest({viewport: vp, settings: {render_mode: GameFrame.CANVAS_ONLY, canvas_background: false, canvas_bake_background: false, sprite_sheets: false}, tfps: 30, background: bg, sprites: sp });
          if (JSGlobal.browser != JSGlobal.IE9) {
            addTest({viewport: vp, settings: {render_mode: GameFrame.CANVAS_ONLY, canvas_background: true, sprite_sheets: true}, tfps: 30, background: bg, sprites: sp });
            addTest({viewport: vp, settings: {render_mode: GameFrame.CANVAS_ONLY, canvas_background: false, canvas_bake_background: false, sprite_sheets: true}, tfps: 30, background: bg, sprites: sp });
            addTest({viewport: vp, settings: {render_mode: GameFrame.CANVAS_ONLY, canvas_background: true, sprite_sheets: false}, tfps: 30, background: bg, sprites: sp });
          }
        }
      }
      tests.sort(function(a,b) {return Math.random()-0.5;});
    }

    PerfTest.done = function(tid, count) {
      FBmark.addScore(tid, count);
      current++;
      PerfTest.doAll();
    }

    PerfTest.stop = function() {
      UI.del('perf');
      UI.del('stoptest');
      Gob.delAll();
      World.reset();
      Benchmark.reset();
      tests = [];
      current = 0;
      for (var i = 0; i < stops.length; i++) {
        stops[i]();
      }
      stops = [];
      Init.reset();
    }

    PerfTest.doAll = function() {
      if (current < tests.length) {
        tests[current]();
      } else {
        var result = FBmark.peak();
        JSGlobal.myscore = parseInt(result.score);
        ClientCmd.logperf(JSGlobal.browser_version, result);
        FBmark.reset();
        PerfTest.stop();
      }
    }

    return PerfTest;
  })();
