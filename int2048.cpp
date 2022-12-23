#include "int2048.h"

namespace sjtu
{
const int unit = 100000000;

  // ===================================
  // Integer1
  // ===================================

int2048::int2048()
{
    number.push_back(0);
    sign = 0;
}

int2048::int2048(const long long &a)
{
    long long x = a;
    sign = false;
    if (x < 0)
    {
        sign = true;
        x = -x;
    }
    if (x == 0)
    {
        number.push_back(0);
        return;
    }
    for (; x != 0; x /= unit)
        number.push_back(x % unit);
}

int2048::int2048(const std::string &s)
{
    sign = false;
    if (*s.begin() == '-') sign = true;
    number.push_back(0);
    int j = 0, k = 1;
    for (auto i = s.rbegin(); i != s.rend() && *i != '-'; i++, k *= 10) 
    {
        if (k >= unit)
        {
            k = 1;
            j++;
            number.push_back(0);
        }
        number[j] += k * (*i - '0');
    }
    if (s == "-0") sign = false;
}

int2048::int2048(const int2048 &x)
{
    number = x.number;
    sign = x.sign;
}

void int2048::read(const std::string &s)
{
    number.clear();
    sign = false;
    if (*s.begin() == '-') sign = true;
    number.push_back(0);
    int j = 0, k = 1;
    for (auto i = s.rbegin(); i != s.rend() && *i != '-'; i++, k *= 10) 
    {
        if (k >= unit)
        {
            k = 1;
            j++;
            number.push_back(0);
        }
        number[j] += k * (*i - '0');
    }
    if (s == "-0") sign = false;
} 

void int2048::print()
{
    if (sign) std::cout << '-';
    int tmpint = *number.rbegin();
    std::cout << tmpint;
    for (auto i = ++number.rbegin(); i != number.rend(); i++)
    {
        std::string tmpstring = std::to_string(*i);
        if (tmpstring.size() < 8)
            tmpstring.insert(0, 8 - tmpstring.size(), '0');
        std::cout << tmpstring;
    }
}

int2048 &int2048::add(const int2048 &second)
{
    if (sign == second.sign)
    {
        int carry = 0;
        auto i = number.begin();
        auto j = second.number.begin();
        for (; i != number.end() && j != second.number.end(); i++, j++)
        {
            int res = *i + *j + carry;
            if (res >= unit)
            {
                res -= unit;
                carry = 1;
            }
            else carry = 0;
            *i = res;
        }
        if (i == number.end())
        {
            while(j != second.number.end())
            {
                int res = *j+ carry;
                if (res >= unit)
                {
                    res -= unit;
                    carry = 1;
                }
                else carry = 0;
                number.push_back(res);
                j++;
            }
        }
        else
        {
            while(i != number.end())
            {
                int res = *i + carry;
                if (res >= unit)
                {
                    res -= unit;
                    carry = 1;
                }
                else carry = 0;
                *i = res;
                i++;
            }
        }
        if (carry == 1) number.push_back(1);
        return *this;
    }
    int2048 tmp = second;
    if (sign)
    {
        sign = 0;
        *this = tmp.minus(*this);
        return *this;
    }
    tmp.sign = 0;
    this->minus(tmp);
    return *this;
}

int2048 add(int2048 a, const int2048 &b)
{
    return a.add(b);
}

int2048 &int2048::minus(const int2048 &second)
{
    if (sign != second.sign)
    {
        int2048 tmp = second;
        tmp.sign = !tmp.sign;
        this->add(tmp);
        return *this;
    }
    std::vector<int> a, b;
    if (sign)
    {
        if (*this >= second)
        {
            sign = 0;
            a = second.number;
            b = number;
        }
        else
        {
            a = number;
            b = second.number;
        }
    }
    else
    {
        if (*this >= second)
        {
            a = number;
            b = second.number;
        }
        else
        {
            sign = 1;
            a = second.number;
            b = number;
        }
    }
    auto i = a.begin();
    auto j = b.begin();
    bool carry = 0;
    for (; j != b.end(); i++, j++)
    {
        int res = *i - *j - carry;
        if (res < 0)
        {
            res += unit;
            carry = 1;
        }
        else carry = 0;
        *i = res;
    }
    while (i != a.end())
    {
        int res = *i - carry;
        if (res < 0)
        {
            res += unit;
            carry = 1;
        }
        else carry = 0;
        *i = res;
        i++;
    }
    while (!(*a.rbegin()) && a.size() > 1) a.pop_back();
    number = a;
    return *this;
}

int2048 minus(int2048 a, const int2048 &b)
{
    return a.minus(b);
}

  // ===================================
  // Integer2
  // ===================================

int2048 &int2048::operator=(const int2048 &x)
{
    number = x.number;
    sign = x.sign;
    return *this;
}

int2048 &int2048::operator+=(int2048 a)
{
    return this->add(a);
}

int2048 operator+(int2048 a, const int2048 &b)
{
    return a.add(b);
}

int2048 &int2048::operator-=(int2048 a)
{
    return this->minus(a);
}

int2048 operator-(int2048 a, const int2048 &b)
{
    return a.minus(b);
}

int2048 &int2048::operator*=(const int2048 &second)
{
    if (sign == second.sign) sign = 0;
    else sign = 1;
    std::vector<int> res;
    res.push_back(0);
    int respos = 0;
    for (auto j = second.number.begin(); j != second.number.end(); j++, respos++)
    {
        auto tmpiter = res.begin();
        tmpiter += respos;
        int carry = 0;
        for (auto i = number.begin(); i != number.end(); i++)
        {
            long long tmpnumber = ((long long)*i) * (*j) + carry + *tmpiter;
            carry = (int) (tmpnumber / unit);
            *tmpiter = (int) (tmpnumber % unit);
            if (++tmpiter == res.end())
            {
                res.push_back(0);
                tmpiter = --res.end();
            }
        }
        *tmpiter += carry;
    }
    while (!(*res.rbegin()) && res.size() > 1) res.pop_back();
    this->number = res;
    return *this;
}

int2048 operator*(int2048 a, const int2048 &b)
{
    return a *= b;
}

int trydivide(const int2048 &a, const int2048 &b, int lowerbound, int upperbound)
{
    if (upperbound - lowerbound <= 1) return lowerbound;
    int mid = (lowerbound + upperbound) >> 1;
    if (a < b * mid) return trydivide(a, b, lowerbound, mid);
    return trydivide(a, b, mid, upperbound);
}

int2048 &int2048::operator/=(const int2048 &b)
{
    bool sign1 = sign;
    int2048 second = b;
    if (!*second.number.rbegin()) second.number.pop_back();
    sign = second.sign = 0;
    if (*this < second)
    {
        number.clear();
        number.push_back(0);
        return *this;
    }
    int size_a = number.size(), size_b = second.number.size();
    std::vector<int> res;
    int2048 part_a;
    part_a.number.clear();
    auto a_iter = number.rbegin();
    for (int i = 0; i < size_b; a_iter++, i++)
        part_a.number.insert(part_a.number.begin(), *a_iter);
    for (int i = 0; i <= size_a - size_b; i++)
    {
        if (part_a < second) res.insert(res.begin(), 0);
        else 
        {
            long long x;
            int y = *second.number.rbegin();
            if (part_a.number.size() > second.number.size())
                x = (long long)(*part_a.number.rbegin()) * unit + *(++part_a.number.rbegin());
            else
                x = *part_a.number.rbegin();
            res.insert(res.begin(), trydivide(part_a, second, x / (y + 1), (x + 1) / y + 1));
        }
        int2048 remain = part_a - (second * (*res.begin()));
        if (remain == 0) part_a.number.clear();
        else part_a = remain;       
        part_a.number.insert(part_a.number.begin(), *a_iter);
        a_iter++;
    }
    while (!*res.rbegin() && res.size() > 1) res.pop_back();
    number.clear();
    number = res;
    if (sign1 == b.sign) sign = 0;
    else sign = 1;
    return *this;
}

int2048 operator/(int2048 a, const int2048 &b)
{
    return a /= b;
}

std::istream &operator>>(std::istream &in, int2048 &a)
{
    std::string toread;
    in >> toread;
    a.read(toread);
    return in;
}

std::ostream &operator<<(std::ostream &out, const int2048 &a)
{
    if (a.sign) std::cout << '-';
    int tmpint = *a.number.rbegin();
    std::cout << tmpint;
    for (auto i = ++a.number.rbegin(); i != a.number.rend(); i++)
    {
        std::string tmpstring = std::to_string(*i);
        if (tmpstring.size() < 8)
            tmpstring.insert(0, 8 - tmpstring.size(), '0');
        std::cout << tmpstring;
    }
    return out;
}

bool operator==(const int2048 &a, const int2048 &b)
{
    if (a.sign != b.sign) return false;
    if (a.number.size() != b.number.size()) return false;
    for (auto i = a.number.begin(), j = b.number.begin(); i != a.number.end(); i++, j++)
        if (*i != *j) return false;
    return true;
}

bool operator!=(const int2048 &a, const int2048 &b)
{
    return !(a == b);
}

bool operator<(const int2048 &a, const int2048 &b)
{
    if (a.sign != b.sign) return a.sign;
    bool res, flag = 1;
    if (a.number.size() != b.number.size()) 
    {
        res = a.number.size() < b.number.size();
        flag = 0;
    }
    else
    {
        for (auto i = a.number.rbegin(), j = b.number.rbegin(); i != a.number.rend(); i++, j++)
            if (*i != *j)
            {
                res = *i < *j;
                flag = 0;
                break;
            }
    }
    if (flag) return false;
    if (a.sign) res = !res;
    return res;
}

bool operator>(const int2048 &a, const int2048 &b)
{
    if (a == b) return false;
    return b < a;
}

bool operator<=(const int2048 &a, const int2048 &b)
{
    return (a < b) || (a == b);
}

bool operator>=(const int2048 &a, const int2048 &b)
{
    return (b < a) || (a == b);
}
}
