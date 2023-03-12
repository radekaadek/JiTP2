#include <iostream>
#include <vector>
#include "range.h"
using namespace std;

ostream& operator<<(ostream& os, const range& rng)
{
    return os << rng.left_paren() << rng.low() << ", "
            << rng.high() << rng.right_paren();
}

void testrange(const range& rng, int start, const vector<bool>& expected)
{
    for (size_t step = 0; step < expected.size(); ++step)
        if (expected[step] != rng(start + step))
        {
            cout << "Error with range " << rng << " at value " << start + step << endl;
        }
}

int main()
{
    range rng(3, 5);
    testrange(rng, 2, {false, false, true, false, false});
    cout << rng << '\n';

    lcrange lcrng(3, 5);
    testrange(lcrng, 2, {false, true, true, false, false});
    cout << lcrng << '\n';

    rcrange rcrng(3, 5);
    testrange(rcrng, 2, {false, false, true, true, false});
    cout << rcrng << '\n';

    bcrange bcrng(3, 5);
    testrange(bcrng, 2, {false, true, true, true, false});
    cout << bcrng << '\n';

    cout << "End of tests.\n";
    return 0;
}
