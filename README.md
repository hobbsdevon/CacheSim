# <div align='center'>CacheSim</div>
## Introduction
CacheSim is a program written for my senior capstone project. It started as an assignment for a class called computer organization. By the end of computer organization, it took user inputs for cache size, ram size, block size, and mapping method. For my capstone, I decided to try to implement replacement methods as well. This is still a work in progress, but as of now, FIFO is implemented with fully associative mapping.
## Functionality
If it isn't obvious, CacheSim simulates the data transfer between cache and RAM. First, it takes a user input cache size, RAM size, block size, mapping method, and replacement method. It then takes a user input memory address, and checks to see whether or not that address is already contained in the cache. It prints cache hit or miss, and if it's a cache miss, the program moves the block containing that address into the cache, replacing another block if necessary.
