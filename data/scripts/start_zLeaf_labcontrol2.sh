#!/bin/bash

# Start a zLeaf with the given arguments
if [ $# == 2 ]; then
	DISPLAY=:0.0 /usr/bin/wine /opt/z-Leaves/zTree_$1/zleaf.exe /server $2 &
	exit
fi
if [ $# == 3 ]; then
	DISPLAY=:0.0 /usr/bin/wine /opt/z-Leaves/zTree_$1/zleaf.exe /server $2 /channel $3 &
	exit
fi
if [ $# == 4 ]; then
	DISPLAY=:0.0 /usr/bin/wine /opt/z-Leaves/zTree_$1/zleaf.exe /server $2 /channel $3 /name $4 &
	exit
fi
