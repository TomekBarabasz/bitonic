#include <iostream>
#include <cstdint>
#include <tuple>

using namespace std;

//gcc bitonic.cpp -lstdc++ -std=c++17 -O3 -o bitonic

template <typename T,int N>
struct Vect {
    T data[N];
    static constexpr size_t size = N;
    T& operator[](size_t i) { return data[i];}
    T  operator[](size_t i) const { return data[i];}
};

using int16x16_t = Vect<int16_t,16>;

__always_inline int16x16_t _mm256_set_epi16(std::initializer_list<int16_t> list)
{
    int16x16_t v;
    for(unsigned i=0;i<int16x16_t::size;++i) {
        v[i] = *(list.begin() + int16x16_t::size - i - 1);
    }
    return v;
}
__always_inline int16x16_t _mm256_permutexvar_epi16(int16x16_t idx, int16x16_t value)
{
    int16x16_t p;
    for(unsigned i=0;i<int16x16_t::size;++i) {
        p[ i ] = value [ idx[i] ];
    }
    return p;
}
__always_inline int16x16_t _mm256_min_epi16(int16x16_t a, int16x16_t b)
{
    int16x16_t ret;
    for(unsigned i=0;i<int16x16_t::size;++i) {
        ret[ i ] = a[i] < b[i] ? a[i] : b[i];
    }
    return ret;
}
__always_inline int16x16_t _mm256_max_epi16(int16x16_t a, int16x16_t b)
{
    int16x16_t ret;
    for(unsigned i=0;i<int16x16_t::size;++i) {
        ret[ i ] = a[i] > b[i] ? a[i] : b[i];
    }
    return ret;
}
__always_inline int16x16_t _mm256_mask_mov_epi16(int16x16_t src, int16_t mask, int16x16_t a)
{
    int16_t pos = 1;
    int16x16_t ret;
    for(unsigned i=0;i<int16x16_t::size;++i,pos <<= 1) {
        ret[i] = pos & mask ? a[i] : src[i];
    }
    return ret;
}
__always_inline int16_t _mm256_gt_epi16_mask(int16x16_t a, int16x16_t b)
{
    int16_t res = 0;
    int16_t pos = 1;
    for(unsigned i=0;i<int16x16_t::size;++i,pos <<= 1) {
        if (a[i] > b[i]) res |= pos;
    }
    return res;
}
__always_inline bool operator==(const int16x16_t& a, const int16x16_t& b) 
{
    for(unsigned i=0;i<int16x16_t::size;++i) {
        if (a[i] != b[i]) return false;
    }
    return true;
}
__always_inline bool operator!=(const int16x16_t& a, const int16x16_t& b) 
{
    return !(a==b);
}
std::string to_string(int16x16_t v)
{
    std::string s="";
    for(int i = int16x16_t::size-1; i>=0; --i) {
        s += std::to_string(v[i]);
        if (i > 0) s += ',';
    }
    return s;
}

int16x16_t bitonic_sort(int16x16_t input)
{
	int16x16_t idx, perm, pmin, pmax;
	idx  = _mm256_set_epi16({10,11,3,2,14,15,8,9,6,7,0,1,13,12,4,5});
	perm = _mm256_permutexvar_epi16(idx,input);
	pmin = _mm256_min_epi16(perm,input);
	pmax = _mm256_max_epi16(perm,input);
	input = _mm256_mask_mov_epi16(pmin,0b1111001010110000,pmax);
	idx  = _mm256_set_epi16({13,5,15,9,8,1,12,11,4,3,14,7,6,0,10,2});
	perm = _mm256_permutexvar_epi16(idx,input);
	pmin = _mm256_min_epi16(perm,input);
	pmax = _mm256_max_epi16(perm,input);
	input = _mm256_mask_mov_epi16(pmin,0b1101110011000100,pmax);
	idx  = _mm256_set_epi16({7,12,4,14,2,6,5,0,15,10,9,13,1,11,3,8});
	perm = _mm256_permutexvar_epi16(idx,input);
	pmin = _mm256_min_epi16(perm,input);
	pmax = _mm256_max_epi16(perm,input);
	input = _mm256_mask_mov_epi16(pmin,0b1110111100001000,pmax);
	idx  = _mm256_set_epi16({14,15,11,7,13,9,10,3,12,5,6,2,8,4,0,1});
	perm = _mm256_permutexvar_epi16(idx,input);
	pmin = _mm256_min_epi16(perm,input);
	pmax = _mm256_max_epi16(perm,input);
	input = _mm256_mask_mov_epi16(pmin,0b1011010101010010,pmax);
	idx  = _mm256_set_epi16({15,12,10,14,7,13,6,4,11,9,2,8,1,5,3,0});
	perm = _mm256_permutexvar_epi16(idx,input);
	pmin = _mm256_min_epi16(perm,input);
	pmax = _mm256_max_epi16(perm,input);
	input = _mm256_mask_mov_epi16(pmin,0b1110101100101001,pmax);
	idx  = _mm256_set_epi16({15,13,14,10,4,12,7,6,9,8,3,11,5,1,2,0});
	perm = _mm256_permutexvar_epi16(idx,input);
	pmin = _mm256_min_epi16(perm,input);
	pmax = _mm256_max_epi16(perm,input);
	input = _mm256_mask_mov_epi16(pmin,0b1101101100100101,pmax);
	idx  = _mm256_set_epi16({15,14,12,13,10,11,8,9,6,7,4,5,2,3,1,0});
	perm = _mm256_permutexvar_epi16(idx,input);
	pmin = _mm256_min_epi16(perm,input);
	pmax = _mm256_max_epi16(perm,input);
	input = _mm256_mask_mov_epi16(pmin,0b1110101010101011,pmax);
	idx  = _mm256_set_epi16({15,14,13,12,9,8,11,10,5,4,7,6,3,2,1,0});
	perm = _mm256_permutexvar_epi16(idx,input);
	pmin = _mm256_min_epi16(perm,input);
	pmax = _mm256_max_epi16(perm,input);
	input = _mm256_mask_mov_epi16(pmin,0b1111110011001111,pmax);
	idx  = _mm256_set_epi16({15,14,13,11,12,9,10,7,8,5,6,3,4,2,1,0});
	perm = _mm256_permutexvar_epi16(idx,input);
	pmin = _mm256_min_epi16(perm,input);
	pmax = _mm256_max_epi16(perm,input);
	input = _mm256_mask_mov_epi16(pmin,0b1111010101010111,pmax);
	return input;
}

__always_inline int16x16_t compare_and_swap(int16x16_t values, int16x16_t idx, int16_t mask)
{
    int16x16_t perm = _mm256_permutexvar_epi16(idx,values);
    int16_t swap_mask = _mm256_gt_epi16_mask(values,perm) ^ mask;
	values = _mm256_mask_mov_epi16(values,swap_mask,perm);
    return values;
}

int16x16_t bitonic_sort_1(int16x16_t values)
{
    values = compare_and_swap(values,_mm256_set_epi16({10,11,3,2,14,15,8,9,6,7,0,1,13,12,4,5}), 0b1111001010110000);
    values = compare_and_swap(values,_mm256_set_epi16({13,5,15,9,8,1,12,11,4,3,14,7,6,0,10,2}), 0b1101110011000100);
    values = compare_and_swap(values,_mm256_set_epi16({7,12,4,14,2,6,5,0,15,10,9,13,1,11,3,8}), 0b1110111100001000);
	values = compare_and_swap(values,_mm256_set_epi16({14,15,11,7,13,9,10,3,12,5,6,2,8,4,0,1}), 0b1011010101010010);
    values = compare_and_swap(values,_mm256_set_epi16({15,12,10,14,7,13,6,4,11,9,2,8,1,5,3,0}), 0b1110101100101001);
    values = compare_and_swap(values,_mm256_set_epi16({15,13,14,10,4,12,7,6,9,8,3,11,5,1,2,0}), 0b1101101100100101);
    values = compare_and_swap(values,_mm256_set_epi16({15,14,12,13,10,11,8,9,6,7,4,5,2,3,1,0}), 0b1110101010101011);
    values = compare_and_swap(values,_mm256_set_epi16({15,14,13,12,9,8,11,10,5,4,7,6,3,2,1,0}), 0b1111110011001111);
    values = compare_and_swap(values,_mm256_set_epi16({15,14,13,11,12,9,10,7,8,5,6,3,4,2,1,0}), 0b1111010101010111);
	return values;
}

__always_inline std::tuple<int16x16_t,int16x16_t> compare_and_swap_kv(std::tuple<int16x16_t,int16x16_t> keys_values, int16x16_t idx, int16_t mask)
{
    int16x16_t keys = std::get<0>(keys_values);
    int16x16_t values = std::get<1>(keys_values);
	int16x16_t kperm = _mm256_permutexvar_epi16(idx,keys);
    int16x16_t vperm = _mm256_permutexvar_epi16(idx,values);
    int16_t swap_mask = _mm256_gt_epi16_mask(keys,kperm) ^ mask;
    keys = _mm256_mask_mov_epi16(keys,swap_mask,kperm);
    values = _mm256_mask_mov_epi16(values,swap_mask,vperm);
    return {keys,values};
}

std::tuple<int16x16_t,int16x16_t> bitonic_sort_kv(std::tuple<int16x16_t,int16x16_t> keys_values)
{
    keys_values = compare_and_swap_kv(keys_values,_mm256_set_epi16({10,11,3,2,14,15,8,9,6,7,0,1,13,12,4,5}), 0b1111001010110000);
    keys_values = compare_and_swap_kv(keys_values,_mm256_set_epi16({13,5,15,9,8,1,12,11,4,3,14,7,6,0,10,2}), 0b1101110011000100);
    keys_values = compare_and_swap_kv(keys_values,_mm256_set_epi16({7,12,4,14,2,6,5,0,15,10,9,13,1,11,3,8}), 0b1110111100001000);
	keys_values = compare_and_swap_kv(keys_values,_mm256_set_epi16({14,15,11,7,13,9,10,3,12,5,6,2,8,4,0,1}), 0b1011010101010010);
    keys_values = compare_and_swap_kv(keys_values,_mm256_set_epi16({15,12,10,14,7,13,6,4,11,9,2,8,1,5,3,0}), 0b1110101100101001);
    keys_values = compare_and_swap_kv(keys_values,_mm256_set_epi16({15,13,14,10,4,12,7,6,9,8,3,11,5,1,2,0}), 0b1101101100100101);
    keys_values = compare_and_swap_kv(keys_values,_mm256_set_epi16({15,14,12,13,10,11,8,9,6,7,4,5,2,3,1,0}), 0b1110101010101011);
    keys_values = compare_and_swap_kv(keys_values,_mm256_set_epi16({15,14,13,12,9,8,11,10,5,4,7,6,3,2,1,0}), 0b1111110011001111);
    keys_values = compare_and_swap_kv(keys_values,_mm256_set_epi16({15,14,13,11,12,9,10,7,8,5,6,3,4,2,1,0}), 0b1111010101010111);
	return keys_values;
}
int test_sort()
{
    int16x16_t inputs[] = {
        _mm256_set_epi16({10,11,3,2,14,15,8,9,6,7,0,1,13,12,4,5}),
        _mm256_set_epi16({7,12,4,14,2,6,5,0,15,10,9,13,1,11,3,8}),
	    _mm256_set_epi16({14,15,11,7,13,9,10,3,12,5,6,2,8,4,0,1}),
	    _mm256_set_epi16({15,12,10,14,7,13,6,4,11,9,2,8,1,5,3,0}),
	    _mm256_set_epi16({15,13,14,10,4,12,7,6,9,8,3,11,5,1,2,0}),
	    _mm256_set_epi16({15,14,12,13,10,11,8,9,6,7,4,5,2,3,1,0}),
	    _mm256_set_epi16({15,14,13,12,9,8,11,10,5,4,7,6,3,2,1,0}),
	    _mm256_set_epi16({15,14,13,11,12,9,10,7,8,5,6,3,4,2,1,0})
    };
    int16x16_t sorted = _mm256_set_epi16({15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0});
    cout << "sorted " << to_string(sorted) << endl;
    for (auto & i : inputs) {
        int16x16_t s = bitonic_sort_1(i);
        if (s != sorted) {
            cout << "sort failed for :" << to_string(i) << " result is " << to_string(s) << endl;
            return 1;
        }
    }
    cout << "all sort tests ok" << endl;
    return 0;
}
int test_sort_kv()
{
    int16x16_t inputs[] = {
        _mm256_set_epi16({10,11,3,2,14,15,8,9,6,7,0,1,13,12,4,5}),
        _mm256_set_epi16({7,12,4,14,2,6,5,0,15,10,9,13,1,11,3,8}),
	    _mm256_set_epi16({14,15,11,7,13,9,10,3,12,5,6,2,8,4,0,1}),
	    _mm256_set_epi16({15,12,10,14,7,13,6,4,11,9,2,8,1,5,3,0}),
	    _mm256_set_epi16({15,13,14,10,4,12,7,6,9,8,3,11,5,1,2,0}),
	    _mm256_set_epi16({15,14,12,13,10,11,8,9,6,7,4,5,2,3,1,0}),
	    _mm256_set_epi16({15,14,13,12,9,8,11,10,5,4,7,6,3,2,1,0}),
	    _mm256_set_epi16({15,14,13,11,12,9,10,7,8,5,6,3,4,2,1,0})
    };
    int16x16_t sorted = _mm256_set_epi16({15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0});
    for (auto  i : inputs) {
        auto [ks,vs] = bitonic_sort_kv({i,i});
        if (ks != sorted) {
            cout << "kv sort failed for keys :" << to_string(i) << " result is " << to_string(ks) << endl;
            return 1;
        }
        if (vs != sorted) {
            cout << "kv sort failed for values :" << to_string(i) << " result is " << to_string(vs) << endl;
            return 1;
        }
    }
    cout << "all kv sort tests ok" << endl;
    return 0;
}
int main(int argc,const char**argv)
{
    test_sort();
    test_sort_kv();
    return 0;
}