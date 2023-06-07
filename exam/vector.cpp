template<typename T>
std::vector<T> vectorsum(const std::vector<T>& left, const std::vector<T>& right) {
    std::vector<T> result;
    result.reserve(left.size());

    std::transform(left.begin(), left.end(), right.begin(), std::back_inserter(result), std::plus<T>());

    return result;
}