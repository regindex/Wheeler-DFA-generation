#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>


unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::mt19937 generator (seed); // mt19937 is a standard mersenne_twister_engine

typedef unsigned long long ull;

class WG {
private:
    std::vector<bool> out;
    std::vector<bool> bars;
    ull n=0, m=0, sigma=0;

    //creation of the outgoing edges of every node.
    void create_out_edges() {
        out.resize(n * sigma, false);
        for (ull i = 0; i < m; ++i) {
            out[i] = true;
        }
        /* Rejection sampling for generate out label and degree
         *
         * We check that we have at least one edge for every label
         *
         * */
        bool check = false;
        while (!check) {
            for (long long i = n * sigma - 1; i > 0; --i) {
                ull j = generator() % (i + 1);
                bool tmp = out[i];
                out[i] = out[j];
                out[j] = tmp;
            }
            check = check_correct();
        }
    }

    /*Check if the configuration is correct
     *
     * We need at least one 1 for every column
     * */
    bool check_correct() const {
        bool check = true;
        for (ull c = 0; c < sigma; ++c) {
            bool check_col = false;
            for (ull i = 0; i < n; ++i) {
                if (out[i * sigma + c])  check_col = true;
            }
            if (!check_col) check = false;
        }
        return check;
    }

    //generate the positions of the bars, excluding the sources and the fixed bars.
    void define_bars() {
        bars.resize(m-sigma,false);
        for (ull i = 0; i < n-sigma-1; ++i) {
            bars[i] = true;
        }
        for (long long i = m-sigma-1 - 1; i > 0; --i) {
            ull j =  generator() % (i + 1);
            bool tmp = bars[i];
            bars[i] = bars[j];
            bars[j] = tmp;
        }
    }
    
    //cout 1's in a column
    int count_col(int c) const {
        ull count = 0;
        for (ull j = 0; j < n; ++j) {
            if (out[j * sigma + c] == 1)
                ++count;
        }
        return count;
    }

public:
    WG(const ull n, const ull m, const ull sigma) {
        this->n = n;
        this->m = m;
        this->sigma = sigma;

        create_out_edges();
        define_bars();
    }

    //generate graph with labels on nodes
    void generate_graphviz() {
        ull pos = 1, j = 0;

        std::cout << "digraph { forcelabels=true;" << std::endl;
        std::cout << "\""<< 0 << "\" [label=\" S \"]"<< std::endl;
        for (int c = 0; c < sigma; ++c) {
            /*cycle inside the label column
            * check how many 1's one a column
            */
            int count = count_col(c);
            for (ull i = 0; i < n; ++i) {
                if (out[i*sigma + c] == 1) {
                    std::cout << "\"" << i << "\"->\"" << pos << "\" " << std::endl;
                    --count;
                    if (count > 0) {//check if we have 1's and so we can put a bar
                        if (bars[j] == 1) {//if we have a bars, we change node
                            std::cout << "\"" << pos << "\" [label=\"" << c+1 << "\"]"<< std::endl;
                            ++pos;
                        }
                        ++j;
                    }
                }
            }
            //fixed bar for change label
            std::cout << "\"" << pos << "\" [label=\"" << c+1 << "\"]" << std::endl;
            ++pos;
        }
        std::cout <<"}"<< std::endl;
    }
};

int main(void) {
    ull n,m,sigma;
    std::cin >> n >> sigma >> m;
    if (m >= sigma && m <= sigma * n && n > sigma) {
        WG wg(n, m, sigma);
        wg.generate_graphviz();
    } else {
        std::cerr << "The value passed as argument don't respect the constraints." << std::endl;
        return -1;
    }
    return 0;
}