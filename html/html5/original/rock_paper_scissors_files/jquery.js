$("#f").submit(function (e) {var name1 = $('input[id=name1]').val();var name2 = $('input[id=name2]').val();var fighting = $('input[id=fighting]').val();play(name1, name2, fighting);$('.num0').removeClass('m0');$('.num1').removeClass('m1');$('.num2').removeClass('m2');$('.num0').addClass('m0');$('.num1').addClass('m1');$('.num2').addClass('m2');store(name1, name2, fighting);e.preventDefault(); });function play(name1, name2, fighting) {var R1 = Math.floor(Math.random()*3);var R2 = Math.floor(Math.random()*3);var h1 = $('#h1');var h2 = $('#h2');h1.removeClass().addClass('h');h2.removeClass().addClass('h');h1.slideDown();h1.addClass('num' + R1);h2.addClass('num' + R2); win(R1, R2, name1, name2, fighting);}function store(name1, name2, fighting) {localStorage.setItem("name1", name1);localStorage.setItem("name2", name2);localStorage.setItem("fighting", fighting);$('#ht1').html(name1);$('#ht2').html(name2);}function win(R1, R2, name1, name2, fighting) {if(R1 == R2) {$("#winner").html("<p>It's a draw, no-one is "+fighting+". Try Again!</p>"); }else if(R1 == 0 && R2 == 1) {$("#winner").html("<p>"+name1+" wins,<br /><strong> "+name2+" is "+fighting+". </strong></p>");$("#h1").addClass('green');$("#h2").addClass('red'); } else if(R1 == 0 && R2 == 2) {$("#winner").html("<p>"+name2+" wins,<br /><strong> "+name1+" is "+fighting+". </strong></p>");$("#h2").addClass('green');$("#h1").addClass('red'); }else if(R1 == 1 && R2 == 0) {$("#winner").html("<p>"+name2+" wins,<br /><strong> "+name1+" is "+fighting+". </strong></p>");$("#h2").addClass('green');$("#h1").addClass('red'); }else if(R1 == 2 && R2 == 0) {$("#winner").html("<p>"+name1+" wins,<br /><strong> "+name2+" is "+fighting+". </strong></p>");$("#h1").addClass('green');$("#h2").addClass('red'); }else if(R1 == 2 && R2 == 1) {$("#winner").html("<p>"+name2+" wins,<br /><strong> "+name1+" is "+fighting+". </strong></p>");$("#h2").addClass('green');$("#h1").addClass('red'); }else if(R1 == 1 && R2 == 2) {$("#winner").html("<p>"+name1+" wins,<br /><strong> "+name2+" is "+fighting+". </strong></p>");$("#h1").addClass('green');$("#h2").addClass('red'); } }