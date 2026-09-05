#include <cstring>
#include <iostream>
#include <math/math.hpp>
#include <task.hpp>
#include <unistd.h>
#include <application.hpp>


void printHelp() {
  constexpr int pathBufferSize = 1024;
  char pathBuffer[pathBufferSize];

  const ssize_t length = readlink("/proc/self/exe", pathBuffer, sizeof(pathBuffer) - 1);

  if (length == -1) {
    std::cout << "Failed to get executable path\n";
    return;
  }

  pathBuffer[length] = '\0';

  char* lastSlash = strrchr(pathBuffer, '/');

  if (lastSlash == nullptr) {
    std::cout << "Incorrect Path \n";
    return;
  }

  strcpy(lastSlash + 1, "help.txt");

  FILE *file = fopen(pathBuffer, "r");
  if (file == nullptr) {
    std::cout << "Failed to read file \n";
    return;
  }
  constexpr int helpBufferSize = 256;
  char helpBuffer[helpBufferSize];

  while (fgets(helpBuffer, sizeof(helpBuffer), file) != nullptr) {
    std::cout << helpBuffer;
  }
  fclose(file);
}

bool makeTask(int argc, char *argv[], Task &task) {
  int option = getopt(argc, argv, "a:b:o:h");
  bool hasA = false;
  bool hasB = false;
  bool hasO = false;
  while (option != -1) {
    switch (option) {
      case 'a': {
        char *endptr = nullptr;
        hasA = true;
        constexpr int decimalBase = 10;
        const long value = strtol(optarg, &endptr, decimalBase);
        if (*endptr == '\0' && value <= INT_MAX && value >= INT_MIN) {
          task.firstNumber = static_cast<int>(value);
        } else {
          std::cout << "Incorrect argument value" << "\n";
          return false;
        }
        break;
      }
      case 'b': {
        char *endptr = nullptr;
        hasB = true;
        const long value = strtol(optarg, &endptr, 10);
        if (*endptr == '\0' && value <= INT_MAX && value >= INT_MIN) {
          task.secondNumber = static_cast<int>(value);
        } else {
          std::cout << "Incorrect argument value" << "\n";
          return false;
        }
        break;
      }
      case 'o':
        hasO = true;
        if (strcmp(optarg, "add") == 0) {
          task.operation = Operation::ADD;
        } else if (strcmp(optarg, "mul") == 0) {
          task.operation = Operation::MULTIPLY;
        } else if (strcmp(optarg, "div") == 0) {
          task.operation = Operation::DIVIDE;
        } else if (strcmp(optarg, "fact") == 0) {
          task.operation = Operation::FACTORIAL;
        } else if (strcmp(optarg, "sub") == 0) {
          task.operation = Operation::SUBTRACT;
        } else if (strcmp(optarg, "pow") == 0) {
          task.operation = Operation::POWER;
        } else {
          std::cout << "Unknown operation" << "\n";
          return false;
        }
        break;
      case 'h':
        printHelp();
        return false;
      default:
        std::cout << "Unknown parameter, check manual" << "\n";
        return false;
    }
    option = getopt(argc, argv, "a:b:o:h");
  }
  if (optind != argc) {
    std::cout << "Too many arguments" << "\n";
    return false;
  }
  if (hasA && hasB && hasO && task.operation != Operation::FACTORIAL) {
    return true;
  }
  if (hasA && hasO && task.operation == Operation::FACTORIAL) {
    return true;
  }
  return false;
}

void makeCalculate(Task &task) {
  switch (task.operation) {
    case Operation::ADD: {
      math::Result const tempResult = math::add(task.firstNumber, task.secondNumber);
      task.result = tempResult.result;
      task.status = tempResult.status;
      break;
    }
    case Operation::MULTIPLY: {
      math::Result const tempResult = math::multiply(task.firstNumber, task.secondNumber);
      task.result = tempResult.result;
      task.status = tempResult.status;
      break;
    }
    case Operation::DIVIDE: {
      math::Result const tempResult = math::divide(task.firstNumber, task.secondNumber);
      task.result = tempResult.result;
      task.status = tempResult.status;
      break;
    }
    case Operation::SUBTRACT: {
      math::Result const tempResult = math::subtract(task.firstNumber, task.secondNumber);
      task.result = tempResult.result;
      task.status = tempResult.status;
      break;
    }
    case Operation::POWER: {
      math::Result const tempResult = math::power(task.firstNumber, task.secondNumber);
      task.result = tempResult.result;
      task.status = tempResult.status;
      break;
    }
    case Operation::FACTORIAL: {
      math::Result const tempResult = math::factorial(task.firstNumber);
      task.result = tempResult.result;
      task.status = tempResult.status;
      break;
    }
  }
}

void printResult(const Task &task) {
  switch (task.status) {
    case math::Status::SUCCESS:
      std::cout << task.result << "\n";
      break;
    case math::Status::OVERFLOW:
      std::cout << "Overflow!" << "\n";
      break;
    case math::Status::DIVISION_BY_ZERO:
      std::cout << "Division by zero not allowed." << "\n";
      break;
    case math::Status::NEGATIVE_POWER:
      std::cout << "Negative power." << "\n";
      break;
    case math::Status::NEGATIVE_FACTORIAL:
      std::cout << "Negative factorial." << "\n";
      break;
  }
}


void applicationRun(int argc, char *argv[]) {
  Task task{};
  if (makeTask(argc, argv, task)) {
    makeCalculate(task);
    printResult(task);
  }
}
