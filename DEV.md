# Common C++ Concepts

### 1. **`std::vector`**
`std::vector` is a dynamic array container provided by the C++ Standard Library. Unlike regular arrays in C++, which have a fixed size, a `std::vector` can grow or shrink dynamically as needed. It handles memory allocation and deallocation automatically, making it a safer and more flexible alternative to raw arrays when dealing with dynamic sizes.

- **Key Features**:
  - **Dynamic resizing**: You can add or remove elements from a `std::vector`, and it will adjust its size automatically.
  - **Contiguous memory**: Elements are stored in contiguous memory locations, providing efficient access by index.
  - **Efficient appends**: Adding elements to the end of a `std::vector` is usually efficient. However, inserting or removing elements in the middle of the vector can be more costly.

Example usage:
```cpp
#include <vector>
#include <iostream>

int main() {
    std::vector<int> vec;
    vec.push_back(10);  // Adds 10 to the end
    vec.push_back(20);  // Adds 20 to the end

    std::cout << "First element: " << vec[0] << std::endl;  // Access by index
    std::cout << "Second element: " << vec[1] << std::endl;

    return 0;
}
```

### 2. **`std::initializer_list`**
`std::initializer_list` is a template class in the C++ Standard Library that allows you to pass a list of values to a constructor or function in a clean and readable way. It is commonly used to initialize containers like `std::vector` with multiple values using curly braces `{}`.

- **Key Features**:
  - Often used in constructors to initialize containers with multiple values.
  - It is a lightweight object providing access to a constant array of elements.
  
Example usage:
```cpp
#include <initializer_list>
#include <iostream>

void print_list(std::initializer_list<int> list) {
    for (int num : list) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}

int main() {
    print_list({1, 2, 3, 4, 5});
    return 0;
}
```
In this example, `{1, 2, 3, 4, 5}` is an initializer list passed to the `print_list` function.

### 3. **Virtual Function**
A **virtual function** in C++ is a function that is declared in a base class with the intention of being overridden in derived classes. When a virtual function is called through a pointer or reference to a base class, C++ uses runtime polymorphism to decide which function to call based on the actual object type, not the type of the pointer/reference.

- **Key Features**:
  - Declared with the `virtual` keyword in the base class.
  - If a derived class overrides the virtual function, it is called instead of the base class's version.
  - Used when the behavior of a function can vary depending on the actual object type.

Example usage:
```cpp
#include <iostream>

class Base {
public:
    virtual void speak() { // Virtual function
        std::cout << "Base speaking!" << std::endl;
    }
};

class Derived : public Base {
public:
    void speak() override { // Overriding the virtual function
        std::cout << "Derived speaking!" << std::endl;
    }
};

int main() {
    Base* basePtr;
    Derived derivedObj;

    basePtr = &derivedObj;

    // Calls Derived::speak because the function is virtual and basePtr points to a Derived object
    basePtr->speak();

    return 0;
}
```
In this example, the `speak()` function is virtual in the base class. Even though the pointer is of type `Base*`, it calls the `speak()` method of the `Derived` class because the actual object is of type `Derived`.

### 4. Templating in C++
Templating in C++ allows you to write generic, reusable code that works with any data type. This is done using **function templates** and **class templates**, which are defined using the `template` keyword.

**Key Points:**
1. **Function Templates**: Write a function that works with any type.
   ```cpp
   template <typename T> void swapValues(T &a, T &b);
   ```

2. **Class Templates**: Define classes that work with any type.
   ```cpp
   template <typename T> class Box { T value; };
   ```

3. **Template Specialization**: Customize the template for specific types.
   ```cpp
   template <> void print<char*>(char* value);
   ```

4. **Variadic Templates**: Handle an arbitrary number of template arguments.
   ```cpp
   template <typename... Args> void print(Args... args);
   ```

**Benefits of Templating in C++**:
- **Code Reusability**: Write once, use for any type.
- **Type Safety**: Ensures operations are type-safe.
- **Efficiency**: Enables compile-time polymorphism, avoiding runtime overhead.
- **Flexibility**: Works with user-defined types as well.


#### References for C++:
- [Vector Documentation - cplusplus.com](https://cplusplus.com/reference/vector/vector/)
- [realloc Documentation - cplusplus.com](https://en.cppreference.com/w/c/memory/realloc)
- [iostream Documentation - cplusplus.com](https://cplusplus.com/reference/istream/iostream/)
- [Virtual Function - Wikipedia](https://en.wikipedia.org/wiki/Virtual_function)
- [Templating in C++ - GeeksForGeeks](https://www.geeksforgeeks.org/templates-cpp/)

# Common C Concepts

### 1. **`realloc()`**
`realloc()` is a function in C (and C++) that allows you to resize a previously allocated block of memory. It can either increase or decrease the size of a memory block. The function returns a pointer to the resized memory block.

- **Key Features**:
  - `realloc()` resizes a previously allocated memory block.
  - If the block can't be resized in place, it may return a new memory location. Therefore, it's important to assign the result to the original pointer (or a new one).
  - Defined in `<stdlib.h>` (C) and `<cstdlib>` (C++).

Example usage:
```cpp
#include <iostream>
#include <cstdlib>

int main() {
    // Initial allocation
    int* arr = (int*)malloc(5 * sizeof(int));
    
    // Resize the memory block to hold 10 integers
    arr = (int*)realloc(arr, 10 * sizeof(int));
    
    // Fill and print the array
    for (int i = 0; i < 10; ++i) {
        arr[i] = i + 1;
        std::cout << arr[i] << " ";
    }

    free(arr); // Don't forget to free the memory when done.
    return 0;
}
```
In this example:
- `malloc()` initially allocates memory for 5 integers.
- `realloc()` resizes the memory block to accommodate 10 integers.
- `free()` releases the allocated memory when done.

#### References for C:
- [Reallocate Memory in C - W3Schools](https://www.w3schools.com/c/c_memory_reallocate.php)
- [Typedef in C - Wikipedia](https://en.wikipedia.org/wiki/Typedef)
