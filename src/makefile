objects = armcollect.o stty.o meter.o global.o xmlread.o

armcollect-cross: $(objects)
	arm-linux-gcc -o armcollect-cross $(objects)  -I /usr/local/arm/4.3.2/arm-none-linux-gnueabi/include/libxml2/  -I /usr/local/arm/4.3.2/arm-none-linux-gnueabi/include/curl/  -L /usr/local/arm/4.3.2/arm-none-linux-gnueabi/lib -lxml2 -L /usr/local/arm/4.3.2/arm-none-linux-gnueabi/lib -lcurl

stty.o:stty.c stty.h
	arm-linux-gcc -c stty.c

meter.o:meter.c meter.h
	arm-linux-gcc -c meter.c

global.o:global.c global.h
	arm-linux-gcc -c global.c

xmlread.o:xmlread.c xmlread.h 
	arm-linux-gcc -c xmlread.c -I /usr/local/arm/4.3.2/arm-none-linux-gnueabi/include/libxml2/ -L /usr/local/arm/4.3.2/arm-none-linux-gnueabi/lib -lxml2

armcollect.o : armcollect.c armcollect.h
	arm-linux-gcc -c armcollect.c -I /usr/local/arm/4.3.2/arm-none-linux-gnueabi/include/libxml2/  -I /usr/local/arm/4.3.2/arm-none-linux-gnueabi/include/curl/  -L /usr/local/arm/4.3.2/arm-none-linux-gnueabi/lib -lxml2 -L /usr/local/arm/4.3.2/arm-none-linux-gnueabi/lib -lcurl


clean :
	rm armcollect-cross $(objects)
