using namespace std;
#include <iostream>
#include <algorithm>
#include <math.h>

bool isPerfectSquare(int n){
    int x = sqrt(n);
    
    return (x*x==n);
}

bool is_fib(int n){
    return isPerfectSquare(5*(n)*(n) + 4) || 
            isPerfectSquare(5*(n)*(n) - 4);
    
}
template <typename T>

bool Fib(T begin, T end)
{
    int count =0;
    T it_begin = begin;
    while (it_begin != end)
    {
        it_begin++;
        count++;
    }
    while(count==2 || count==1){
        it_begin = begin;
        if(is_fib(*it_begin)) return true;
        count--;
    }
    T new_b1 = begin;
    begin++;
    T new_b2 = begin;
    begin++;

    while (begin != end)
    {
        if(!is_fib(*begin)) return false;

        if ((*(new_b1) + *(new_b2)) != (*(begin)))
            return false;

        begin++;
        new_b1++;
        new_b2++;
    }
    return true;
}

template <typename T>

T Transpose(T begin, T end)
{ 
    int count = 0;
    T it_begin = begin;
    while (it_begin != end)
    {
        it_begin++;
        count++;
    }
    if (count % 2 == 1)
        return begin;

    for (int i = 0; i < count; i = i + 2)
    {
        iter_swap(begin, begin + 1);
        begin = begin + 2;
    }

    return end;
}

template <typename T,typename T2, typename F>
T2 Transform2(T begin, T end, T2 begin2, F f)
{
    
    int count = 0;
    T it_begin = begin;
    while (it_begin != end)
    {
        it_begin++;
        count++;
    }

    bool flag = false;
    if (count%2 == 1 || begin == end)
    {
        flag = true;
    }
    
    T it_b = begin;

    for (int i = 0; i < count; i = i + 2)
    {
        if(flag && i == (count-1)) {
            return begin2;

        }
        *(begin2) = f(*begin, *(begin + 1));
        begin = begin + 2;
        (begin2)++;
    }
    return begin2;
}