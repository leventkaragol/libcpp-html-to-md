# libcpp-html-to-md

High performance, easy to use, HTML to Markdown conversion library for C++ (17+)

[![linux](https://github.com/leventkaragol/libcpp-html-to-md/actions/workflows/linux.yml/badge.svg)](https://github.com/leventkaragol/libcpp-html-to-md/actions/workflows/linux.yml)
[![windows](https://github.com/leventkaragol/libcpp-html-to-md/actions/workflows/windows.yml/badge.svg)](https://github.com/leventkaragol/libcpp-html-to-md/actions/workflows/windows.yml)


> [!TIP]
> Please read this document before using the library. I know, you don't have time but reading 
> this document will save you time. I mean just this file, it's not long at all. Trial and error 
> will cost you more time.


# Table of Contents

* [How to add it to my project](#how-to-add-it-to-my-project)
* [How to use? (Simplest Form)](#how-to-use-simplest-form)
* [Semantic Versioning](#semantic-versioning)
* [Full function list](#full-function-list)
* [License](#license)
* [Contact](#contact)

## How to add it to my project?

This is a header only library. So actually, all you need is to add the libcpp-html-to-md.hpp file
in src folder to your project and start using it with #include.

But this library is uses libxml2 under the hood. So, you also need to add libxml2 to
your project before to use it.

You can find usage examples in the examples folder, also find a sample CMakeLists.txt file content below.

```cmake
cmake_minimum_required(VERSION 3.14)

project(myProject)

find_package(libxml2 REQUIRED)

add_executable(myProject main.cpp libcpp-html-to-md.hpp)

target_include_directories(myProject PRIVATE ${LIBXML2_INCLUDE_DIR})
target_link_libraries(myProject PRIVATE ${LIBXML2_LIBRARIES})

```

> [!IMPORTANT]
> In order to run the library on Windows, the Iconv library is also needed. The CMakeLists.txt file under the examples
> folder contains the necessary additions to work on both platforms.


## How to use? (Simplest Form)

You can call the **"convert"** method of the **"MarkdownConverter"** class to convert the HTML content you have  
as a string variable to Markdown format without any customization.

```cpp
#include "libcpp-html-to-md.hpp"

using namespace lklibs;

int main() {

    const auto htmlContent = R"(
    <html>
        <head>
            <title>Test</title>
        </head>
        <body>
            <h1>Simple H1 Header</h1>
            <p>Simple Paragraph</p>
            <h2>Another H2 Header</h2>
            <ol>
                <li>First item</li>
                <li>Second item</li>
            </ol>

            <table>
                <tr>
                    <td>Row 1, Cell 1</td>
                    <td>Row 1, Cell 2</td>
                </tr>
                <tr>
                    <td>Row 2, Cell 1</td>
                    <td>Row 2, Cell 2</td>
                </tr>
            </table>
        </body>
    </html>
    )";
    
    MarkdownConverter mdConverter(htmlContent);

    const auto markdownContent = mdConverter.convert();

    std::cout << markdownContent << std::endl;

    return 0;
}
```

## Semantic Versioning

Versioning of the library is done using conventional semantic versioning. Accordingly,
in the versioning made as **MAJOR.MINOR.PATCH**;

**PATCH:** Includes possible Bug&Fixes and improvements. You definitely want to get this.

**MINOR:** Additional functionality added via backwards compatibility. You probably want to
get this, it doesn't hurt.

**MAJOR:** Additional functionality that breaks backwards compatibility. You'll need to know
what's changed before you get it, and you'll probably have to make changes to your own code.
If I publish something like this, I will definitely add the changes required for migration
section to the documentation.

## Full function list

You can find the complete list of functions in the library below.

> [!TIP]
> All functions and parameters descriptions are also available within the code as comment for IDEs.

```cpp
std::string convert();
```

## License

MIT License

Copyright (c) 2024 Levent KARAGÖL

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

## Contact

If you have problems regarding the library, please open an
[issue on GitHub](https://github.com/leventkaragol/libcpp-html-to-md/issues/new).
Please describe your request, issue, or question in as much detail as possible
and also include the version of your compiler and operating system, as well as
the version of the library you are using. Before opening a new issue, please
confirm that the topic is not already exists in closed issues.