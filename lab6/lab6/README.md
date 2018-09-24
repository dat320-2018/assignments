![UiS](http://www.ux.uis.no/~telea/uis-logo-en.png)

# Lab 6: Linux Kernel Drivers

| Lab 6:		| Linux Kernel Drivers		|
| -------------------- 	| ------------------------------------- |
| Subject: 		| DAT320 Operating Systems 		|
| Deadline:		| Nov 5 2015 23:00			|
| Expected effort:	| 20-25 hours 				|
| Grading: 		| Graded 				|
| Submission: 		| Group			|


### Table of Contents

1. [Introduction](https://github.com/uis-dat320-fall17/glabs/blob/master/lab6/README.md#introduction)
2. [Virtual Image](https://github.com/uis-dat320-fall17/glabs/blob/master/lab6/README.md#virtual-image)
3. [Loadable Kernel Module](https://github.com/uis-dat320-fall17/glabs/blob/master/lab6/README.md#loadable-kernel-module)
4. [Air Sensor](https://github.com/uis-dat320-fall17/glabs/blob/master/lab6/README.md#air-sensor)
5. [Some Useful Information](https://github.com/uis-dat320-fall17/glabs/blob/master/lab6/README.md#some-useful-information)
6. [Lab Approval](https://github.com/uis-dat320-fall17/glabs/blob/master/lab6/README.md#lab-approval)


## Introduction

This lab assignment is divided into two parts. In the first part, you will build a simple Linux
device driver, and install it on a virtual machine image. Then you will write two small programs,
one in C and one in Go, that uses this device driver. In the second part of the lab, you will
study how an operating system handles virtual and physical memory.

**Very Important: Read the entire document before you begin programming.**

**Please place all C and C++ files in a subdirectory within the lab folder.**

## Virtual Image

In some parts of this lab you are going to use a stripped down version of Linux Slackware,
which we will run from a VirtualBox virtual machine. This image does not have a GUI, and
is very minimalistic, so everything must be done directly from the shell. We are using virtual
machines in this lab, because you will need to have root access to the system when developing
device drivers.

**Starting the Virtual Machine Image**

The virtual machine image can be found at `/home/share/ide/dat320/StudentSlack.ova` on the
machines on the Unix system, e.g. one of the pitter machines. Download/copy the image to
the machine you are working on. You can run the image on Linux, Windows, and Mac hosts.
The Linux lab machines has the latest version of VirtualBox installed, but if you prefer to
use your own machine, you first need to install VirtualBox from [here]
(https://www.virtualbox.org/wiki/Downloads).

Once VirtualBox has been installed, use `File - Import appliance` inside Oracle VM VirtualBox 
Manager to import the image. Start the image and log in with the following user/pass-
word: `user/user`. You can also log in as root using: `root/root`, but only use the root account
if you absolutely need to. We recommend that you adopt the habit of using the user account,
and the `sudo` command to become root when necessary.

**Share folder between host and guest on VirtualBox**

It is possible to share folders between the host (the machine where the VirtualBox application is running on) and the virtual image.
 
You first need to set up a folder on the host (settings->shared folders) and give the share a name. 
Inside SlackWare you need to run the following command to mount the share:
 
Make a dir where you do the mount:
* `mkdir share` (or another name you choose)
* `sudo mount -t vboxsf share ~/share` (where the first 'share' is the name given in settings->shared folder, and ~/share is the directory just created)
* PS: mount is reserved to 'root' to execute. But by use of sudo we can run 'root' commands as 'user'. Use man sudo for more info.


## Loadable Kernel Module

In this first part of the lab you will develop a Loadable Kernel Module that can be dynamically
loaded into a running Linux kernel. Kernel modules runs in kernel mode (i.e., not in user mode as
normal applications does) within the kernel’s address space, and it can interact with the running
kernel in just about any way. Key OS concepts such as execution context, synchronization, and
low-level memory management come into play in the construction of a Loadable Kernel Module.

The most common use of a kernel module is in the construction of device drivers. There
are three main types of device drivers in Linux: character drivers, block drivers, and network
drivers. In this assignment we will only work with character drivers.

Character device files look just like regular files, and can be ”read from” and ”written to”
using the standard filesystem API: `open()`, `close()`, `read()`, `write()` system calls at the
granularity of an individual byte. These types of drivers are used for most devices, including
keyboards, mice, webcams, etc.

Please be aware that inside the device driver you have unrestricted access. This means that
you need to program carefully to avoid crashing the kernel (which is another reason to use a
virtual machine when developing modules). Since a kernel module has this much power, all
operations regarding the installation, removal and configuration of a kernel module must be
done by the root user, e.g. using the sudo command.

**Preparations**

Before you start developing your kernel module, you must read chapter 2 and 3 of ”Linux Device
Drivers, Third edition”. This book is free and can be found at [here](http://lwn.net/Kernel/LDD3/).
And remember that: google is your friend!

**Getting Started**

We will start with a very simple module. This module will not have any real functionality; our
objective is rather to install and to remove the module. We will use the example source code
in Listing 1 and the example makefile in Listing 2.

**Tasks**

`1.` To move files back and forth between your computer and the virtual machine image, it
is highly recommended that you share a folder on your computer with the VM. This will
allow you to edit your files on your host computer (in a GUI), and only do the actual
build on the VM.

`2.` Log into the Virtual Image as user. Copy the `lab6` folder to the VM, and ensure that it
contains a folder named `simp_lko` and that it contains the two files from Listing 1 and
Listing 2. Build the driver in this folder by running `make`. The output from the build is
the actual device driver `simp_lko.ko`.

`3.` Install the driver by executing the `insmod` command:

`sudo insmod simp_lko.ko`

Use `sudo lsmod | grep simp` to verify that the module has been loaded.

PS: `insmod`, `lsmod`, `rmmod` and `mknod` is located in `/sbin`. `/sbin` might not be added
to the path. Add to path to avoid specifying full path each time you use those commands.

`4.` Check out the code in `simp lko.c`. The `printk()` messages in the driver will be written
to a special log file. You can use the `dmesg` command to look at the messages.

`5.` Remove the driver by using the `rmmod` command.

`6.` Use `dmesg` to verify that the remove message is written.

**Questions**
```
1.) Where are the "linux/kernel.h" and "linux/module.h" files located?
	a. /usr/sbin
	b. /usr/share/Modules/modulefiles
	c. /usr/src/kernels/<kernel-release>/drivers/linux/
	d. /usr/src/kernels/<kernel-release>/include/linux/

2.) The makefile for a driver is only an entry for calling another subset of makefiles.
What is the location of the makefile that actually performs the build?
	a. /lib/modules/<kernel-release>
	b. /lib/modules/<kernel-release>/build
	c. /lib/<kernel-release>
	d. /lib/<kernel-release>/build

3.) How does the make file (in simp_lko) know where to find the the other make file?
	a. -C $(KDIR)
	b. M=$(PWD)
	c. It is always in the same place.
	d. The PATH environment variable.

4.) A kernel module must use printk() for debugging. Why can’t printf() be used?
	a. printk() overrides printf().
	b. printk() is a kernel function while printf() is a c library function.
	c. Both printk() and printf() can be used in a kernal module.
	d. printk() calls printf().

5.) What happens when the MODULE_LICENSE is missing from the module code?
	a. The module fails to compile.
	b. The module fails to load.
	c. The kernal complains about the module missing a license.
	d. Nothing.
```

Listing 1: Simple kernel module.
```
/*****************************************************************************

DESCRIPTION: A very simple LKM

*****************************************************************************/

/*-------------------- I n c l u d e F i l e s -------------------------*/

#include <linux/module.h> /* Needed by all modules */
#include <linux/kernel.h> /* Needed for KERN_INFO */

/*-------------------- C o n s t a n t s ---------------------------------*/

#define DEV_NAME "simplkm"

/*-------------------- T y p e s -----------------------------------------*/

/*-------------------- V a r i a b l e s ---------------------------------*/

/*-------------------- F u n c t i o n s ---------------------------------*/

int init_module(void)
{
	printk(KERN_INFO "Hello world! I’m %s and I’m being installed!\n",DEV_NAME);
	/*
	* A non 0 return means init_module failed; module can’t be loaded.
	*/
	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "Goodbye world, the %s lko is being removed.\n", DEV_NAME);
}

MODULE_AUTHOR("Morten Mossige, University of Stavanger");
MODULE_DESCRIPTION("Simple Linux devicedriver");
MODULE_LICENSE("GPL");
```
C

Listing 2: Makefile for a simple kernel module.
```
obj-m := simp_lko.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
```
make

**Extending the Module with read()**

Now we are going to extend our simple kernel module with some functionality. First we will
add support for `read()`. There is a working version of a driver on github in the `simp_read_s`
folder. However, its functionality is not as we would like to have it.

**Tasks**

`1.` Make sure that your `lab6` folder contains the `simp_read_s` folder pulled from github.
Build the new driver and install it using `insmod`.

`2.` To attach the driver to the filesystem we must do an additional step using the `mknod`
command. Details on how to use the `mknod` command can be obtained by reading log
produced by the `dmesg` command after the driver has been installed with `insmod`.

`3.` Next, we must also take care of access rights for the new drivers we create. The user
account on the VM image belong to the group `wheel` and this group must be given read
and write access to the device file `simp_read` as follows:
```
sudo chgrp wheel /dev/simp read
sudo chmod 664 /dev/simp read
```

`4.` Now you can read from the driver by executing the command: `cat /dev/simp_read`. The
output should be many repeating lines with the text Number of driver read=????. Hit
Ctrl-C to stop the cat command’s output from the driver.

`5.` The small Python script given in Listing 3 can be used to access the the driver from user
space. The script is also in the corresponding folder pulled from github. Try it!

Listing 3: Userspace python script to access a devicedriver
```
# A small userspace application i python for accessing the
# simp_read devicedriver
a=file(’/dev/simp_read’)
b=a.readline()
print ’This is what I read from the driver:’, b
```
python

`6.` Write a Go program that replicates the behavior of the Python script in Listing 3.

`7.` Rewrite the driver so that it produces the following output:
```
% cat /dev/simp_read
Number of driver reads=1
% cat /dev/simp_read
Number of driver reads=2
% cat /dev/simp_read
Number of driver reads=3
...
```

`8.` Write a C program called `userread` that replicates the last task. The output
should be as follows:
```
% ./userread
Number of driver reads=1
% ./userread
Number of driver reads=2
% ./userread
Number of driver reads=3
...
```

**Extend the Module with write()**

We will now continue and expand our driver module to support both `read()` and `write()`. We
will also use this driver from Go. Our new driver should now behave as a small mailbox. It
should be possible to write short messages to the mail box, and later read the message back.
Below is shown the expected behavior of an interaction with our driver.


| Command, Output	|	Description	|
| --------------	| -----------------	|
| % echo hello > /dev/simp rw	|	Writing a message	|
| % cat /dev/simp rw	|	Read back	|
| hello	|	The displayed result	|
| % cat /dev/simp rw	|	Try to read once more	|
|	|	No message left to read	|
| % echo msg1 > /dev/simp rw	|	Writing a message	|
| % echo msg2 > /dev/simp rw	|	Writing a message	|
| % cat /dev/simp rw	|	Read back	|
| msg2	|	msg1 is lost	|
| % cat /dev/simp rw	|	Try to read once more	|
|	|	No message left to read	|

**Tasks**

`1.` Extend the driver to support `write()` to a message box as illustrated with the
above execution example.

`2.` Create a user-space Go program called `msgbox` that accesses the driver and can
be used in the same way as the `cat` and `echo` commands above. That is, it should provide
the following command line interface:


| Command, Output	|	Description	|
| --------------	| -----------------	|
| % msgbox hello	|	Writing a message	|
| % msgbox	|	Read back	|
| hello	|	The displayed result	|
| % msgbox	|	Try to read once more	|
|	|	No message left to read	|
| % msgbox msg1	|	Writing a message	|
| % msgbox msg2	|	Writing a message	|
| % msgbox	|	Read back	|
| msg2	|	msg1 is lost	|
| % msgbox	|	Try to read once more	|
|	|	No message left to read	|

## Air Sensor

In this task we simulate calibration of an air pressure sensor. Before we can use the sensor, 
we need to calibrate the sensor. We use linear first order best fit to find a best approximation 
of a linear function `y = ax + b` where `a` is the gain and `b` is the offset.

We also have a high presision air supply available which we can control through SetOutputRefValue().

**Tasks:**

`1.` The first task is to implement `dev_read` and `dev_write` in the `airtune.c` file provided. 
The files are located in the `air_sensor` directory. You should not need to change the other files 
in the directory. `AirFlowSensor.h` shows you how to call the methods located in `AirFlowSensor.o_shipped`.
The implementation of AirFlowSensor is hidden.
```
static ssize_t dev_read (struct file *file, char *buf, size_t count, loff_t *ppos) 
{
	// Read can only return the current value of the air preasure sensor.
}

static ssize_t dev_write (struct file *file, const char *buf, size_t count, loff_t *ppos) 
{
	// Sets the gain, offset, or reference value.
}
```
These methods will call the methods listed in AirFlowSensor.h.
```
int SetOutputRefValue(int val);
int SetSensorGain(int val);
int SetSensorOffset(int val);
int ReadSensor(void);
```

This should more or less complete the driver. When you are finished, you can build the driver with the `Makefile`.

`2.` Make a userspace program which will `read` and `write` to the sensor using the driver.

Here are some examples of what could be written to the driver.

When writing a reference value: `R=<some integer value>`

When writing gain: `G=<some integer value>`

When writing offset: `O=<some integer value>`

The reference value should be between 0 and 10 (times a scale of 1000). The gain should be between 0 and 2 (times a scale of 1000). The offset should be between -2 and 2 (times a scale of 1000).

`3.` Implement the linear best fit algorithm for finding values for the gain and offset. 
Some theory behind this algorithm can be found in [linreg.pdf] (http://www.pgccphy.net/Linreg/linreg.pdf). Equations 1 and 2 in the pdf can be used to find the gain and offset. Due to the noise, the actuator must be read many times and the average calculated.

![Gain](https://github.com/uis-dat320-fall17/glabs/blob/master/lab6/air_sensor/gain.png)
![Offset](https://github.com/uis-dat320-fall17/glabs/blob/master/lab6/air_sensor/offset.png)

Start by setting the gain to 1 (times a scale of 1000) and the offset to 0 (times a scale of 1000). For a set of reference values, 0 through 10 (times a scale of 1000), get the sensor reading a multiple number of times and determine the average sensor reading (due to noise) for a reference value. When you have the set of averages, then apply the best fit equations to get a new gain and offset.

`4.` What is the value of the gain?

`5.` What is the value of the offset?

**Note: The driver interface only work with integer math. All numbers are multiplied by 1000 and cast to an integer.**

![Overall Setup](https://github.com/uis-dat320-fall17/glabs/blob/master/lab6/air_sensor/OverallSetup.jpg)


## Some Useful Information

**dmesg** 	The `dmesg` command shows messages in kernel ring buffers. These messages contain
valuable information about device drivers loaded into the kernel when booting as well as when
connecting a hardware device to the system on the fly. In other words `dmesg` will give us
details about hardware drivers connected to or disconnected from a machine and any errors
when hardware driver is loaded into the kernel. These messages are helpful in diagnosing and
debugging hardware and device driver issues. [See also]
(http://www.linuxnix.com/2013/05/what-is-linuxunix-dmesg-command-and-how-to-use-it.html).

**insmod** 	The `insmod` command tries to insert a module into the running kernel by resolving
all symbols from the kernel’s exported symbol table.

**mknod** 	The `mknod` command creates device special files. This is the command that allows us
to attach a kernel module with the Linux file system.

**screen** 	The `screen` utility is a full-screen window manager that multiplexes a physical terminal 
between several processes (typically interactive shells). The same way tabbed browsing revolutionized 
the web experience, screen can do the same for your experience in the command line.
Instead of opening up several terminal instances on your desktop, screen can do it better and
simpler. [See also](http://www.cyberciti.biz/tips/linux-screen-command-howto.html).

An overview of some useful screen commands:

**Ctrl+a c**	Creates a new screen session so that you can use more than one screen session at once.

**Ctrl+a n**	Switches to the next screen session (if you use more than one).

**Ctrl+a p**	Switches to the previous screen session (if you use more than one).

**Ctrl+a d**	Detaches a screen session (without killing the processes in it - they continue).


## Lab Approval

To have your lab assignment approved, you must come to the lab during lab hours
and present your solution. This lets you present the thought process behind
your solution, and gives us more information for grading purposes. When you are
ready to show your solution, reach out to a member of the teaching staff. It
is expected that you can explain your code and show how it works. You may show
your solution on a lab workstation or your own computer. The results from
Autograder will also be taken into consideration when approving a lab. At least
60% of the Autograder tests should pass for the lab to be approved. A lab needs
to be approved before Autograder will provide feedback on the next lab
assignment.

Also see the [Grading and Collaboration
Policy](https://github.com/uis-dat320-fall17/course-info/blob/master/policy.md)
document for additional information.
