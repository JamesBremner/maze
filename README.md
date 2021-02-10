# maze

Generate random, solveable mazes and solve them.

<pre>
+---+---+---+---+---+---+---+---+
|   |   |       |       |       |
+   +   +   +   +---+   +---+   +
|       |   |   |               |
+   +---+   +---+   +---+---+---+
|       |       |       | *   *  
+   +---+---+   +---+   +   +---+
  *   *         |     *   *     |
+---+   +---+---+---+   +---+---+
|     *   *   * | *   *         |
+   +---+---+   +   +---+---+   +
|       |     * | *     |       |
+---+---+---+   +   +---+---+---+
|       |     *   * |   |       |
+   +   +---+   +   +   +   +   +
|   |           |           |   |
+---+---+---+---+---+---+---+---+
</pre>

Two maze generation algoritms are used:

 - binary tree produces simple easily solved mazes. https://en.wikipedia.org/wiki/Maze_generation_algorithm#Simple_algorithms
 - recursive division produces more complex mazes. https://en.wikipedia.org/wiki/Maze_generation_algorithm#Recursive_division_method
 
 The algorithm is selected using a command line option
```
maze --h
Allowed options:
  --help                produce help message
  --input arg           input file.
  --gen arg             Generate new maze. Specify algorithm: none, binary,
                        recursive
```

The generated mazes are stored in a text file using "ascii art".  An example is shown at the top of this page.

```
'+' represents the corner of a cell
'|' represents a vertical wall between two cells
'---' represents a horizontal wall between two cells
'*' represents a cell visited on the solution path
```

These files are also used to input mazes to be solved.
