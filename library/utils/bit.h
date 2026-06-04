
#define ctz(x) (countr_zero((unsigned long long)x))
#define cto(x) ctz(~(unsigned long long)x)
#define popcount(x) (std::popcount((unsigned long long)x))