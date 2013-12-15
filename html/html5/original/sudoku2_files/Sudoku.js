//Ello. M.Firdaus wrote this code. He doesn't know about copyright but he just wants to be attributed =)
window.onload=function(){
var
SQUARE_SMALL=(window.location.search.substring(1)=="small")?2:3,//Easter egg. Because 4 is too much and 2 is too cute ;_;
SAMPLE_SIZE=SQUARE_SMALL*SQUARE_SMALL,
GRID_SIZE=SAMPLE_SIZE*SAMPLE_SIZE,
CELL_SIZE=50,

rp=[],cp=[],sp=[],tn=[],or=[],p={},np={},
allPoss=Math.pow(2,SAMPLE_SIZE)-1,
wrong=0,working=0,

grid=$("grid"),
input=$("input"),
showGuide=$("guide"),
newGameButton=$("new"),
blankButton=$("blank"),
limitInput=$("limitInput"),
solveButton=$("solve"),
loading=$("loading"),
control=$("control"),
reset=$("reset"),
good=$("good"),
difficulty=$("difficulty")

function gs(i)  {return tn[i]?p[tn[i]]:rp[x(i)]&cp[y(i)]&sp[cs(i)]}
function $(n)   {return document.getElementById(n)} //I miss php
function x(i)   {return i%SAMPLE_SIZE}
function y(i)   {return Math.floor(i/SAMPLE_SIZE)}
function sx(i)  {return Math.floor(x(i)/SQUARE_SMALL)}
function sy(i)  {return Math.floor(y(i)/SQUARE_SMALL)}
function cs(i)  {return sx(i)+sy(i)*SQUARE_SMALL}
function c(x,y) {return x+y*SAMPLE_SIZE}
function sc(i,j){return c(sx(i)*SQUARE_SMALL+j%SQUARE_SMALL,sy(i)*SQUARE_SMALL+Math.floor(j/SQUARE_SMALL))}
function init(p){
  if(!p){tn=[];or=[]}
  wrong=0
  for(var i=0;i<SAMPLE_SIZE;i++){
   cp[i]=allPoss
   rp[i]=allPoss
   sp[i]=allPoss
  }
}
function repro(){
  init(p)
  for(var i=0;i<GRID_SIZE;i++)
   if(tn[i]) improbable(i,tn[i])
}
function unset(i){
  tn[i]=0
  repro()
}
function improbable(i,n){
  rp[x(i)]&=~p[n]
  cp[y(i)]&=~p[n]
  sp[cs(i)]&=~p[n]
}
function set(i,n,sl){
  if(!(gs(i)&p[n]))wrong=1
  improbable(i,n)
  tn[i]=n
  if(sl==2)or[i]=n
  if(sl==1)
  for(var j=0;j<SAMPLE_SIZE;j++){
   fill(c(x(i),j))
   fill(c(j,y(i)))
   fill(sc(i,j))
  }
}
function solved(){
  if(wrong)return false
  for(var i=0;i<GRID_SIZE;i++) if(!tn[i])
  return false
  return true
}

for(var i=0;i<SAMPLE_SIZE;i++){
  p[i+1]=Math.pow(2,i)
  np[p[i+1]]=i+1
}

grid.innerHTML=(new Array(GRID_SIZE+1)).join("<div></div>")
input.innerHTML=(new Array(SAMPLE_SIZE+1)).join("<div></div>")

grids=grid.getElementsByTagName("div")
buttons=input.getElementsByTagName("div")
grid.style.width=SAMPLE_SIZE*CELL_SIZE+"px"
input.style.width=SQUARE_SMALL*CELL_SIZE+"px"

showGuide.onclick=refresh
difficulty.onchange=newGame
newGameButton.onclick=newGame
blankButton.onclick=blankGame
solveButton.onclick=function(){
  for(var i=0;i<GRID_SIZE;fill(i++));
  refresh()
}
reset.onclick=function(){
  tn=or.slice(0)
  repro()
  refresh()
}
for(var i=grids.length;i--;){
  grids[i].num=i
  grids[i].className=(sx(i)+sy(i)%2)%2?"":"dark"
  grids[i].onmousedown=function(){
    if(!working&!or[this.num]){
    if(tn[this.num]){
    unset(this.num)
    refresh()
    }  else {
    input.className=""
    input.num=this.num
    input.style.left=(x(this.num)+0.5)*CELL_SIZE+"px"
    input.style.top =(y(this.num)+0.5)*CELL_SIZE+"px"
    for(var j=0;j<buttons.length;j++)
      buttons[j].innerHTML=(!limitInput.checked|gs(input.num)&p[j+1])?j+1:"&nbsp;";
    }}
  }
}

for(var i=buttons.length;i--;){
  buttons[i].n=i+1;
  buttons[i].innerHTML=i+1
  buttons[i].onmousedown=function(){
    if(this.innerHTML!="&nbsp;"){
      set(input.num,this.n)
      refresh()
    if(solved()){
      good.className=""
      if(confirm("It is done! New game?"))newGame()
    }}
  }
}

function refresh(){
  input.className="hidden"
  good.className="hidden"
  free()
  for(var i=0;i<GRID_SIZE;i++){
   grids[i].className=((sx(i)+sy(i)%2)%2?"":"dark") + ((or[i])?" orig":"")
   grids[i].innerHTML=
    tn[i]?tn[i]:((showGuide.checked)?"<span>"+has(gs(i)).join(" ")+"</span>":"&nbsp;")
  }
}

function loadGame(start){
  init()
  for(var i=0;i<start.length;i++)
   if(start[i])set(i,start[i],2)
}

function has(gt){
  for(var i=0,str=[];i<SAMPLE_SIZE;i++)
  if(gt&p[i+1])str.push(i+1)
  return str
}

function randomNumber(gt){
  str=has(gt)
  return str[Math.floor(str.length*Math.random())]
}

function fill(i){
  if(!tn[i]){
    var gt=gs(i)
    if(np[gt]) set(i,np[gt],1);else{
    for(var j=0;j<SAMPLE_SIZE;j++){
        if(y(i)!=j)   gt&=~gs(c(x(i),j))
        if(x(i)!=j)   gt&=~gs(c(j,y(i)))
        if(sc(i,j)!=i)gt&=~gs(sc(i,j))
    }
    if(np[gt])set(i,np[gt],1)
   }}
}

function create(){
busy()
for(var i=0;i<GRID_SIZE;i++){
  if(!gs(i)){
    i=0
    init()
  } else if(!tn[i]) set(i,randomNumber(gs(i)),1)
}
free()
}

//16x16 grid is impossible without better algorithm
function unSolve(ii){
return function(){
  busy()
  var temp,temp2
  if(ii){
    temp=tn.slice(0)
    unset(Math.floor(Math.random()*GRID_SIZE));
    unset(Math.floor(Math.random()*GRID_SIZE));
    temp2=tn.slice(0)
    for(var i=0;i<GRID_SIZE;fill(i++));
    if(solved()) loadGame(temp2);else loadGame(temp)
    window.setTimeout(unSolve(--ii),0)
  } else {
   refresh()  
  }
}}

function busy(){
 working=1
 loading.className=""
 control.className="hidden"
}

function free(){
 working=0
 loading.className="hidden"
 control.className=""
}

function newGame(){
  init()
  refresh()
  create()
  unSolve(difficulty.value)()
}

function blankGame(){
  init()
  refresh()
}


newGame()
} //I'm just going to submit this now. Sorry for the mess. Thanks for reading. -M.Fir