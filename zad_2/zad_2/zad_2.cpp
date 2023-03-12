#include <iostream>
#include <vector>
#include <algorithm>
#include "range.h"


void testrange(const range& rng, int start, const std::vector<bool>& expected)
{
    for (size_t step = 0; step < expected.size(); ++step)
        if (expected[step] != rng(start + step))
        {
            std::cout << "Error with range " << rng << " at value " << start + step << std::endl;
        }
}

int main()
{
    range rng(3, 5);
    testrange(rng, 2, { false, false, true, false, false });
    std::cout << rng << '\n';

    lcrange lcrng(3, 5);
    testrange(lcrng, 2, { false, true, true, false, false });
    std::cout << lcrng << '\n';

    rcrange rcrng(3, 5);
    testrange(rcrng, 2, { false, false, true, true, false });
    std::cout << rcrng << '\n';

    bcrange bcrng(3, 5);
    testrange(bcrng, 2, { false, true, true, true, false });
    std::cout << bcrng << '\n';

    std::cout << "End of tests.\n";
    return 0;
}
