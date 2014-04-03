#!/bin/bash
make clean
make

if [ -a RayTracer ]
then
    echo "Preping…"
	if [ -d tmp ]
	then
      rm -R tmp
	fi

    mkdir tmp

	./RayTracer

#	echo "Converting images…"
#	cd tmp
#	for file in *.bmp
#	do
#		convert $file ${file%.bmp}.png
#	done
#	cd ..

#	ffmpeg -i tmp/%05d.bmp output.mp4 -y
	
	echo "Done!"
fi