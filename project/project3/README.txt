Jia Huang
jzh9076@nyu.edu

My work is complete and has no known bugs.

Resources: E20 manual, w3schools, google, stackoverflow, lecture and recitation slides

I decided to make a cache class and a block class because there would be multiple caches (L1 and L2) and multiple blocks. It would be easier to instantiate them as class objects rather than manually define tags, blocksizes, etc for every instance of a cache or block. The function for reading or writing from ram was also written as a method of the cache class so that it could easily access the blocksize, size, and associativity attributes of caches. This allowed me to make all the changes to project 2 in the cache class rather than my already existing code.