# Uniform generation of a Wheeler DFA

## Description

This code uniformly generate a Wheeler DFA (WDFA) with the given number of nodes, number of edges and cardinality of the alphabet.

### Download

To clone the repository, run:

> git clone https://github.com/regindex/Wheeler-DFA-generation

### Compile

You can compile with make:

> make

### Input

The lines of the input correspond to the number of nodes of the graph ($n$), the cardinality of its alphabet ($\sigma$) and its number of edges ($m$).
These values need to respect this constraints:

* unordered list
  * $\sigma < n $
  * $\sigma \leq m \leq n \cdot \sigma$

> 5\
  4\
  12
  
### Run

After compiling, run:

> ./WDFA-generation < input.txt > output.txt

This command will generate a WDFA with $n$ nodes, $m$ edges and an alphabet of cardinality sigma. The input is taken from the file "input.txt" and the output from the file "output.txt".
The output will be written in graphviz format, with the label on the nodes and the source is marked with 'S'.

### Funding

This project has received funding from the European Research Council (ERC) under the European Union’s Horizon Europe research and innovation programme, project REGINDEX, grant agreement No 101039208
