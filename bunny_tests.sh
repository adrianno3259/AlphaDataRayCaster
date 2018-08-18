#!/bin/bash
obj=3d_models
ren=renders

for k in `seq 1 3`;
do
	for i in 2k 10k 50k;
	do
		echo $i;
		prefix=fr_bunny_
		sufix=_200_200.ppm
		./raytracer $obj/bunny/bunny_$i.obj $ren/$prefix$i$sufix 200 200 0 2.5 2.5 0.9
		sufix=_800_600.ppm
		./raytracer $obj/bunny/bunny_$i.obj $ren/$prefix$i$sufix 800 600 0 2.5 2.5 0.4
		sufix=_1920_1080.ppm
		./raytracer $obj/bunny/bunny_$i.obj $ren/$prefix$i$sufix 1920 1080 0 2.5 2.5 0.15
	done
done
