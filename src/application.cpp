#include <unistd.h>

#include <application.hpp>
#include <math/math.hpp>
#include <task.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>
namespace
{
constexpr const char* helpText = R"(cpp_calc — console calculator

DESCRIPTION
    Simple command-line calculator for integer numbers.

USAGE
    cpp_calc -a <number> -b <number> -o <operation>

OPTIONS
    -a <number>
        First integer number.

    -b <number>
        Second integer number.
        This option is ignored for the factorial operation.

    -o <operation>
        Operation to perform.

    -h
        Show this help message.

OPERATIONS
    add
        Addition.

    sub
        Subtraction.

    mul
        Multiplication.

    div
        Division.

    pow
        Raise the first number to the power of the second number.

    fact
        Calculate the factorial of the first number.

EXAMPLES
    cpp_calc -a 10 -b 20 -o add
    cpp_calc -a 20 -b 5 -o div
    cpp_calc -a 2 -b 10 -o pow
    cpp_calc -a 5 -o fact

ERRORS
    Division by zero is not allowed.
    Negative powers are not allowed.
    Factorial of a negative number is not defined.
    Integer overflow is detected and reported.
    Invalid command-line arguments are rejected.
)";
} //namespace

void printHelp()
{
    puts(helpText);
}

bool parseHelper(const char* arg, int& number)
{
    char* endptr = nullptr;
    constexpr int decimalBase = 10;
    const long value = strtol(arg, &endptr, decimalBase);
    if (*endptr == '\0' && value <= INT_MAX && value >= INT_MIN)
    {
        number = static_cast<int>(value);
        return true;
    }

    puts("Incorrect argument value");
    return false;
}

bool makeTask(int argc, char* argv[], Task& task)
{
    int option = getopt(argc, argv, "a:b:o:h");
    bool hasA = false;
    bool hasB = false;
    bool hasO = false;
    while (option != -1)
    {
        switch (option)
        {
            case 'a':
            {
                if (!parseHelper(optarg, task.firstNumber))
                {
                    return false;
                }
                hasA = true;
                break;
            }
            case 'b':
            {
                if (!parseHelper(optarg, task.secondNumber))
                {
                    return false;
                }
                hasB = true;
                break;
            }
            case 'o':
                hasO = true;
                if (strcmp(optarg, "add") == 0)
                {
                    task.operation = Operation::ADD;
                }
                else if (strcmp(optarg, "mul") == 0)
                {
                    task.operation = Operation::MULTIPLY;
                }
                else if (strcmp(optarg, "div") == 0)
                {
                    task.operation = Operation::DIVIDE;
                }
                else if (strcmp(optarg, "fact") == 0)
                {
                    task.operation = Operation::FACTORIAL;
                }
                else if (strcmp(optarg, "sub") == 0)
                {
                    task.operation = Operation::SUBTRACT;
                }
                else if (strcmp(optarg, "pow") == 0)
                {
                    task.operation = Operation::POWER;
                }
                else
                {
                    puts("Unknown operation");
                    return false;
                }
                break;
            case 'h':
                printHelp();
                return false;
            default:
                puts("Unknown parameter, check manual");
                return false;
        }
        option = getopt(argc, argv, "a:b:o:h");
    }
    if (optind != argc)
    {
        puts("Too many arguments");
        return false;
    }
    if (!hasA || !hasO)
    {
        puts("Missing operation or first argument");
        return false;
    }

    if (task.operation == Operation::FACTORIAL)
    {
        return true;
    }

    if (!hasB)
    {
        puts("Second argument is required for this operation");
        return false;
    }

    return true;
}

void makeCalculate(Task& task)
{
    switch (task.operation)
    {
        case Operation::ADD:
        {
            math::Result const tempResult =
                math::add(task.firstNumber, task.secondNumber);
            task.result = tempResult.result;
            task.status = tempResult.status;
            break;
        }
        case Operation::MULTIPLY:
        {
            math::Result const tempResult =
                math::multiply(task.firstNumber, task.secondNumber);
            task.result = tempResult.result;
            task.status = tempResult.status;
            break;
        }
        case Operation::DIVIDE:
        {
            math::Result const tempResult =
                math::divide(task.firstNumber, task.secondNumber);
            task.result = tempResult.result;
            task.status = tempResult.status;
            break;
        }
        case Operation::SUBTRACT:
        {
            math::Result const tempResult =
                math::subtract(task.firstNumber, task.secondNumber);
            task.result = tempResult.result;
            task.status = tempResult.status;
            break;
        }
        case Operation::POWER:
        {
            math::Result const tempResult =
                math::power(task.firstNumber, task.secondNumber);
            task.result = tempResult.result;
            task.status = tempResult.status;
            break;
        }
        case Operation::FACTORIAL:
        {
            math::Result const tempResult = math::factorial(task.firstNumber);
            task.result = tempResult.result;
            task.status = tempResult.status;
            break;
        }
    }
}

void printResult(const Task& task)
{
    switch (task.status)
    {
        case math::Status::SUCCESS:
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
            printf("%d\n", task.result);
            break;
        case math::Status::OVERFLOW:
            puts("Overflow!");
            break;
        case math::Status::DIVISION_BY_ZERO:
            puts("Division by zero not allowed.");
            break;
        case math::Status::NEGATIVE_POWER:
            puts("Negative power.");
            break;
        case math::Status::NEGATIVE_FACTORIAL:
            puts("Negative factorial.");
            break;
    }
}

void applicationRun(int argc, char* argv[])
{
    Task task{};
    if (makeTask(argc, argv, task))
    {
        makeCalculate(task);
        printResult(task);
    }
}
