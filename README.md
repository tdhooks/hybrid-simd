# hybrid-simd

## Purpose

Though currently incomplete, the purpose of this soon-to-be library is to use two different parallel processing libraries (openMP and openMPI) in tandem to create a cluster processing API with an interesting architecture. This is built specifically for my Raspberry Pi 4 cluster using Slurm resource management, but realistically it might run just fine on any properly configured openMPI environment with openMP.

## Structure

The idea is pretty simple. Use openMP to parallelize work across *cores*, and use openMPI to parallelize work across *nodes*. This makes sense, because openMP is a memory sharing and multiprocessing library while openMPI is a message passing interface. In practice, there isn't a whole lot of evidence that this hybrid architecture is any faster than pure openMPI, but still an interesting project nonetheless.

## Notes on Current State

* Currently, the openMPI portion does all processing on the master node. The openMP portion is multithreaded, though.
* The project currently looks a lot like an hpgm color flipper. The intent later on is to separate the multithreading logic and present a library with functions that perform operations on generic buffers, without need to worry about the implementation. Perhaps matrix or vector operations to start?
* The project is on hold because my testbed has temporarily become inactive due to some hardware constraints, and I can't really justify spending more on it than I have already.