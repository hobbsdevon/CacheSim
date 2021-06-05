# <div align='center'>CacheSimulator and CacheRamSimulator</div>
## Introduction
CacheSimulator is a program written in C as an assignment for a computer organization class. Though it certainly needs some improvements, I have decided that I would rather focus on C++ and thus, I have written CacheRamSimulator. This program has all of the same functionality as CacheSim, with added RamSim, as these usually work in tandem. Moving forward, I will be referencing the CacheRamSimulator
## Motivation
I've decided that C++ is one of the primary languages I would like to learn, and instead of improving upon CacheSim in C, I think that improving and adding to it in C++ serves as a good first project
## Functionality
If it isn't obvious, CacheRamSimulator simulates the data transfer between cache and RAM. First, it takes a user input cache size, RAM size, block size, and mapping method. It then takes a user input memory address, and checks to see whether or not that address is already contained in the cache. If not, it moves the block containing that address into the cache, removing another block if necessary. It also asks if the user modified the data to determine whether it must be copied back to the RAM, or simply discarded
