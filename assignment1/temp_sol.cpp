/*
Clock Test Results :

vectorsize: 50.000n        10.000n       5.000n
-------------------------------------------------------------- -
LargeType Result : 199399 ms      8974.16 ms    2307.85 ms
LargeTypeRaw Result : 0.498291 ms    0.030385 ms   0.035032 ms
-------------------------------------------------------------- -
*/


#include <iostream>
#include <vector>
#include <ctime>

// Assignment Part 1

using namespace std;

template <typename T>
class LargeType {
public:
    explicit LargeType(int initialSize = 10)
        : data(initialSize) {}

    int getSize() const {

        return data.size();
    }
    // Operator '<' LargeType 
    bool operator<(const LargeType<T>& rhs) {
        return data.size() < rhs.data.size();
    }

private:
    vector<T> data;
};

// Assignment Part 2 -3
template <typename T>
class LargeTypeRaw {
public:
    explicit LargeTypeRaw(int size)
        : dataRaw(new T[size]), sizeRaw(size) {}

    // LargeTypeRaw Copy constructor
    LargeTypeRaw(const LargeTypeRaw& rhs) noexcept
        : dataRaw(new int[rhs.sizeRaw]), sizeRaw(rhs.sizeRaw)
    {
        copy(rhs.dataRaw, rhs.dataRaw + sizeRaw, dataRaw);
    }

    // LargeTypeRaw Move constructor
    LargeTypeRaw(LargeTypeRaw<T>&& rhs) noexcept
        : dataRaw(rhs.dataRaw), sizeRaw(rhs.sizeRaw)
    {
        rhs.dataRaw = nullptr;
        rhs.sizeRaw = 0;
    }

    // LargeTypeRaw Copy assignment operator '= '
    LargeTypeRaw& operator=(const LargeTypeRaw <T>& rhs) noexcept {
        if (this != &rhs) {

            int* newData = new T[rhs.sizeRaw];
            copy(rhs.dataRaw, rhs.dataRaw + rhs.sizeRaw, newData);
            delete[] dataRaw;
            dataRaw = newData;
            sizeRaw = rhs.sizeRaw;
        }
        return *this;

        // LargeTypeRaw Smaller than operator  ' <' 
    }
    bool operator<(LargeTypeRaw <T>& rhs) {
        return this->sizeRaw < rhs.getSize();
    }
    // LargeTypeRaw Move assignment operator
    LargeTypeRaw& operator=(LargeTypeRaw<T>&& rhs) noexcept {
        if (this != &rhs) {
            delete[] dataRaw;
            dataRaw = rhs.dataRaw;
            sizeRaw = rhs.sizeRaw;
            rhs.dataRaw = nullptr;
            rhs.sizeRaw = 0;
        }
        return *this;
    }

    // LargeTypeRaw  Destructor
    ~LargeTypeRaw() {
        delete[] dataRaw;
    }

    int getSize() const
    {
        return sizeRaw;
    }

private:
    int* dataRaw;
    int sizeRaw;
};

//Insertion sort algorithm with vector
template <typename T>
void insertSort(vector <T>& arr, int arrSize) {

    for (int i = 1; i < arrSize; i++) {
        T temp = arr[i];
        int j = i - 1;
        while (j >= 0 && temp < arr[j]) {
            arr[j + 1] = move(arr[j]);
            j--;
        }
        arr[j + 1] = move(temp);
    }
}


int main() {

    // Assignment Part 4- Testing and comparing the Copy and Move Constructor performances 

    constexpr int vecSize = 20000;

    // Filling the vector arrays with random numbers 1-100
    vector<LargeType<int>> vecLarge;
    vector<LargeTypeRaw <int>> vecRaw;
    for (int i = 0; i < vecSize; i++) {

        int randNum = rand() % 100;
        vecLarge.push_back(LargeType<int>(randNum));

        vecRaw.push_back(LargeTypeRaw <int>(randNum));
    }

    // Setting the start time
    clock_t start1 = clock();

    //Calling the sorting function and printing out the elapsed execution time 
    insertSort(vecLarge, vecSize);
    cout << "Sorting LargeType Result: " << (clock() - start1) / (float)(CLOCKS_PER_SEC / 1000) << " ms" << endl;

    clock_t start2 = clock(); // Reset the start time for the second sort function call
    insertSort(vecRaw, vecSize);
    cout << "Sorting LargeTypeRaw Result: " << (clock() - start2) / (float)(CLOCKS_PER_SEC / 1000) << " ms" << endl;


    for (int i = 1; i < vecSize; i++) {

        if (vecLarge[i - 1].getSize() <= vecLarge[i].getSize() && vecRaw[i - 1].getSize() <= vecRaw[i].getSize())

            continue;

        cout << " not sorted  ";
    }

    return 0;
}