Incitatus-OS
============

Stage 3 Dissertation project. The aim is to create an X86-32bit operating system as a vehicle to learn about operating systems from a student perspective.

##Running the OS

	› The simplest and most risk-free way is to run it on emulators such as VirtualBox, Qemu or Bochs
		› Boot from "image.iso"(inside bin folder) CD image.

	› Or, you may test-run(at your own risk) the OS on a 32-bit or 64-bit X86 PC.
		› Burn "image.iso" and boot from CD.

##Progress

![ScreenShot](http://oi49.tinypic.com/bje97s.jpg)

##Objectives

	› Modular
		› Orthogonal
		› Dependency control
		› Module Interfaces
	› Extensible
		› Driver interface
		› Allow user-space development (through a minimal C library)
	› Unambigous code design
		› Simplicity over complexity
		› Object oriented design
		› Unit testing
	› Clear and concise documentation
		› Focus on 'Why?' more than 'What?'

##Short-term tasks

	✔ Setup development environment
		 ✔ OS
			Linux 3.2.0 X86-64bit
		 ✔ Cross-compiler toolchain
			Binutils 2.22
			GCC-i586-elf 4.6.3
		 ✔ Editor-Debugger
			Sublime Text v2
			GDB 7.4

	 Programming
		 ✔ Rudimentary debugging functions, screen output
		 ✔ Module manager
		 ✔ Global descriptor table
		 ✔ Interrupt descriptor table
		 ☐ Interrupt service routines
		 ☐ Physical memory management
		 ☐ Virtual memory management
		 ☐ Context switching, multi-tasking
		 ☐ Heap memory management
		 ☐ Driver Interface

##License

	Copyright 2012 Ali Ersenal. Licensed under WTFPL V2.



