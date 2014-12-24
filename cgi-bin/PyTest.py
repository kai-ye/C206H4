#!/usr/bin/python
import cgi

form = cgi.FieldStorage()
print """Content-type: text/html

"""
if form.has_key("hiddenElem"):
	print form["hiddenElem"].value
print form
