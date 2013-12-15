#!/C:/Python27/python.exe

#DEFINE THE VARIABLES
page = "Home"
content = "Home page!"
#END DEFINE VARIABLES

#PRINT THE PAGE
print "content-type:text/html"
print """\
<html>
<head>
<title>
%s
</title>
</head>
<body>
%s
</body>
</html>
""" % (page, content)
#FINISHED PRINTING THE PAGE