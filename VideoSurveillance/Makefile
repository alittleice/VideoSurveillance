OBJS = main.o udpclient.o tcpclient.o yuv422_rgb.o

main: $(OBJS)
	arm-linux-gnueabihf-gcc $(OBJS) -o main

main.o: main.c udpclient.h yuv422_rgb.h tcpclient.h
	arm-linux-gnueabihf-gcc -c main.c -o main.o

udpclient.o: udpclient.c udpclient.h
	arm-linux-gnueabihf-gcc -c udpclient.c -o udpclient.o

tcpclient.o: tcpclient.c tcpclient.h
	arm-linux-gnueabihf-gcc -c tcpclient.c -o tcpclient.o	

yuv422_rgb.o: yuv422_rgb.c yuv422_rgb.h
	arm-linux-gnueabihf-gcc -c yuv422_rgb.c -o yuv422_rgb.o	

clean:
	rm -rf *.o main