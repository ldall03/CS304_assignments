#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>
#include <stdexcept>

template <typename T>
class LargeType {

public:
    LargeType(int size = 10)
        : vec(std::vector<T>(size)) {}

    int get_size() const {
        return this->vec.size();
    }

    bool operator<(const LargeType& rhs) {
        return (vec.size() < rhs.get_size());
    }

private:
    std::vector<T> vec;
};

template <typename T>
class LargeTypeRaw {
public:
    // Default Constructor
    explicit LargeTypeRaw(int size = 10) 
        : size{ size }, arr{ new T[size] }
    {
        // std::cout << "Normal constructor called.\n";
    }

    // Destructor
    ~LargeTypeRaw() {
        delete[] this->arr;
        // std::cout << "Destructor called.\n";
    }

    // Copy constructor
    LargeTypeRaw(const LargeTypeRaw& other) 
        : size{ other.size }, arr{ new T[size] }
    {
        std::copy(other.arr, other.arr + this->size, this->arr);
        // std::cout << "Copy constructor called.\n";
    }

    // Move constructor
    LargeTypeRaw(LargeTypeRaw&& other) noexcept
        : size{ other.size }, arr{ other.arr }
    {
        other.arr = nullptr;
        // std::cout << "Move constructor called.\n";
    }

    // Copy assignment operator
    LargeTypeRaw& operator=(const LargeTypeRaw& other) {
        // std::cout << "Copy assignment operator called.\n";

        if (&other != this) {
            delete[] this->arr;
            this->size = other.size;
            this->arr = new T[this->size];
            std::copy(other.arr, other.arr + this->size, this->arr);
        }

        return *this;
    }

    // Move assignment operator
    LargeTypeRaw& operator=(LargeTypeRaw&& other) noexcept {
        // std::cout << "Move cassignment operator called.\n";

        if (&other != this) {
            delete[] this->arr;
            this->size = other.size;
            this->arr = other.arr;
            other.arr = nullptr;
        }

        return *this;
    }

    bool operator<(const LargeTypeRaw& rhs) {
        return (this->size < rhs.get_size());
    }

    int get_size() const {
        return this->size;
    }

private:
    int size;
    T* arr;

};


template <typename T>
void insertion_sort(std::vector<T>& v) {
    for (int j = 1; j < v.size(); j++) {
        auto key = v[j];
        int i = j - 1;
        while (i > -1 && key < v[i]) {
            // Using move here greatly improves performance
            v[i + 1] = std::move(v[i]);
            i--;
        }
        v[i + 1] = std::move(key);
    }
}

int _main()  // temp main for testing big 5
{
    LargeTypeRaw<int> l1 = LargeTypeRaw<int>{ 5 };  // Normal constructor
    LargeTypeRaw<int> l2 = LargeTypeRaw<int>{ 10 };  // Normal constructor
    LargeTypeRaw<int> l3 = l1;  // Copy constructor
    LargeTypeRaw<int> l4 = std::move(l1);  // Move constructor

    l2 = l4;  // Copy assignment
    l3 = std::move(l2);  // Move assignment

    return 0;
}

int main()
{
    const int SIZE = 50000;
    std::vector<LargeType<int>> LT(SIZE);
    std::vector<LargeTypeRaw<int>> LTR(SIZE);

    srand(time(nullptr));
    // Initialize vectors
    for (int i = 0; i < SIZE; i++) {
        // Make sure both vectors have the same values for most precise results
        int rand_val = rand() % 1000;
        LT[i] = LargeType<int>(rand_val);
        LTR[i] = LargeTypeRaw<int>(rand_val);
    }

    // Start clock
    auto start = std::chrono::high_resolution_clock::now();
    insertion_sort(LT);
    // Stop clock
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Time for LargeType: " << duration.count() << " ms\n";

    // Start clock
    auto start_raw = std::chrono::high_resolution_clock::now();
    insertion_sort(LTR);
    // Stop clock
    auto stop_raw = std::chrono::high_resolution_clock::now();
    auto duration_raw = std::chrono::duration_cast<std::chrono::milliseconds>(stop_raw - start_raw);
    std::cout << "Time for LargeTypeRaw: " << duration_raw.count() << " ms\n";

    // Make sure lists are sorted
    for (int i = 1; i < SIZE; i++) {
        if (LT[i - 1].get_size() <= LT[i].get_size() && LTR[i - 1].get_size() <= LTR[i].get_size())
            continue;

        throw;
    }

    return 0;
}
