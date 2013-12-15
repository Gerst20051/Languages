#!C:/Python27/python.exe

#DEFINE VARIABLES
page = "Home"

content = """\
This is a multi-line text example.
Somehting like a menu could be placed here, and then substituted into the program later on.
A menu created in this manner would be much easier to manage because it could be changed in a single place rather than on every page.
 So that's pretty much it!
 """

#END DEFINE VARIABLES

#PRINT PAGE CONTENT
print "content-type:text/html"
print """\
<html>
    <head>
        <title>
             %s
        </title>
     <head>
     <body>
         %s
     </body>
</html>
""" % (page, content)
#END PRINT PAGE CONTENT