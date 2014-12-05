#!/bin/bash
CATALOGUE=Catalogue.html
BACKUP=Catalogue.html.bak

mv "$CATALOGUE" "$BACKUP"
if [ "$?" -eq 0 ]; then
	./UpdateCatalogue.py "$BACKUP" "$CATALOGUE" CSV/Inventory.csv 
	chmod 755 "$CATALOGUE"
fi
