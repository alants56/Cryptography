import time

def Erato(n) :
    flag = [True] * (n+1)
    for i in range(2, int(n ** 0.5) + 1) :
        if flag[i] :
            for j in range(i*i, n+1, i) :
                flag[j] = False
    return list(filter(lambda x : flag[x], range(2, n+1)))

def pow_m_normal(x, n, m) :
    res = 1
    for i in range(n) :
        res *= x
    return (res%m)


def pow_m_fast(x, n, m) :
    d = 1
    while n > 0:
        if n % 2 == 1 :
            d = (d * x)%m
        x = (x **2)%m
        n = n//2
    return d

def gcd(a, b) :
    while b != 0:
        a, b = b, a%b
    return a    


#扩展欧几里得非递归实现
def Exgcd(a, b) :
    x0,y0,x1,y1=1,0,0,1
    while b != 0 :
        r,q = a%b, a//b
        x,y = x0-q*x1,y0-q*y1
        x0,y0,x1,y1 = x1,y1,x,y
        a,b = b,r 
    return a,x0,y0

#扩展欧几里得递归实现
def Exgcd_recursion(a, b) :
    if b == 0 :
        return a, 1, 0
    else :
        d, x, y = Exgcd(b, a%b)
        x, y = y, x - (a//b)*y
        return d, x, y

def Multiply_all(X) :
    r = 1
    for x in X :
        r *= x
    return r

#求a的乘法逆元（mod m）
def Inverse(a, m) :
    return (Exgcd(a, m)[1])

def CRT(a, m, k) :
    M = Multiply_all(m)
    T = [(M//m[i]) * Inverse(M//m[i], m[i]) * a[i] for i in range(k)]
    print(T)
    x = sum(T)%M
    return x




def test_CRT():
    print("CRT: x = 2(mod 3), x = 3(mod 5), x = 2 (mod 7)")
    a = [2,3,2]
    m = [3,5,7]
    print("x = " +  str(CRT(a,m,3)))


def test_gcd() :
    print("gcd : 1160718174, 316258250")
    start = time.clock()
    res = gcd(1160718174, 316258250)
    #res = gcd(1759, 550)
    end = time.clock()
    print("gcd(1160718174, 316258250) = " + str(res))
    print("time :" + str(end - start))

    print("\n\n")

    print("Exgcd （非递归版）: 1160718174, 316258250")
    start = time.clock()
    res = Exgcd(1160718174, 316258250)
    #res = Exgcd(1759, 550)
    end = time.clock()
    print("Exgcd(1160718174, 316258250) = " + str(res))
    print("time :" + str(end - start))

    print("\n\n")

    print("Exgcd（递归版） : 1160718174, 316258250")
    start = time.clock()
    res = Exgcd_recursion(1160718174, 316258250)
    #res = Exgcd(1759, 550)
    end = time.clock()
    print("Exgcd(1160718174, 316258250) = " + str(res))
    print("time :" + str(end - start))

def test_Erato() :
    print("Eratosthenes : 100 ")
    start = time.clock()
    res = Erato(100)
    end = time.clock()
    print(res)
    print("time :" + str(end - start))

def test_pow() :
    print("pow_m_normal : 111^7000 % 13")
    start = time.clock()
    res = pow_m_normal(111, 7000, 13)
    end = time.clock()
    print("111^7000 % 13 = " + str(res))
    print("time :" + str(end - start))

    print("\n\n")

    print("pow_m_fast : 111^7000 % 13")
    start = time.clock()
    res = pow_m_fast(111, 7000, 13)
    end = time.clock()
    print("111^7000 % 13 = " + str(res))
    print("time :" + str(end - start))

def main() :
    test_Erato()
    print("\n\n\n")
    test_gcd()
    print("\n\n\n")
    test_pow()
    print("\n\n\n")
    test_CRT()
    

if __name__ == '__main__':
    main()
