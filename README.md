
This program takes in command line arguments of allocation size, 
allocation count, and delay time. Then it makes allocations accordingly, 
with an option of printing allocation status in the process. 
After allocation is done, the program attempts to free all allocated 
memory, provided that the system does not crash at somepoint during 
allocations.

The main purpose of this program is to test the memory behavior of any 
computer. Specifically, I wrote this program to test out the swapfile 
behaviors of my personal laptop.

For usage, type `./allocate -h`.

@author Owen Li <tianwei2@andrew.cmu.edu>
