--TEST--
Ming: Simple SWFAction() test
--SKIPIF--
<?php
if (!extension_loaded("ming")) die("skip"); 
if (MING_NEW || MING_ZLIB) die("skip new ming");
?>
--FILE--
<?php /* $Id: swfaction.phpt,v 1.3 2005/07/18 00:14:53 helly Exp $ */

  $s = new SWFShape();
  $f = $s->addFill(0xff, 0, 0);
  $s->setRightFill($f);

  $s->movePenTo(-500,-500);
  $s->drawLineTo(500,-500);
  $s->drawLineTo(500,500);
  $s->drawLineTo(-500,500);
  $s->drawLineTo(-500,-500);

  $p = new SWFSprite();
  $i = $p->add($s);
  $i->setDepth(1);
  $p->nextFrame();

  for($n=0; $n<5; ++$n)
  {
    $i->rotate(-15);
    $p->nextFrame();
  }

  $m = new SWFMovie();
  $m->setBackground(0xff, 0xff, 0xff);
  $m->setDimension(6000,4000);

  $i = $m->add($p);
  $i->setDepth(1);
  $i->moveTo(1000,2000);
  $i->setName("box");

  $m->add(new SWFAction("_root.box._x += 3;"));
  $m->nextFrame();
  $m->add(new SWFAction("gotoFrame(0); play();"));
  $m->nextFrame();

  $m->save(dirname(__FILE__).'/test.swf');
  echo md5_file(dirname(__FILE__).'/test.swf'), "\n";
  unlink(dirname(__FILE__).'/test.swf');
?>
--EXPECT--
9e47538692393b9915faf3fc7e686cd5
