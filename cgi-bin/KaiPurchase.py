#!/usr/bin/python
import sys
import cgi
import cgitb

cgitb.enable()

LOGGEDINCSV = "../CSV/LoggedIn.csv"
INVENTORYCSV = "../CSV/Inventory.csv"
INVFIELDNO = 4	#Number of fields in Invetory csv; only last field may have commas
CATALOGUE = "../Catalogue.html"
MSGGRAB = "<!--Message grab-->"
HIDDENGRAB = "<!--C hidden input grab-->"

#Helper class for storing item info
class Item:
	def __init__( self, s ):
                (self.name, self.qty, self.price, self.description) = s.split(",",INVFIELDNO-1)[0:INVFIELDNO]
##########
def main():
	form = cgi.FieldStorage()

	#Check log in status
	if not form.has_key( "username"):
		exitWith( "Log in first.")	#If not logged-in, message and exit
	IN = open( LOGGEDINCSV, "r")
	userName = form.getvalue( "username")
	if not userName+"\n" in IN:
		exitWith( "You have already logged out. Please log in.")
	IN.close()

	#Read in the inventory
	IN = open( INVENTORYCSV, "r")
	inventory = {}
	line = IN.readline()
	while line:
		item = Item( line.strip() )
		inventory[ item.name ] = item
		line = IN.readline()
	IN.close()

	#See what is to be purchased
	purchase = {}
	for inputName in form.keys():
		if inputName[:3] == "BUY":	#If this is a checkbox input
			itemName = inputName[3:]#Name of item to be purchased
			if not itemName in inventory.keys():
				refreshWith( userName, "%s is no longer available.<br/><br/>" % itemName)
			#else
			quantity = form.getvalue( itemName)#quantity input
			if not quantity.isdigit():
				refreshWith( userName, "Invalid amount for %s.<br/><br/>" % itemName)
			#else
			quantity = int( quantity )
			availableQty = int( inventory[ itemName ].qty )
			if quantity > availableQty:
				refreshWith( userName, "%d is more than we have for %s. We currently only have %d<br/><br/>" % (quantity, itemName, availableQty) )
			#else
			if quantity > 0:
				inventory[ itemName].qty = str( availableQty - quantity)#subtract from inv
				purchase[ itemName ] = ( quantity, float( inventory[ itemName ].price) )
	
	if not purchase:	#If no purchase
		refreshWith( userName, "You have ordered zero items.<br/>Remember to check the boxes for the items to be purchased.<br/><br/>" )

	#All went well. Update inventory csv.
	OUT = open( INVENTORYCSV, "w")
	for item in inventory.itervalues():
		OUT.write(",".join(( item.name, item.qty, item.price, item.description )) + "\n")
	OUT.close()

	displayBill( purchase )



##########
def exitWith( message):
	"""Print html, with message, then exit with 0."""
	print ("""Content-type:text/html

<html>
	<head>
		<link rel="stylesheet" type="text/css" href="http://www.cs.mcgill.ca/~kye/style1.css"/>
		<title>Transaction failed</title>
	</head>

	<body>
        <center>
             <table class="menu">
                <tr>
                    <td><a href="http://www.cs.mcgill.ca/~kye/index.html">Home</a></td>
                    <td><a href="http://www.cs.mcgill.ca/~kye/Catalogue.html">Catalogue</a></td>
                    <td><a href="http://www.cs.mcgill.ca/~kye/Login.html" target="_blank">Login</a></td>
                    <td><a href="http://www.cs.mcgill.ca/~kye/Registration.html">Register</a></td>
                </tr>
            </table>

			<br/><br/><br/><br/><br/><br/>

			<p>%s</p>
		</center>	
	</body>
</html>
""" % ( message) )
	sys.exit(0)



##########
def refreshWith( user, message):
	"""Refresh Catalogue page, with message, then exit with 0."""
	print "Content-type:text/html\n\n"
	HTML = open( CATALOGUE, "r")
	line = HTML.readline()
	while line:
		if MSGGRAB in line:
			print '<b class="highlight1">'+message+"</b>"
		if HIDDENGRAB in line:
			print """<input type="hidden" name="username" value="%s">""" % user
		print line,
		line = HTML.readline()
	HTML.close()
	sys.exit(0)
##########
def displayBill( purchase ):
	print """Content-type:text/html

<head>
<link rel="stylesheet" type="text/css" href="http://www.cs.mcgill.ca/~kye/style1.css"/>

<title>Transaction successful</title>
</head>

<body>
<center>
    <table class="menu">
        <tr>
            <td><a href="http://www.cs.mcgill.ca/~kye/index.html">Home</a></td>
			<td><a href="http://www.cs.mcgill.ca/~kye/Catalogue.html">Catalogue</a></td>
            <td><a href="http://www.cs.mcgill.ca/~kye/Login.html" target="_blank">Login</a></td>
            <td><a href="http://www.cs.mcgill.ca/~kye/Registration.html">Register</a></td>
        </tr>
	</table>

	<br/><br/><br/>
	<h1>Transaction successful.<br/>
		Summary
	</h1>
	<br/><br/><br/>
	
	<table class="input-boxes">
	<tr>
	<td>{0}</td>
	<td>{1}</td>
	<td>{2}</td>
	<td>{3}</td>
	</tr>\n""".format("Items","Amount","Unit price","Subtotal")
	
	total = 0
	for key in purchase.keys():
		amount = purchase[ key ][0]
		price = purchase[ key ][1]
		total+= amount* price
		print """\
	<tr>
	<td>{0:30s}</td>
	<td>{1:^10d}</td>
	<td>{2:^15.2f}</td>
	<td>{3:^15.2f}</td>
	</tr>\n""".format(key,amount,price,amount*price)
	
	print """\
	<tr>
	<td colspan="4"><hr/>Total: %.2f</td>
	</tr>
	</table>
</center>
</body>""" % total



##########
if __name__ == "__main__":
	main()
