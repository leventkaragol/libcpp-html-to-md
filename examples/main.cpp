#include "libcpp-html-to-md.hpp"

using namespace lklibs;

void simpleHTMLtoMDConversion()
{
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
}

int main()
{
    simpleHTMLtoMDConversion();
}
