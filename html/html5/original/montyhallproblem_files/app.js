var card_right;var card_clicked;var card_wrong;var card_change;var done;var counter=Array(0,0,0,0,0);var text_message=Array("You changed your guess and you were right!","You kept your guess and you were right!","You changed your guess and you were wrong!","You kept your guess and you were wrong!");$(document).ready(init);function init(){$("#card_one").click(function(){click(1)});$("#card_two").click(function(){click(2)});$("#card_three").click(function(){click(3)});$("#card_restart").click(restart);$("#show_more").click(function(){$("#basic_about").slideUp();$("#about").slideDown()});$("#show_less").click(function(){$("#about").slideUp();$("#basic_about").slideDown()});$("#about").hide();load_from_storage();change_page_status();restart()}function restart(){$(get_element_by_index(card_right)).removeClass("right");$(get_element_by_index(card_wrong)).removeClass("wrong");$(get_element_by_index(card_clicked)).removeClass("clicked");$("#message span").text("");$("#message").slideUp();card_right=Math.floor(Math.random()*3)+1;card_wrong=0;card_clicked=0;card_changed=0;done=false}function click(card_number){if(card_number==card_wrong||done)return;$(get_element_by_index(card_clicked)).removeClass("clicked");if(card_clicked>0)if(card_clicked!=card_number)card_changed=1;card_clicked=card_number;$(get_element_by_index(card_clicked)).addClass("clicked");if(card_wrong<1)delete_random_card();else{$(get_element_by_index(card_right)).addClass("right");var event;if(card_clicked==card_right){if(card_changed)event=0;else event=1}else{if(card_changed)event=2;else event=3}counter[event]++;counter[4]++;change_page_status();save_to_storage();$("#message span").text(text_message[event]);$("#message").slideDown();done=true}}function change_page_status(){$("#trc").text(counter[0]);$("#tkc").text(counter[1]);$("#tp").text(counter[4]);var p1=((counter[0]+counter[2])==0?0:Math.round(counter[0]*100/(counter[0]+counter[2])));$("#pc1").text(p1+"%");var p2=((counter[1]+counter[3])==0?0:Math.round(counter[1]*100/(counter[1]+counter[3])));$("#pc2").text(p2+"%")}function save_to_storage(){for(i=0;i<5;i++){localStorage.setItem("counter"+i,counter[i])}}function load_from_storage(){for(i=0;i<5;i++){counter[i]=localStorage.getItem("counter"+i);counter[i]=counter[i]==null?0:parseInt(counter[i])}}function get_element_by_index(index){switch(index){case 1:return"#card_one";case 2:return"#card_two";case 3:return"#card_three"}}function get_array(){switch(card_right){case 1:return Array(2,3);case 2:return Array(1,3);case 3:return Array(1,2)}}function delete_random_card(){random_number=Math.floor(Math.random()*2);card_ops=get_array();if(card_ops[random_number]!=card_right&&card_ops[random_number]!=card_clicked){$(get_element_by_index(card_ops[random_number])).addClass("wrong");card_wrong=card_ops[random_number]}else{card_ops.splice(random_number,1);$(get_element_by_index(card_ops[0])).addClass("wrong");card_wrong=card_ops[0]}}
