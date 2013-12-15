var bX,bY,size,winList,timeLoop,cells;
var startTimer = false;
var startTime,endTime;
var win = false;
var orient = -1;
var d = document;

function drawTable(s) {
	size = (s < 2) ? 2:s;
    cells = new Array(0);
	var table = "<table>";
	for(var i=0;i<size;i++) {
		table += "<tr>";
		for(var j=0;j<size;j++) {
			table += '<td ';
			table += ((i*size+j+1)%2!=0) ? 'class="red"':'';
			table += 'onMouseDown="clickHandle(this)" id="'+ i +"."+ j +'">'+ (i*size+j+1) +'</td>';
			cells[cells.length] = ((i*size)+j+1);
		}
		table += "</tr>";
	}
	table += "</table>";
	d.getElementById("grid").innerHTML = table;
	d.getElementById((size-1)+"."+(size-1)).innerHTML = " ";
	d.getElementById((size-1)+"."+(size-1)).className = "blank";
	bX = size-1;
	bY = size-1;
	clearTimeout(timeLoop);
	d.getElementById("timer").value = "- Scramble It! -";
	d.title = size*size-1 +" Square";
	d.getElementById("record").innerHTML = (records != null && size-2 < records.length && records[size-2] != "*") ? "Chris' Score: "+records[size-2]+"<br>\n":"";
}
function scramble() {
	cells = new Array();
	for(var i=0;i<size*size;i++)
		cells[cells.length] = i+1;
	winList = cells.toString();
	for(var i=0;i<cells.length;i++) {
		cells[cells.length-1] = cells[i];
		var rand = (Math.floor(Math.random()*(size*size)));
		cells[i] = cells[rand];
		cells[rand] = cells[cells.length-1];
	}
	cells[cells.length-1] = 0;
	var permu = 0;
	for(var i=0;i<cells.length-1;i++) {
		for(var j=i+1;j<cells.length-1;j++) {
			permu += (cells[i]>cells[j]) ? 1:0;
		}
	}
	if(permu%2 == 1) {
		cells[cells.length-1] = cells[cells.length-3];
		cells[cells.length-3] = cells[cells.length-2];
		cells[cells.length-2] = cells[cells.length-1];
	}
	for(var i=0;i<size;i++) {
		for(var j=0;j<size;j++) {
			d.getElementById(i +"."+ j).innerHTML = cells[i*size+j];
			d.getElementById(i+"."+j).className = (cells[i*size+j]%2!=0) ? 'red':'';
		}
	}
	d.getElementById((size-1)+"."+(size-1)).innerHTML = " ";
	d.getElementById((size-1)+"."+(size-1)).className = "blank";
	bX = size-1;
	bY = size-1;	
	startTimer = true;
	d.getElementById("timer").value = "-Ready When You Are-";
	win = false;
}
function clickHandle(cell) {
	var cellY = Math.floor(cell.id);
	var cellX = Math.round((parseFloat(cell.id)-cellY)*10);
	if(cellY == bY || cellX == bX)
		moveBlank(cellX,cellY);
}
function keyHandle(e)
{
	switch(e.which)	{
		case 37:
			if(bX-orient > -1 || bX-orient < size)
				moveBlank(bX-orient,bY);
			break;
		case 38:
			if(bY-orient > -1 || bY-orient < size)
				moveBlank(bX,bY-orient);
			break;
		case 39:
			if(bX+orient > -1 || bX+orient < size)
				moveBlank(bX+orient,bY);
			break;
		case 40:
			if(bY+orient > -1 || bY+orient < size)
				moveBlank(bX,bY+orient);
	}
}
function invertKeys()
{
	orient *= -1;
}
function timer()	{
	var tr=d.getElementById("timer");
	if(!(win || startTimer)) {
		endTime = new Date();
		var time = endTime.getTime()-startTime.getTime();
		var M = Math.floor(time/60000);
		time = ((time%60000)/1000).toFixed(2);
		time = (M>0 && time<10) ? "0"+time:time;
		tr.value = (M>0) ? M +":"+ time +" minutes":time+" seconds";
		timeLoop = setTimeout("timer();",41);
	}
	else {
		if(win) {
			var timeO = endTime.getTime()-startTime.getTime();
			var M = Math.floor(timeO/60000);
			var time = ((timeO%60000)/1000).toFixed(2);
			time = (M>0 && time<10) ? "0"+time:time;
			tr.value = (M>0) ? "- "+ M +":"+ time +" minutes -":"- "+ time+" seconds -";
			d.getElementById((size-1)+"."+(size-1)).innerHTML = ":)";
		}
	}
}
function moveBlank(cellX,cellY) {
	if(cellX == bX) {
		var delta = (Math.abs(cellY-bY)/(cellY-bY));
		while(cellY != bY) {
			slideTile(bX,bY+delta);
			bY+=delta;
		}
	}
	else {
		var delta = (Math.abs(cellX-bX)/(cellX-bX));
		while(cellX != bX) {
			slideTile(bX+delta,bY);
			bX+=delta;
		}
	}
	if(startTimer) {
		startTime = new Date();
		startTimer = false;
		timer();
	}
}
function slideTile(cellX,cellY)	{
	var cell = cellY*size+cellX;
	var blank = bY*size+bX;
	cells[blank] = cells[cell];
	cells[cell] = cells.length;
	d.getElementById(cellY+"."+cellX).innerHTML = " ";
	d.getElementById(cellY+"."+cellX).className = "blank";
	d.getElementById(bY+"."+bX).innerHTML = cells[blank];
	d.getElementById(bY+"."+bX).className = (cells[blank]%2!=0) ? 'red':'';
	win = (cells.toString()==winList);
}