# haikuboot
HaikuBoot is a linux based operating system which only interprets code written in Haiku (my esolang).
<<<<<<< HEAD
Its purpose is to create a standalone environment for Haiku code execution. Can be booted by any linux-compatibile environment.

HOW DOES IT WORK?
-----------------

The system has compiled newest version of Haiku Interpreter installed with all the required libs, init process starts the execution of /usr/ppm/sys.ppm, then after end of execution system is powered off.

HOW TO MAKE DISK IMAGE WITH IT
------------------------------

To create disk image with HaikuBoot which can be later burned to pendrive or disc, you need to make a raw disc image. Then partition it, root partition must be ext2/ext3/ext4 because no other fs drivers are in the kernel.
Then copy contents of src/disk to root partition, if your system is not x86_64 you will need to exchange all libs and bins to the correct version. In src/linux there is compiled kernel image and initrd copy them to boot partition and set your choosen bootloader config to load them. Also you will need a bootloader that will load the system, you can easily install grub using a virtual machine.

PREBUILT DISK IMAGE
-------------------

Prebuilt disk image (in disk-images) is 1 GB raw disk file, bootable in EFI made for x86_64, you can burn it to pendrive or other storage media. Don't forget to adjust root partition size.
=======
>>>>>>> 93e515752e5b55a7d32905fe53d0082bf9db09d7`
