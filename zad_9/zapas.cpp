#include <iostream>
#include <vector>
#include <array>

/**
 * @brief Test a function with a set of inputs and outputs
 * 
 * @tparam Fn type of the function to be tested
 * @tparam RAContainer type of a container with random access
 * @param fTested function to be tested
 * @param in container of inputs
 * @param out container of outputs
 * @param name optional name of the function to be tested
 * @return true if all tests passed
 * @return false if some tests failed
 * @throws std::invalid_argument if the size of the input and output vectors is different
 */
template <typename Fn, typename RAContainer>
bool test(const Fn fTested, const RAContainer in, const RAContainer out, const std::string name = "") {
    if (in.size() != out.size()) {
        const std::string msg = "Input and output containers must have the same size";
        throw std::invalid_argument(msg);
    }
    
    bool passed = true;
    for (size_t i = 0; i < in.size(); i++) {
        auto retv = fTested(in[i]);
        if (retv != out[i]) {
            std::cout << "\n--- Test failed ---\n";
            if (name != "") {
                std::cout << "Function: " << name << "\n";
            }
            std::cout << "Input: " << in[i] << "\n";
            std::cout << "Expected output: " << out[i] << "\n";
            std::cout << "Actual output: " << retv << "\n";
            std::cout << "-------------------\n";
            passed = false;
        }
    }
    return passed;
}

// a function that return the square of a number
template <typename T>
T square(T x) {
    return x * x;
}

int main() {

    // test the square function
    std::vector<int> in = {1, 2, 3, 4, 5};
    std::vector<int> out = {1, 4, 9, 16, 20};
    test(square<int>, in, out , "square");
    std::array<int, 5UL> in2 = {1, 2, 3, 4, 5};
    std::array<int, 5UL> out2 = {1, 4, 9, 16, 20};
    test(square<int>, in2, out2 , "square");

    return 0;
}
