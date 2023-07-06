#ifndef SAMPLES_GENERATOR_HPP_
#define SAMPLES_GENERATOR_HPP_

/*
Sequential random sampler without replacement
source: https://github.com/shekelyan/sampleiterator.git
*/

#include <iostream> 
#include <random>
#include <assert.h>
#include <unordered_set>
#include <set>
#include <chrono>
#include <algorithm>

class RandomNumbers{

	std::seed_seq seed1;
	std::mt19937 generator;
	
	std::uniform_real_distribution<double> default_real;
	std::uniform_int_distribution<size_t> default_int;
	
public:
	
	inline RandomNumbers(const std::string seed, size_t i1 = 0, size_t i2 = 1, double r1 = 0.0, double r2 = 1.0) : seed1(seed.begin(), seed.end()), generator(seed1), default_real(r1,r2), default_int(i1, i2){
	
	}
	
	inline double randomReal(){
	
		return default_real(generator);
	}
	
	inline size_t randomInteger(){
	
		return default_int(generator);
	}
	
	inline size_t randomInteger(size_t a, size_t b){
	
		return std::uniform_int_distribution<long>(a, b)(generator);
	}
};

namespace SampleIterator{

	class HiddenShuffle{ // sequential sample iterator using the hidden shuffle method (Shekelyan & Cormode 2021) storing a handful values

		RandomNumbers rnd; // pseudorandom number generator
		long H; // number of low-high swaps (follows poisson-binomial distribution)
		long L; // equal to number of sampled low-items < n beginning of step 3 (follows hypergeometric distribution)
		long N; // population size
		long n; // sample size
		double a; // current order statistic of H standard uniform variables
	
		long current; // current sampled item
	
	public:
		inline HiddenShuffle(const std::string seed, long universeSize, long sampleSize) : rnd(seed){
		
			N = universeSize;
			n = sampleSize;
		
			// STEP 1
		
			H = 0; long i = 0;
		
			if (N > n){
			
				H = n;
				while (i < n){
			
					const double q = 1-1.0*(N-n)/(N-i);
				
					i += (long) floor(log(rnd.randomReal() )/log(1-q));
			
					const double pi = 1-1.0*(N-n)/(N-i);
				
					if (i < n && (rnd.randomReal() < (pi/q) )){
						--H;
					}
					++i;
				}
			}
		
			L = n-H; // (n-H) is the number of low-low swaps
			a = 1.0;
			popFront();
		}
	
		long front() const{

			return current;
		}
	
		bool empty() const {
		
			return current == N;
		}
	
		void popFront(){
		
			// STEP 2
		
			while (H > 0){
		
				const long S_old = n+(long) floor(a*(N-n));
		
				a *= pow(rnd.randomReal(), 1.0/H);
			
				const long S = n+(long) floor(a*(N-n));
			
				if (S < S_old){
			
					current = (N-1)-S;
					--H;
					return;
				
				} else {
			
					++L; // duplicate detected
					--H;
				}
			}
		
			// STEP 3
		
			while (L > 0){
			
				const double u = rnd.randomReal();
				long s = 0;
				double F = L*1.0/n;
			
				while (F < u && s < (n-L)){
				
					F = 1.0-(1.0-L*1.0/(n-s-1))*(1.0-F);
					++s;
				}
			
				--L;
				n = n-s-1;
			
				current = (N-1)-n; 
				return;
			}
		
			current = N; // termination condition (N will not be reported!)
		}
	};
};

#endif