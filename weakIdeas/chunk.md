# A chunk is a struct with an arbitrary number of trailing Flexible Array Members.

The aim of the `chunk` structure is to allow the programmer to group related data in a single heap allocation.
Chunk structures may not be declared directly; the compiler will only allow pointer declarations, and all operations will be in terms of chunk pointers.

At runtime, it will save bytewise indices from the end of the static portion of the struct.
These will mostly be used by compiler-generated code for finding each array when requested, but they will also be available through operators (in a similar vein to `sizeof`).
This is possible manually using a single trailing FAM and partitioning it carefully, but alignment is the major foot gun in this case.
There will be some syntactical operation for initializing a new chunk pointer, passing the size of each member. Not sure about this yet.
chunks will handle this alignment automatically; that is their main strength.

Its most well-suited application is structs which have an amount of associated data which is dynamic in size at creation, but static in size over struct lifetime. Using them for structs with growing data may be a mistake of implementation.

However, the compiler will also offer an operator for resizing its arrays at runtime dynamically. This does *not* resize the block of memory the chunk sits on. The compiler will simply change the offsets saved by the chunk and copy memory so that everything aligns the same way as before.

There are a few different ways to store indices, and they all have tradeoffs.

## bare pointers
Storing full pointers to the FAM segments is the fastest way to index them at runtime, but it is the most wasteful. In most cases, a 16-bit integer as an index from the end of the struct will be plenty and a 32-bit integer will be overkill.

## full bytewise indices
Storing full indicies in absolute bytes to the FAM segments is a way to index directly to each FAM segment with only one addition and one dereference. However, calculating the capacity of a given array (a utility the programmer must have access to) becomes an O(n) runtime operation with the number of FAM segments. The number of FAM segments should not be excessively large, so this isn't a deal-breaker, but it should be considered.

## composite indices
Storing indices where each index refers to offset from the *last* FAM segment allows quick runtime computation of array capacity. However, it pushes the O(N) calculation to each segment offset calculation.

## cache both
Storing both composite indices (essentially array capacities) and full bytewise indices requires more space but allows both getting the position of an array and getting its capacity in one operation.

The compiler will be required to provide the capacity *and* offset of each segment through operators which will invoke runtime calculations. It may choose which of the above approaches to use for storing the indices of each segment. It may also be extended so that the user can select one manually, but this is not standard.
