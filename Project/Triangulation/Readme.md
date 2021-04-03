# Requirements
- [x] Implement divide and conquer algorithm. Guibas and Stolfi
- [] Implement divide and conquer algorithm whose recursion alternates between
using horizontal cuts (at even depths of the recursion tree) and vertical cuts (at odd depths) to divide the
points into two subsets. Rex Dwyer
- [] Implement the incremental insertion algorithm.  Guibas and Stolfi suboptimal walking method.
- [] Implement fast point location incremental.
- [x] Code will take as input a csv file containing, use floating point.
- [] Instructions for running will be included including how to build.
- [] A table containing timings for each of the algorithms or point location methods on random points
sets of 10,000, 100,000, 1,000,000 points.  (If runtime longer than 30 minutes cut short and note)
Time only the randomized versions of the incremental insertion algorithm. Try
to exclude all file I/O from timings.
- [] Why do you think the fastest algorithm is fastest?
- [] Questions answered

# Instructions for Running


# Question Answers

1. Divide and conquer: Can you create a point set for which the vertical cuts algorithm
is notably faster than the alternating cuts algorithm?
What do you think accounts for the discrepancy?


2. Incremental: Create and orderly point set, like a square grid whose vertices are 
given in a structured order. Does failing to randomize the order of the vertices significantly change
the running time of the incremental insertion algorithm with the fast point location method?

3. Why do you think the fastest algorithm is fastest?


# Timings

| Input Size N | Time (s) |
| --- | --- |
| Divide and Conquer | --- |
| 10,000 | 1 second |
| 100,000 | 13 seconds |
| 1,000,000 | 157 seconds |
| Divide and Conquer Alternate Cuts| --- |
| 10,000 | --- |
| 100,000 | --- |
| 1,000,000 | --- |
| Incremental insertion| --- |
| 10,000 | --- |
| 100,000 | --- |
| 1,000,000 | --- |