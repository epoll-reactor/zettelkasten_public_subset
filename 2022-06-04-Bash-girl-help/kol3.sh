#!/bin/bash

while true; do
	read -r -p "Please hit the button (P/D/K): " -n1 button
	case $button in
		[Pp]* ) find /var/log ! -user root | wc -l ;;
		[Dd]* ) find /usr/lib -type l ;;
		[Kk]* ) break ;;
	esac
done
