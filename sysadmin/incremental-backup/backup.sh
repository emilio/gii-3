#!/bin/bash

dir=/home/emilio
backup=/root/whatever
remove=0

if [ $(date +"%w" -eq 7) || ! -f $backup.0.tar ]; then
	mv "$backup.snap" "$backup.snap.bak"
	level=0
	remove=1
else
	last_level=$(ls $backup.*.tar | uniq | sort | tail -1 | rev | cut -d. -f 2)
	level=$(expr $last_level + 1)
fi

tar -g "$backup.snap" -vcf "$backup.$level.tar" "$dir"

if [ $remove -eq 1 ]; then
	find . -name $backup.*.tar -\! -name $backup.0.tar
fi
