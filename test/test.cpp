#include "libcpp-html-to-md.hpp"
#include <gtest/gtest.h>

using namespace lklibs;

TEST(ConversionTest, HttpGetRequestMustBeCompletedSuccessfullyInItsSimplestForm)
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

    const auto targetMarkdownContent = "Test# Simple H1 Header\n\nSimple Paragraph\n\n## Another H2 Header\n\n1. First item\n2. Second item\n\n| Row 1, Cell 1 | Row 1, Cell 2 | \n| ------------- | ------------- | \n| Row 2, Cell 1 | Row 2, Cell 2 | \n\n";

    MarkdownConverter mdConverter(htmlContent);

    const auto markdownContent = mdConverter.convert();

    ASSERT_FALSE(markdownContent.empty()) << "Markdown content is empty";
    ASSERT_EQ(markdownContent, targetMarkdownContent) << "Markdown content is invalid";
}
