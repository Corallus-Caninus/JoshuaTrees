# JoshuaTrees
![image](https://user-images.githubusercontent.com/41231332/171738402-4b4763b5-5491-4156-a9c3-67d02747bf6b.png)

[WIP]
tunable BST-arrays built for performance.
set chunksize as a power of two to set the slice size for each B-Tree node.
* can index any part of the array, even if unallocated.
* can set any part of the array, even if unallocated.
* worst case unused memory (if treated as a linked list e.g.: push pop operations) is always chunksize-1.
* can defragment allocations in the tree for chunks that have become empty.

Let me know what you think:
ward.joshua92@yahoo.com
