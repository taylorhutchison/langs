#include <vector>

std::vector<bool> makeSieve(int size){
    std::vector<bool> sieve(size);
    for(auto i = sieve.begin(); i != sieve.end(); i++){
        *i = true;
    }
    
    for (int i = 2; i <= size; i++)
    {
        if (sieve[i])
        {
            for (int j = i * 2; j <= size; j += i)
                sieve[j] = false;
        }
    }
    return sieve;
}