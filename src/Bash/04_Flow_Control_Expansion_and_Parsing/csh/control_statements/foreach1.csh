#!/bin/csh

foreach i ( *.JPG )
    mv "${i}" `echo ${i} | sed 's/\.JPG$/.x/'`
    mv `echo ${i} | sed 's/\.JPG$/.x/'` `echo ${i} | sed 's/\.JPG$/.jpg/'`
end

