# Uniform random generation of Wheeler DFAs

## Description

Tool to sample a random uniform Wheeler DFA (WDFA) with the given number of nodes, number of edges, and alphabet size.

### Requirements

* A modern C++17 compiler such as `g++` version 8.4 or higher.
* The cmake build system, version \geq 3.10.
* A Linux 64-bit operating system.

### Download

To clone the repository, run the following command:

```console
git clone https://github.com/regindex/Wheeler-DFA-generation
```

### Compile 

You can compile with make:

```console
cd Wheeler-DFA-generation
make
```

### Input

You need to give in input the number of states ($n$), the size of the alphabet ($\sigma$), and the number of edges ($m$) of the resulting uniform WDFA.
These input values must respect the following constraints:

* unordered list
  * $\sigma < n$
  * $\sigma \leq m \leq n \cdot \sigma$
  * $m \geq n-1$

### Usage

```
usage: gen-WDFA.py [-h] [-n N] [-m M] [-a A] [--binary] [-o O] [-s S]
                   [--verbose]

Tool to generate uniform random Wheeler DFAs.

optional arguments:
  -h, --help   show this help message and exit
  -n N, --N N  number of states
  -m M, --M M  number of edges
  -a A, --A A  alphabet size (def. 128)
  --binary     output the resulting WDFA in binary format (def. False)
  -o O, --O O  output file basename (def. n_m_a.wdfa)
  -s S, --S S  random number generation seed (def. rand)
  --verbose    verbose mode on (def. False)
```
  
### Run

After compiling, run:

```console
python3 gen-WDFA.py -n 5 -m 4 -a 4 -o test.wdfa 
```

This command will generate a WDFA with $n$ nodes and $m$ edges labeled with an alphabet of size $a = \sigma$. The resulting WDFA is streamed to the 'test.wdfa' file.
The output will be written in an intermediate format (see below for an example), and the source state marked as 0.

```
5 8 4 0
0
0 1
2 1
1
3 2
4 2
2
0 3
1 3
2 3
3
3 4
```

The first line contains the number of states, number of edges, alphabet size, and source node separated by newline characters. Each other line contains either a label or an edge (pair of states: origin-destination). All edges between two consecutive labels, c and c', are labeled with c. For instance, the two edges 0 1 and 2 1 are labeled with 0.
Using the '--binary' flag, it is possible to stream the resulting WDFA in binary format (the fastest method); in this case, each line after the first contains three integers representing an edge (origin label destination). The integers in the first line are represented using 64 bits, while the others can be represented using either 32 bits (if $n < 2^{32} - 1$) or 64 bits. 

### External resources

* [sampleiterator](https://github.com/shekelyan/sampleiterator)

### Funding

This project has received funding from the European Research Council (ERC) under the European Union’s Horizon Europe research and innovation programme, project REGINDEX, grant agreement No 101039208