set count=0
cd data2000|find "*.mdb"/c>nofiles.###
count<nofiles
if(count GTR 10)
for *.mdb in data2000
if(count>10) 
delete *.mdb
count=count-1
else
exit
end
else
exit