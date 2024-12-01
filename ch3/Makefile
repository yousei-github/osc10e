# Input the object name in argument1 for build

obj-m += $(argument1) # For example, obj-m += 4_pid.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
