#!/usr/bin/env python3

import sys, time, argparse, subprocess, os.path
from random import uniform


Description = """
Tool to generate random uniform Wheeler DFAs.
"""

dirname = os.path.dirname(os.path.abspath(__file__))
gendfa_file_exe    =  dirname + "/exec/gen-dfa-file.x"
gendfa_bin_exe     =  dirname + "/exec/gen-dfa-bin.x"
gendfa_bin_64_exe  =  dirname + "/exec/gen-dfa-bin-64.x"

def main():
    parser = argparse.ArgumentParser(description=Description, formatter_class=argparse.RawTextHelpFormatter)
    parser.add_argument('-n', '--N', help='number of states', default = 0, type=int)
    parser.add_argument('-m', '--M', help='number of edges', default = 0, type=int)
    parser.add_argument('-a', '--A', help='alphabet size (def. 128)', default = 128, type=int)
    parser.add_argument('--binary',  help='output the resulting WDFA in binary format (def. False)', action='store_true')
    parser.add_argument('-o', '--O', help='output file basename (def. n_m_a.wdfa)', default = " ", type=str)
    parser.add_argument('-s', '--S', help='random number generation seed (def. rand)', default = -1, type=int)
    parser.add_argument('--verbose', help='verbose mode on (def. False)',action='store_true')
    args = parser.parse_args()

    if args.N == 0 or args.M == 0:
        print("n and m must be set in input!")
        return

    if args.S == -1:
        args.S = str(int(uniform(0,2**64 - 1)))

    if args.O == " ":
        args.O = str(args.N) + "_" + str(args.M) + "_" + str(args.A) + ".wdfa"

    if args.binary:

        if args.N > 2**32 - 1:

            command = "{exe} {n} {m} {a} {out} {seed}".format(exe=gendfa_bin_64_exe, n=str(args.N), m=str(args.M), a=str(args.A), out=args.O, seed=args.S)

            if args.verbose:
                command += " 1"
            else:
                command += " 0"

            print("==== generate a random WDFA (binary mode, 64 bit mode). Command: ", command)
            if(execute_command(command)!=True):
                return

        else:

            command = "{exe} {n} {m} {a} {out} {seed}".format(exe=gendfa_bin_exe, n=str(args.N), m=str(args.M), a=str(args.A), out=args.O, seed=args.S)

            if args.verbose:
                command += " 1"
            else:
                command += " 0"

            print("==== generate a random WDFA (binary mode, 32 bit mode). Command: ", command)
            if(execute_command(command)!=True):
                return

    else:

        command = "{exe} {n} {m} {a} {out} {seed}".format(exe=gendfa_file_exe, n=str(args.N), m=str(args.M), a=str(args.A), out=args.O, seed=args.S)

        if args.verbose:
            command += " 1"
        else:
            command += " 0"

        print("==== generate a random WDFA. Command: ", command)
        if(execute_command(command)!=True):
            return


# execute command: return True is everything OK, False otherwise
def execute_command(command):
  try:
    subprocess.check_call(command.split())
  except subprocess.CalledProcessError:
    print("Error executing command line:")
    print("\t"+ command)
    return False
  return True


if __name__ == '__main__':
    main()