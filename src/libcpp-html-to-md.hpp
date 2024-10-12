/*

High performance, easy to use, HTML to Markdown conversion library for C++ (17+)
version 1.0.0
https://github.com/leventkaragol/libcpp-html-to-md

If you encounter any issues, please submit a ticket at https://github.com/leventkaragol/libcpp-html-to-md/issues

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

*/

#ifndef LIBCPP_HTML_TO_MD
#define LIBCPP_HTML_TO_MD


#include <libxml/HTMLparser.h>
#include <libxml/tree.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <memory>
#include <filesystem>
#include <optional>

namespace lklibs
{
    /**
     * @brief Custom deleter for smart pointer
     */
    struct XmlDocDeleter
    {
        void operator()(xmlDoc* doc) const
        {
            if (doc) xmlFreeDoc(doc);
        }
    };

    /**
     * @brief HTML to Markdown converter class
     */
    class MarkdownConverter
    {
    public:
        /**
         * @brief Constructor for the HttpRequest class
         *
         * @param htmlContent: html content for the conversion
         */
        explicit MarkdownConverter(const std::string& htmlContent)
        {
            this->htmlContent = htmlContent;
        }

        /**
         * @brief Parses the given HTML data and creates Markdown content according to the specified specifications.
         *
         * @return Markdown content
         */
        std::string convert()
        {
            markdown.clear();

            const std::unique_ptr<xmlDoc, XmlDocDeleter> doc(
                htmlReadMemory(htmlContent.c_str(), static_cast<int>(htmlContent.size()), nullptr, nullptr, HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING)
            );

            if (!doc)
            {
                throw std::runtime_error("Could not read HTML content.");
            }

            xmlNode* root = xmlDocGetRootElement(doc.get());

            traverse(root);

            return markdown;
        }

    private:
        std::string htmlContent;
        std::string markdown;

        void traverse(xmlNode* node)
        {
            for (xmlNode* current = node; current != nullptr; current = current->next)
            {
                if (current->type == XML_ELEMENT_NODE)
                {
                    std::string tagName = reinterpret_cast<const char*>(current->name);

                    if (tagName.empty())
                    {
                        continue;
                    }

                    if (auto tagContent = handleElement(current, tagName); tagContent.has_value())
                    {
                        markdown += tagContent.value();
                    }
                    else
                    {
                        traverse(current->children);
                    }
                }
                else if (current->type == XML_TEXT_NODE)
                {
                    std::string content = reinterpret_cast<const char*>(current->content);

                    content.erase(content.find_last_not_of(" \n\r\t") + 1);

                    markdown += content;
                }
                else
                {
                    traverse(current->children);
                }
            }
        }

        std::optional<std::string> handleElement(xmlNode* current, const std::string& tagName)
        {
            std::string result;

            if (tagName.find('h') == 0 && tagName.size() == 2 && isdigit(tagName[1]))
            {
                const int level = tagName[1] - '0';

                result = std::string(level, '#') + " " + getNodeContent(current) + "\n\n";
            }
            else if (tagName == "p")
            {
                result = getNodeContent(current) + "\n\n";
            }
            else if (tagName == "strong" || tagName == "b")
            {
                result = "**" + getNodeContent(current) + "**";
            }
            else if (tagName == "em" || tagName == "i")
            {
                result = "*" + getNodeContent(current) + "*";
            }
            else if (tagName == "a")
            {
                const auto href = getAttribute(current, "href");
                const auto text = getNodeContent(current);

                result = "[" + text + "](" + href + ")";
            }
            else if (tagName == "img")
            {
                const auto src = getAttribute(current, "src");
                const auto alt = getAttribute(current, "alt");

                result = "![" + alt + "](" + src + ")";
            }
            else if (tagName == "ul" || tagName == "ol")
            {
                const bool ordered = (tagName == "ol");

                traverseList(current, ordered);

                result = "\n";
            }
            else if (tagName == "pre")
            {
                result = "```\n" + getNodeContent(current) + "\n```\n\n";
            }
            else if (tagName == "code")
            {
                result = "`" + getNodeContent(current) + "`";
            }
            else if (tagName == "table")
            {
                traverseTable(current);

                result = "\n";
            }

            if (!result.empty())
            {
                return result;
            }

            return std::nullopt;
        }

        void traverseList(xmlNode* listNode, bool ordered, int depth = 0)
        {
            int index = 1;

            for (xmlNode* item = listNode->children; item != nullptr; item = item->next)
            {
                if (item->type == XML_ELEMENT_NODE && std::string(reinterpret_cast<const char*>(item->name)) == "li")
                {
                    std::string prefix(depth * 2, ' ');

                    if (ordered)
                    {
                        prefix += std::to_string(index++) + ". ";
                    }
                    else
                    {
                        prefix += "- ";
                    }

                    markdown += prefix + getNodeContent(item) + "\n";

                    for (xmlNode* child = item->children; child != nullptr; child = child->next)
                    {
                        if (child->type == XML_ELEMENT_NODE)
                        {
                            if (std::string childName = reinterpret_cast<const char*>(child->name); childName == "ul" || childName == "ol")
                            {
                                traverseList(child, childName == "ol", depth + 1);
                            }
                        }
                    }
                }
            }
        }

        void traverseTable(const xmlNode* tableNode)
        {
            std::vector<std::vector<std::string>> rows;
            std::vector<size_t> columnWidths;

            for (xmlNode* child = tableNode->children; child != nullptr; child = child->next)
            {
                if (child->type == XML_ELEMENT_NODE)
                {
                    if (std::string childName = reinterpret_cast<const char*>(child->name); childName == "thead" || childName == "tbody" || childName == "tfoot")
                    {
                        for (xmlNode* tr = child->children; tr != nullptr; tr = tr->next)
                        {
                            if (tr->type == XML_ELEMENT_NODE && std::string(reinterpret_cast<const char*>(tr->name)) == "tr")
                            {
                                auto currentRow = extractRow(tr);

                                if (!currentRow.empty())
                                {
                                    rows.push_back(currentRow);

                                    updateColumnWidths(currentRow, columnWidths);
                                }
                            }
                        }
                    }
                    else if (childName == "tr")
                    {
                        if (auto currentRow = extractRow(child); !currentRow.empty())
                        {
                            rows.push_back(currentRow);

                            updateColumnWidths(currentRow, columnWidths);
                        }
                    }
                }
            }

            if (rows.empty())
            {
                return;
            }

            const auto& header = rows[0];

            std::vector<std::string> separator(columnWidths.size(), "");

            for (size_t i = 0; i < columnWidths.size(); ++i)
            {
                separator[i] = std::string(columnWidths[i], '-');
            }

            markdown += "| ";

            for (size_t i = 0; i < header.size(); ++i)
            {
                markdown += padString(header[i], columnWidths[i]) + " | ";
            }

            markdown += "\n";

            markdown += "| ";

            for (const auto& sep : separator)
            {
                markdown += sep + " | ";
            }

            markdown += "\n";

            for (size_t r = 1; r < rows.size(); ++r)
            {
                markdown += "| ";

                for (size_t c = 0; c < rows[r].size(); ++c)
                {
                    markdown += padString(rows[r][c], columnWidths[c]) + " | ";
                }

                markdown += "\n";
            }
        }

        std::vector<std::string> extractRow(xmlNode* tr)
        {
            std::vector<std::string> currentRow;

            for (xmlNode* cell = tr->children; cell != nullptr; cell = cell->next)
            {
                if (cell->type == XML_ELEMENT_NODE)
                {
                    if (std::string cellName = reinterpret_cast<const char*>(cell->name); cellName == "td" || cellName == "th")
                    {
                        currentRow.emplace_back(getNodeContent(cell));
                    }
                }
            }

            return currentRow;
        }

        static void updateColumnWidths(const std::vector<std::string>& row, std::vector<size_t>& columnWidths)
        {
            if (row.size() > columnWidths.size())
            {
                columnWidths.resize(row.size(), 0);
            }
            for (size_t i = 0; i < row.size(); ++i)
            {
                columnWidths[i] = std::max(columnWidths[i], row[i].size());
            }
        }

        [[nodiscard]] static std::string padString(const std::string& str, const size_t width)
        {
            if (str.size() >= width)
            {
                return str;
            }

            return str + std::string(width - str.size(), ' ');
        }

        std::string getNodeContent(const xmlNode* node)
        {
            std::ostringstream oss;

            for (xmlNode* current = node->children; current != nullptr; current = current->next)
            {
                if (current->type == XML_ELEMENT_NODE)
                {
                    std::string tagName = reinterpret_cast<const char*>(current->name);

                    if (tagName.empty())
                    {
                        continue;
                    }

                    if (tagName == "strong" || tagName == "b")
                    {
                        oss << "**" << getNodeContent(current) << "**";
                    }
                    else if (tagName == "em" || tagName == "i")
                    {
                        oss << "*" << getNodeContent(current) << "*";
                    }
                    else if (tagName == "a")
                    {
                        auto href = getAttribute(current, "href");

                        auto text = getNodeContent(current);

                        oss << "[" + text + "](" + href + ")";
                    }
                    else if (tagName == "img")
                    {
                        auto src = getAttribute(current, "src");

                        auto alt = getAttribute(current, "alt");

                        oss << "![" << alt << "](" << src << ")";
                    }
                    else if (tagName == "code")
                    {
                        oss << "`" << getNodeContent(current) << "`";
                    }
                    else
                    {
                        oss << getNodeContent(current);
                    }
                }
                else if (current->type == XML_TEXT_NODE)
                {
                    std::string content = reinterpret_cast<const char*>(current->content);

                    content.erase(content.find_last_not_of(" \n\r\t") + 1);

                    oss << content;
                }
            }

            return oss.str();
        }

        static std::string getAttribute(const xmlNode* node, const char* attrName)
        {
            if (xmlChar* attr = xmlGetProp(node, reinterpret_cast<const xmlChar*>(attrName)))
            {
                std::string value = reinterpret_cast<const char*>(attr);

                xmlFree(attr);

                return value;
            }

            return "";
        }
    };
}

#endif //LIBCPP_HTML_TO_MD
