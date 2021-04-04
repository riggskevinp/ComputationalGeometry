# Requirements
- [x] Implement divide and conquer algorithm. Guibas and Stolfi
- [x] Implement divide and conquer algorithm whose recursion alternates between
using horizontal cuts (at even depths of the recursion tree) and vertical cuts (at odd depths) to divide the
points into two subsets. Rex Dwyer
- [NA] Implement the incremental insertion algorithm.  Guibas and Stolfi suboptimal walking method.
- [NA] Implement fast point location incremental.
- [x] Code will take as input a csv file containing, use floating point.
- [x] Instructions for running will be included; including how to build.
- [x] A table containing timings for each of the algorithms or point location methods on random points
sets of 10,000, 100,000, 1,000,000 points.  (If runtime longer than 30 minutes cut short and note)
Time only the randomized versions of the incremental insertion algorithm. Try
to exclude all file I/O from timings.
- [x] Questions answered

# Instructions for Running
Commandline usage:
Triangulation.exe {name of csv}
or
./{Triangulation.exe or Triangulation} {name of csv}

CSV ideally is in the same folder as the executable.  Passing the path to the csv might work but I give no guarantees, especially cross platform.

If no arguments are passed, three random triangulations will be done of sizes 10k, 100k and 1M for both vertical cuts and alternating cuts algorithms.

# Instructions for Building
Opencv must be installed
Cmake must be installed, min version 3.5.
I have not built targetting linux or OSX.  CMakeLists.txt might need to be modified on those to get rid of the /MT and /MTd flags that were required to include gtest.  There was an issue with gtest wanting to use the static VC runtimes when the rest of the project was configured dynamically.  

Using an IDE like Qt that handles CMake projects easily is recommended.  Otherwise calling "cmake.exe --build . --target all" from the command line in the source directory should work.  Or breaking it up by calling "cmake ." from the command line and then "make build" should work.  Some combination of that depending on how your system is configured.

In Qt, I have my build folder separate from the source directory.  This is not required but is nice to have.  Additionally, the CMakelists.txt expects this as it copys some files from /src/test to /build/.  This should not be a problem if you do not separate your build folder but I give no guarantees.



# Question Answers

1. Divide and conquer: Can you create a point set for which the vertical cuts algorithm
is notably faster than the alternating cuts algorithm?
I don't think we should be able to produce a point set that will be noticeably faster than the alternating cuts algorithm.
My expectation is that in the worst case the alternating cuts will still be nlogn.
A factor that might slow down the alternating cuts vs vertical only is if a point set is created that does not split evenly based on the sorting or causes the ordering to flip depending on if sorted horizontally vs vertically.  Specifically if the sorting horizontally then sorting vertically repeatedly results in a worst case sort the performance will suffer.  

The other possibility I see is if the points do not follow some sort of uniform distribution.  This thought comes based on the proof used in Dwyer's paper around why alternating cuts is faster.

What do you think accounts for the discrepancy?
Partitioning the points into boxes recursively requires an extra sort per recursive call.  If this sort is inefficient enough it can take as long as the vertical cut only algorithm.  This can be demostrated by fulling sorting at each level which causes the alternating cut algorithm to have the same run time as the vertical cut algorithm.

2. Incremental: Create and orderly point set, like a square grid whose vertices are 
given in a structured order. Does failing to randomize the order of the vertices significantly change
the running time of the incremental insertion algorithm with the fast point location method?

NA

3. Why do you think the fastest algorithm is fastest?
Dwyer includes a proof for theorem 4.2 which is around why the alternating cuts is faster than vertical cuts.  From what I gather, the alternating cuts algorithm breaks the problem into fewer subproblems of the same size that are solved in the same amount of time.
Number of subproblems: n/logn
Subproblem size: elogn
Subproblem solved using nlogn runtime algorithm which results in:
n/logn*elognlog(elogn) = n*elog(elogn) = nloglogn
I have a feeling that intermediate step above is wrong but I thought I'd give a quick shot at it.  Unless I'm overcomplicating it and e is negligible for large n.


# Timings

| Input Size N | Time (s) |
| --- | --- |
| Divide and Conquer | --- |
| 10,000 | 1 second |
| 100,000 | 12 seconds |
| 1,000,000 | 150 seconds |
| Divide and Conquer Alternate Cuts| --- |
| 10,000 | 0.? (Did not print sub whole second precision) |
| 100,000 | 7 |
| 1,000,000 | 87 |
| Incremental insertion| --- |
| 10,000 | --- |
| 100,000 | --- |
| 1,000,000 | --- |