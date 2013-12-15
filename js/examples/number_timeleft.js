Number.prototype.timeLeft=function(){var days=Math.floor(this/86400);var hours=Math.floor((this-(days*86400))/3600);var minutes=Math.floor((this-((hours*3600)+(days*86400)))/60);var seconds=this-((days*86400)+(hours*3600)+(minutes*60));var result=new String();if((days>0)===true){result+=days+' days,';}if((hours>0)===true){result+=' '+hours+' hours, ';}if((minutes>0)===true){result+=' '+minutes+' minutes,';}if((seconds>0)){result+=' '+seconds+' seconds,';}result=result.slice(0,-1);return result;}

Number.prototype.timeLeft = function(){

    var days = Math.floor(this / 86400);

    var hours = Math.floor((this - (days * 86400)) / 3600);

    var minutes = Math.floor((this - ((hours * 3600) + (days * 86400))) / 60);

    var seconds = this - ((days * 86400) + (hours * 3600) + (minutes * 60));

    var result = new String();

    if((days > 0) === true){result += days + ' days,';}
    if((hours > 0) === true){result += ' ' + hours + ' hours, ';}
    if((minutes > 0) === true){result += ' ' + minutes + ' minutes,';}
    if((seconds > 0)){result += ' ' + seconds + ' seconds,';}

    result = result.slice(0, -1);

    return result;

}