var RotateImage =
{
  slides : new Array(),
  currentSlide : new Number(0),
  int : new Object(),

  init : function()
  {  
    i = 0;
    slide = $("rotating_img_" + i);
    
    while(slide)
    {
      RotateImage.slides[i] = slide;
      slide = $("rotating_img_" + ++i);
    }
    
    if(RotateImage.slides.length > 1) // first image will already be visible, no need to rotate
    {
      RotateImage.showController();
      RotateImage.int = setInterval("RotateImage.Rotate()", 5000);
    }
    return;
  },
    
  showController : function()
  {
    var controller = $('playPauseController');
    if(controller)
    {
      controller.style.display = "block";
      
      var play = $('play');
      var pause = $('pause');
      var next = $('next');
      var prev = $('previous');
      
      if(play && pause && next && prev)
      {
        next.removeClassName('disabled');
        $('next').observe('click',
          function()
          {
            clearInterval(RotateImage.int);
            RotateImage.Rotate();
            if(pause.style.display == 'block') RotateImage.int = setInterval("RotateImage.Rotate()", 5000);
          });
        
        prev.removeClassName('disabled');
        $('previous').observe('click',
          function()
          {
            clearInterval(RotateImage.int);
            RotateImage.RotateReverse();
            if(pause.style.display == 'block') RotateImage.int = setInterval("RotateImage.Rotate()", 5000);
          });

        Event.observe(play, 'click', function(){ RotateImage.Play(); });
        Event.observe(pause, 'click', function(){ RotateImage.Pause(); });
      }
    }
  },
  
  Rotate : function()
  {
    Effect.Fade(RotateImage.slides[RotateImage.currentSlide], { duration:1, from:1.0, to:0.0 });
    RotateImage.currentSlide++;

    if(RotateImage.currentSlide >= RotateImage.slides.length){ RotateImage.currentSlide = 0; } // reset the rotation
    Effect.Appear(RotateImage.slides[RotateImage.currentSlide], { duration:1, from:0.0, to:1.0 });
  },
  
  RotateReverse : function()
  {
    Effect.Fade(RotateImage.slides[RotateImage.currentSlide], { duration:1, from:1.0, to:0.0 });
    RotateImage.currentSlide--;

    if(RotateImage.currentSlide < 0){ RotateImage.currentSlide = RotateImage.slides.length-1; } // reset the rotation
    Effect.Appear(RotateImage.slides[RotateImage.currentSlide], { duration:1, from:0.0, to:1.0 });
  },
  
  Play : function()
  {    
    $('play').style.display = "none";
    $('pause').style.display = "block";
    
    RotateImage.Rotate();
    RotateImage.int = setInterval("RotateImage.Rotate()", 5000);
  },
  
  Pause : function()
  {
    clearInterval(RotateImage.int);
    $('play').style.display = "block";
    $('pause').style.display = "none";    
  }
}
addEvent(window, 'load', RotateImage.init);