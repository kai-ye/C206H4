#!/usr/bin/python
#Copies all lines up to STARTGRAB from TEMPLATE to OUTPUTFILE.
#Then, further inserts the html blocks into OUTPUTFILE for each item in CSVFILE
#Then, finds line containing ENDGRAB in TEMPLATE, and copies
#+all lines after that into OUTPUTFILE.
#################
import sys


TEMPLATE = sys.argv[1]
STARTGRAB = "<!--Update grab-->"
ENDGRAB = "<!--Update grab end-->"
OUTPUTFILE = sys.argv[2]
CSVFILE = sys.argv[3]
CSVFIELDNO = 5	#no. of fields; only last field may contain commas

class Item:
	def __init__( self, s ):
		(self.name, self.qty, self.price, self.img, self.description) = s.split(",",CSVFIELDNO-1)[0:CSVFIELDNO]

csv = open( CSVFILE, "r")
temp = open( TEMPLATE, "r")
out = open( OUTPUTFILE, "w")


line = temp.readline()
while line and ( not STARTGRAB in line ):
	out.write(line)
	line = temp.readline()
if not line:
	out.seek(0,0)
	out.truncate()
	out.write("""<p color="#CC0000">Update: Grab not found.<br></p>""")
	sys.exit(1)
out.write(line)	#Put grab back in

for line in csv:
	item = Item( line.strip() )
	out.write(
"""\
  <tr>
	<td class="catalogue">{0}</td>
    <td class="catalogue"><img src="http://www.cs.mcgill.ca/~kye/Images/{1}" width="400"></td>
    <td class="catalogue">{2}</td>
	<td class="catalogue">{3:.2f}</td>
	<td class="catalogue">{4}</td>
    <td class="catalogue">
    Purchase: <input type="checkbox" name = "BUY{0}" value = "{0}"><br/><br/>
    <input type="text" class="num" maxlength="6" name="{0}" value="0">
    </td>
  </tr>

""".format(item.name, item.img, item.description, float(item.price), item.qty)
)

line = temp.readline()
while line and ( not ENDGRAB in line ): line = temp.readline()
if not line:
	out.write("""<p color="#CC0000">Update: End grab not found.<br></p>""")
else:
	out.write(line)
	for line in temp: out.write( line )


out.close()
temp.close()
csv.close()
