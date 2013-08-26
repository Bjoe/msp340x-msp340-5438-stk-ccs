#!/bin/sh

# This file should be executed from the command line prior to the first
# build.  It will be necessary to refresh the Eclipse project once the
# .sh file has been executed (normally just press F5 to refresh).

# Copies all the required files from their location within the standard
# FreeRTOS directory structure to under the Eclipse project directory.
# This permits the Eclipse project to be used in 'managed' mode and without
# having to setup any linked resources.

# Have the files already been copied?
if [ ! -d "FreeRTOS_Source" ]
then
	# Create the required directory structure.
	mkdir FreeRTOS_Source
	mkdir -p FreeRTOS_Source/include	
	mkdir -p FreeRTOS_Source/portable/CCS
	mkdir -p FreeRTOS_Source/portable/CCS/MSP430X
	mkdir -p FreeRTOS_Source/portable/MemMang	
	mkdir -p Demo_Source/Common_Demo_Files
	mkdir -p Demo_Source/Common_Demo_Files/include
	
	# Copy the core kernel files.
	cp ../../Source/tasks.c FreeRTOS_Source
	cp ../../Source/queue.c FreeRTOS_Source
	cp ../../Source/list.c FreeRTOS_Source
	cp ../../Source/timers.c FreeRTOS_Source
	
	# Copy the common header files

	cp ../../Source/include/* FreeRTOS_Source/include
	
	# Copy the portable layer files
	cp ../../Source/portable/CCS/MSP430X/* FreeRTOS_Source/portable/CCS/MSP430X
	
	# Copy the basic memory allocation files
	cp ../../Source/portable/MemMang/heap_1.c FreeRTOS_Source/portable/MemMang

	# Copy the files that define the common demo tasks.
	cp ../Common/Minimal/dynamic.c Demo_Source/Common_Demo_Files
	cp ../Common/Minimal/comtest.c Demo_Source/Common_Demo_Files
	cp ../Common/Minimal/GenQTest.c Demo_Source/Common_Demo_Files
	cp ../Common/Minimal/TimerDemo.c Demo_Source/Common_Demo_Files
	cp ../Common/Minimal/countsem.c Demo_Source/Common_Demo_Files
	
	# Copy the common demo file headers.
	cp ../Common/include/dynamic.h Demo_Source/Common_Demo_Files/include
	cp ../Common/include/comtest.h Demo_Source/Common_Demo_Files/include
	cp ../Common/include/comtest2.h Demo_Source/Common_Demo_Files/include
	cp ../Common/include/GenQTest.h Demo_Source/Common_Demo_Files/include
	cp ../Common/include/serial.h Demo_Source/Common_Demo_Files/include
	cp ../Common/include/partest.h Demo_Source/Common_Demo_Files/include
	cp ../Common/include/TimerDemo.h Demo_Source/Common_Demo_Files/include
	cp ../Common/include/countsem.h Demo_Source/Common_Demo_Files/include
	
fi
