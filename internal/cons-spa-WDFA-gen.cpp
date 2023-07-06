#include "sample_generator.hpp"
#include <fstream>
#include <chrono>

typedef unsigned long long ull;

std::mt19937 seed_gen;

int main(int argc, char** argv) {

    // read input variables
    ull n,m,sigma;
    std::string seed, out;
    bool v = false;
    ull rejections = 0;

    if(argc > 6)
    { 
        n = std::stoull(argv[1]);
        m = std::stoull(argv[2]);
        sigma = std::stoull(argv[3]);
        out = std::string(argv[4]);
        seed = std::string(argv[5]);
        v = std::atoi(argv[6]);
    }
    else{ std::cerr << "wrong number of parameters!\n"; exit(1); }

    if( v )
        std::cout << "Generating a Wheeler DFA consisting of " << n << " states: " << m << " edges with alphabet size " << sigma << "\n";

    std::ofstream ofile;
    bool valid = false;
    seed_gen = std::mt19937(atoi(seed.c_str()));

    auto start = std::chrono::steady_clock::now();

    // check input concistency
    if (m >= sigma && m <= sigma * n && n > sigma && m >= n-1) {

        while(!valid)
        {

            #ifdef OFILE
            {
                ofile.open(out); 
            }
            #endif
            
            valid = true;

            ull i = 0, v = 0;
        
            SampleIterator::HiddenShuffle iterator(seed, n*sigma, m);

            SampleIterator::HiddenShuffle iterator_bars(seed, m-sigma, n-sigma-1);

            #ifdef OFILE
            {
                ofile << n << " " << m << " " << sigma <<  " " << 0 << "\n";
            }
            #else
            {
                std::cout << n << " " << m << " " << sigma << " " << 0 << "\n";
            }
            #endif

            ull i_p = iterator_bars.front();

            ull col = 0, prev_col = 0, u = 0;

            while(!iterator.empty())
            {
                ull current = iterator.front();
                iterator.popFront();

                col = (current/n) + 1;
                u = (current%n) + 1;

                if( col > prev_col + 1 )
                {
                    ofile.close();
                    rejections++;
                    seed = std::to_string(seed_gen());
                    valid = false;
                    break;
                }

                if( col == prev_col + 1 )
                {
                    v++;
                    prev_col = col;

                    #ifdef OFILE
                    {
                        ofile << col-1 << "\n";
                    }
                    #else
                    {
                        std::cout << col-1 << "\n";
                    }
                    #endif
                }
                else
                {
                    if( i == i_p )
                    {
                        v++;
                        iterator_bars.popFront();
                        i_p = iterator_bars.front();
                    }

                    i++;
                }

                #ifdef OFILE
                {
                    ofile << u-1 << " " << v << "\n";
                }
                #else
                {
                    std::cout << u-1 << " " << v << "\n";
                }
                #endif
            }

            if( valid && col != sigma )
            {
                ofile.close();
                rejections++;
                seed = std::to_string(seed_gen());
                valid = false;
            }
        }
        
        #ifdef OFILE
        {
            ofile.close();
        }
        #endif

        auto end = std::chrono::steady_clock::now();

        if( v )
        {
            std::cout << "Number of rejections: " << rejections << "\n";
            std::cout << "WDFA construction: Elapsed time in microseconds: "
            << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
            << " sec" << std::endl;
            std::cout << "WDFA construction: Throughput: "
            << (double)m/std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
            << " edges/microsec" << std::endl;
        }
        
    }
    else
    {
        std::cerr << "The value passed as argument don't respect the constraints." << std::endl;
        return -1;
    }
    
    return 0;
}