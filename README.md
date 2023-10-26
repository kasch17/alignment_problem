# Alignment Problem
## Overview
In this repository, you will find the code for the assignments of the Advanced Algorithms and Parallel Programming course I took during my MSc in Computer Science and Engineering @ Politecnico di Milano, during the A.Y. 2023/2024.
## Assignment 1
Let’s assume we have two sequences of strings composed by any sequence of the following four characters: 'A', 'C', 'G', and 'T'.
- Adenine (A)
- Cytosine (C)
- Guanine (G)
- Thymine (T)

You would like to align those two sequences by inserting gaps or admitting differences.
- Any time you have a gap, an '\_' is inserted in one of the sequences. Any '\_' adds a cost of 2 units to the final solution.
- Any time you allow a difference, replace the two characters with a '\*'. Any '\*' adds a cost of 5 units to the final solution (10 on both strings).

Given two strings, X = x<sub>1</sub> x<sub>2</sub> ... x<sub>m</sub>, and Y = y<sub>1</sub> y<sub>2</sub> ... y<sub>n</sub>, write the algorithm that outputs the minimum cost-aligned strings. The two aligned strings generated have to have the same length.
