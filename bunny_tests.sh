for k in `seq 1 3`;
do

	#Pumpkin
	./raytracer 3d_models/pumpkin.obj renders/fr_pumpkin_200_200.ppm 200 200 0 3 1 1.1
	./raytracer 3d_models/pumpkin.obj renders/fr_pumpkin_800_600.ppm 800 600 0 2.5 2 0.5
	./raytracer 3d_models/pumpkin.obj renders/fr_pumpkin_1920_1080.ppm 1920 1080 0 2.5 2 0.14


	#Teddy bear
	./raytracer 3d_models/teddy.obj renders/fr_teddy_200_200.ppm 200 200 0 3 3 1.1
	./raytracer 3d_models/teddy.obj renders/fr_teddy_800_600.ppm 800 600 0 3 3 0.5
	./raytracer 3d_models/teddy.obj renders/fr_teddy_1920_1080.ppm 1920 1080 0 3 3 0.14


	#bunnies

	obj=3d_models
	ren=renders

	for i in 5k 10k 50k;
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
