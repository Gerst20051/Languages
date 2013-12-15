/*******************************************************************************
 jquery.mb.components
 Copyright (c) 2001-2010. Matteo Bicocchi (Pupunzi); Open lab srl, Firenze - Italy
 email: mbicocchi@open-lab.com
 site: http://pupunzi.com

 Licences: MIT, GPL
 http://www.opensource.org/licenses/mit-license.php
 http://www.gnu.org/licenses/gpl.html
 ******************************************************************************/

/*
 * jQuery.mb.components: jquery.mb.core.js
 * version: 1- 30-gen-2010 - 52
 * © 2001 - 2011 Matteo Bicocchi (pupunzi), Open Lab
 *
 */

(function($) {

  //to bring any DOM element on top  .
  $.fn.mb_bringToFront= function(el){
    if (!el) el="*";
    var zi=0;
    $(el).each(function() {
      var position= "absolute" || "relative" || "fixed";
      if($(this).css("position")==position){
        var cur = parseInt($(this).css('zIndex'));
        zi = cur > zi ? parseInt($(this).css('zIndex')) : zi;
      }
    });
    $(this).css('zIndex',zi+=1);
    return $(this);
  };


  //COLOR
  function rgb2hex(rgb) {
    var hex= function(x) {
      var hexDigits = new Array("0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F");
      return isNaN(x) ? "00" : hexDigits[(x - x % 16) / 16] + hexDigits[x % 16];
    };
    return "#" + hex(rgb[0]) + hex(rgb[1]) + hex(rgb[2]);
  }
  
  function hex2rgb(v){
    var val=(v.charAt(0)=="#") ? v.substring(1,7):"ffffff";
    var R=parseInt(val.substring(0,2),16);
    var G=parseInt(val.substring(2,4),16);
    var B=parseInt(val.substring(4,6),16);
    return [R,G,B];
  }




})(jQuery);