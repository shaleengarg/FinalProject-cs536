#!/bin/bash

debug () {
	echo "[XML.sh]$1"
}

rocksdb_print_usage () {
	echo "Usage:"
	echo " - aggregate <optional xml prefix> ... (aggregates xml files)"
	echo " - clean <optional xml prefix> ... (cleans all xml files)"
}

check_env () {
	if [ -z "$XML_EXPORT" ]; then
		debug " XML_EXPORT environment variable is undefined. Can not continue"
		exit 1
	fi
}


clean_single () {
	debug "[Clean] Cleaning up all files with prefix $1"
	
	if [ -z "$1" ]; then
		debug "[Clean] EMPTY PREFIX ENCOUNTERED. HOW DID WE GET HERE? THIS COULD'VE BEEN BAD REALLY REALLY BAD. HOW DID YOU PASS AN EMPTY STRING AS AN ARGUMENT? (BAD BAD BAD)"
		exit 1
	fi
	
	for file in $1*; do
		if [ ! -f $file ]; then
			debug "[Clean] file $file not found. Skipping..."
			continue
		fi
		debug "[Clean] Removing file $file"
		rm $file
	done
	debug "[Clean] Done cleaning up files with prefix $1"
}


clean () {
	if [ $# -eq 0 ]; then	
		debug "[Clean] No xml prefix specified. Will look at XML_EXPORT..."
		check_env
		clean_single $XML_EXPORT
	else
		for prefix in "$@"; do
			clean_single $prefix
		done
	fi
}


aggregate_single () {
	debug "[Aggregate] Going to aggregate XML files with prefix $1"

	AGG_PATH=$1.xml
	TMP_PATH=.tmp-export.xml
	debug "[Aggregate] Removing $AGG_PATH if it exists."
	rm -rf $AGG_PATH $TMP_PATH

	echo "<run name=\"$RUN_NAME\" description=\"$RUN_DESCRIPTION\">" >> $TMP_PATH

	echo "	<processes>" >> $TMP_PATH

	# aggregate
	for file in $1*.xml; do
		if [ ! -f $file ]; then
			debug "[Aggregate] file $file not found. Skipping..."
			continue
		fi
		
		debug "[Aggregate] processing file $file"
		
		sed 's/^/\t\t/' $file >> $TMP_PATH

	done

	echo "	</processes>" >> $TMP_PATH

	echo "</run>" >> $TMP_PATH

	# remove old files 
	clean_single $1
	
	# rename file
	mv $TMP_PATH $AGG_PATH


	debug "[Aggregate] Done aggregating XML files with prefix $1. Aggregated to $AGG_PATH"
}


aggregate () {
	if [ $# -eq 0 ]; then
		debug "[Aggregate] No xml prefix specified. Will look at XML_EXPORT..."
		check_env	
		aggregate_single $XML_EXPORT
	else
		for prefix in "$@"; do
			aggregate_single $prefix
		done
	fi
}


if [ $# -ne 0 ]; then
	if [ $1 == "aggregate" ]; then
		aggregate "${@:2}" 
	elif [ $1 == "clean" ]; then
		clean "${@:2}" 
	else
		debug  "Unknown command"
		print_usage
	fi
else
	debug "Incorrect # of arguments"
	print_usage
fi
