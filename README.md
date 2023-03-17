# Uniform generation of a Wheeler DFA

## Description

This code uniformly generate a Wheeler DFA (WDFA) with the given characteristic.

### Download

To clone the repository, run:

> git clone http://github.com/regindex/

### Compile

You can compile with make:

> make

### Input

The lines of the input correspond to the number of nodes of the graphs ($n$), the cardinality of the alphabet ($\sigma$) and the number of edges ($m$). They need to respect this constraint:

* unordered list
  * $\sigma < n $
  * $\sigma \leq m \leq n \cdot \sigma$

> 5\
  4\
  12
  
### Run

After compiling, run:

> ./WDFA-generation < input.txt > output.txt

This command will generate a WDFA with $n$ nodes, $m$ edges and an alphabet of cardinality sigma. The input wil be take from the  "input.txt" file and the output from the "output.txt" file.
The output will be written is graphviz format, with the label on the nodes and the source is marked with 'S'.

### Funding

This project has received funding from the European Research Council (ERC) under the European Union’s Horizon Europe research and innovation programme, project REGINDEX, grant agreement No 101039208
