import itertools

"""
Problem Statement

Let's call a permutation p of length n anti-Fibonacci if the condition pi−2+pi−1≠pi holds for all i (3≤i≤n). Recall that the permutation is the array of length n which contains each integer from 1 to n exactly once.
Your task is for a given number n print n distinct anti-Fibonacci permutations of length n.

Input

The first line contains a single integer t (1≤t≤48) — the number of test cases.

The single line of each test case contains a single integer n (3≤n≤50).

Output

For each test case, print n lines. Each line should contain an anti-Fibonacci permutation of length n. In each test case, you cannot print any permutation more than once.

If there are multiple answers, print any of them. It can be shown that it is always possible to find n different anti-Fibonacci permutations of size n under the constraints of the problem.

Example
Input
2
4
3
Output
4 1 3 2
1 2 4 3
3 4 1 2
2 4 1 3
3 2 1
1 3 2
3 1 2
"""


def check_fib(lis):
    for i in range(2, len(lis)):
        if lis[i-2] + lis[i-1] == lis[i]:
            return False
    return True


tests = int(input())
numbers = []
for i in range(tests):
    numbers.append(int(input()))


for i in range(tests):
    number = numbers[i]
    nums = list(i+1 for i in reversed(range(number)))
    count = 0
    for perm in itertools.permutations(nums):
        if count == number:
            break
        if check_fib(perm):
            count += 1
            print(*perm)
