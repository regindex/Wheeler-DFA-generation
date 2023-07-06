#include "sample_generator.hpp"
#include <cmath>
#include <chrono>

typedef unsigned long long ull;

std::mt19937 seed_gen;

int main(int argc, char** argv) {

    // read input variables
    ull n,m,sigma;
    std::string seed, out;
    FILE * ofile;
    bool v = false;
    ull rejections = 0;

    #ifdef M64
    {
        typedef uint64_t uint;
    }
    #else
    {
        typedef uint32_t uint;
    }
    #endif

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

    seed_gen = std::mt19937(atoi(seed.c_str()));
    bool valid = false;

    auto start = std::chrono::steady_clock::now();

    // check input concistency
    if (m >= sigma && m <= sigma * n && n > sigma && m >= n-1) {

        while(!valid)
        {

            ofile = std::fopen(out.c_str(),"w+");
            valid = true;

            ull i = 0, v = 0;
        
            SampleIterator::HiddenShuffle iterator(seed, n*sigma, m);

            SampleIterator::HiddenShuffle iterator_bars(seed, m-sigma, n-sigma-1);

            std::fwrite(&n, sizeof(n), 1, ofile);
            std::fwrite(&m, sizeof(m), 1, ofile);
            std::fwrite(&sigma, sizeof(sigma), 1, ofile);
            std::fwrite(&v, sizeof(v), 1, ofile);

            ull i_p = iterator_bars.front();

            ull col = 0, prev_col = 0, u = 0;

            while(!iterator.empty())
            {
                ull current = iterator.front();
                iterator.popFront();

                col = (current/n) + 1;
                u = (current%n);

                if( col > prev_col + 1 )
                {
                    std::fclose(ofile);
                    seed = std::to_string(seed_gen());
                    valid = false;
                    break;
                }

                if( col == prev_col + 1 )
                {
                    v++;
                    prev_col = col;
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
                
                std::fwrite(&u,   sizeof(uint), 1, ofile);
                col--;
                std::fwrite(&col, sizeof(uint), 1, ofile);
                col++;
                std::fwrite(&v,   sizeof(uint), 1, ofile);

            }

            if( valid && col != sigma )
            {
                std::fclose(ofile);
                rejections++;
                seed = std::to_string(seed_gen());
                valid = false;
            }

        }

        std::fclose(ofile);

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