#!/bin/bash 

function users_count() {
	echo `who | tr -s ' ' | cut -d ' ' -f 1 | grep "[0-9]" | wc -l`
}

export -f users_count

watch -n 1 users_count
