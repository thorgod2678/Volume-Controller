#include "parser.h"

#include <cwchar>
#include <cmath>
#include <stdexcept>
#include <string>


class Parser
{
private:

    const wchar_t* input;

public:

    Parser(const wchar_t* text)
        : input(text)
    {}

    float Parse()
    {
        float result = ParseExpression();

        SkipWhitespace();

       
        if (*input != L'\0')
            throw std::runtime_error("Unexpected characters");

        return result;
    }


private:

   

    void SkipWhitespace()
    {
        while (*input == L' ' ||
            *input == L'\t')
        {
            input++;
        }
    }


    bool Match(wchar_t c)
    {
        SkipWhitespace();

        if (*input == c)
        {
            input++;
            return true;
        }

        return false;
    }


    

    float ParseExpression()
    {
        float value = ParseTerm();

        while (true)
        {
            if (Match(L'+'))
            {
                value += ParseTerm();
            }
            else if (Match(L'-'))
            {
                value -= ParseTerm();
            }
            else
            {
                break;
            }
        }

        return value;
    }


   
    float ParseTerm()
    {
        float value = ParsePower();

        while (true)
        {
            if (Match(L'*'))
            {
                value *= ParsePower();
            }
            else if (Match(L'/'))
            {
                float divisor = ParsePower();

                if (divisor == 0.0f)
                    throw std::runtime_error("Division by zero");

                value /= divisor;
            }
            else
            {
                break;
            }
        }

        return value;
    }


    float ParsePower()
    {
        float value = ParseUnary();

        if (Match(L'^'))
        {
            float exponent = ParsePower();

            value = std::pow(value, exponent);
        }

        return value;
    }


   
    float ParseUnary()
    {
        if (Match(L'+'))
            return ParseUnary();

        if (Match(L'-'))
            return -ParseUnary();

        return ParsePrimary();
    }


   
    float ParsePrimary()
    {
        SkipWhitespace();

        
        if (Match(L'('))
        {
            float value = ParseExpression();

            if (!Match(L')'))
                throw std::runtime_error("Missing ')'");

            return value;
        }


        
        if ((*input >= L'0' && *input <= L'9') ||
            *input == L'.')
        {
            wchar_t* end;

            float value = wcstof(input, &end);

            if (end == input)
                throw std::runtime_error("Invalid number");

            input = end;

            return value;
        }


       
        if ((*input >= L'A' && *input <= L'Z') ||
            (*input >= L'a' && *input <= L'z'))
        {
            return ParseIdentifier();
        }


        throw std::runtime_error("Unexpected character");
    }


 
    float ParseIdentifier()
    {
        wchar_t name[32];

        int i = 0;

        while (((*input >= L'A' && *input <= L'Z') ||
            (*input >= L'a' && *input <= L'z')) &&
            i < 31)
        {
            name[i++] = *input;
            input++;
        }

        name[i] = L'\0';


        if (_wcsicmp(name, L"PI") == 0)
        {
            return static_cast<float>(3.14159265358979323846);
        }


        if (_wcsicmp(name, L"E") == 0)
        {
            return static_cast<float>(2.71828182845904523536);
        }


        if (!Match(L'(')) {
            throw std::runtime_error("Expected '(' after function");
        }


        float argument = ParseExpression();


        if (!Match(L')')) {
            throw std::runtime_error("Missing ')'");
        }


        if (_wcsicmp(name, L"sin") == 0) {
            return std::sin(argument * 3.14159265358979323846f / 180.0f);
        }

        else if (_wcsicmp(name, L"cos") == 0) {
            return std::cos(argument * 3.14159265358979323846f / 180.0f);
        }

        else if (_wcsicmp(name, L"tan") == 0) {
            return std::tan(argument * 3.14159265358979323846f / 180.0f);
        }
        else if (_wcsicmp(name, L"csc") == 0) {
            return 1.0f / std::sin(argument * 3.14159265358979323846f / 180.0f);
        }

        else if (_wcsicmp(name, L"sec") == 0) {
            return 1.0f / std::cos(argument * 3.14159265358979323846f / 180.0f);
        }

        else if (_wcsicmp(name, L"cot") == 0) {
            return 1.0f / std::tan(argument * 3.14159265358979323846f / 180.0f);
        }
        else if (_wcsicmp(name, L"asin") == 0) {
            return std::asin(argument) * 180.0f / 3.14159265358979323846f;
        }

        else if (_wcsicmp(name, L"acos") == 0) {
            return std::acos(argument) * 180.0f / 3.14159265358979323846f;
        }

        else if (_wcsicmp(name, L"atan") == 0) {
            return std::atan(argument) * 180.0f / 3.14159265358979323846f;
        }

        else if (_wcsicmp(name, L"sqrt") == 0) {
            return std::sqrt(argument);
        }

        else if (_wcsicmp(name, L"abs") == 0) {
            return std::fabs(argument);
        }

        else if (_wcsicmp(name, L"floor") == 0) {
            return std::floor(argument);
        }

        else if (_wcsicmp(name, L"ceil") == 0) {
            return std::ceil(argument);
        }


        throw std::runtime_error("Unknown function");
    }
};



float Parse(const wchar_t* buffer)
{
    Parser parser(buffer);

    return parser.Parse();
}