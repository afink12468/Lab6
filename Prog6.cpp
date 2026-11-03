#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::vector;
using std::string;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;

bool success = false;
bool findall = false;
bool verbose = false;

void show_solution(vector<int> &numbers, vector<int> &operations) { // this function applies the necessary operation to each number starting at index [0]. Then the result is printed and verbose is called if necessary.

  int result = numbers[0];
  for (size_t i = 0; i < operations.size(); ++i) {
    if (operations[i] == 0) result += numbers[i + 1];
    else if (operations[i] == 1) result -= numbers[i + 1];
    else if (operations[i] == 2) result *= numbers[i + 1];
  }

  if (verbose) {
    cout << numbers[0];
    for (size_t i = 0; i < operations.size(); ++i) {
      if (operations[i] == 0) cout << "+";
      else if (operations[i] == 1) cout << "-";
      else if (operations[i] == 2) cout << "*";
      cout << numbers[i + 1];
    }
    cout << "=" << result << " ";
  }

  for (size_t i = 0; i < operations.size(); ++i) {
    if (operations[i] == 0) cout << "+";
    else if (operations[i] == 1) cout << "-";
    else if (operations[i] == 2) cout << "*";
  }
  cout << endl;
}

bool find_solution(vector<int> &numbers, vector<int> &operations, int k = 0, int fvalue = 0) {  // this function is a backtracking search. when k == 0 the operations vector get cleared. A different operation is used for each iteration and a running total is kept.
  if (k == (int)numbers.size() - 1) {
    int target = numbers.back();
    if (fvalue == target) {
      success = true;
      show_solution(numbers, operations);
      if (!findall) {
        return true;
      }
    }
    return false;
  }

  if (k == 0) {
    operations.clear();
    return find_solution(numbers, operations, 1, numbers[0]);
  }

  int next_num = numbers[k];
  int next_k = k + 1;

  operations.push_back(0);
  if (find_solution(numbers, operations, next_k, fvalue + next_num)) return true;
  operations.pop_back();

  operations.push_back(1);
  if (find_solution(numbers, operations, next_k, fvalue - next_num)) return true;
  operations.pop_back();

  operations.push_back(2);
  if (find_solution(numbers, operations, next_k, fvalue * next_num)) return true;
  operations.pop_back();

  return false;
}

int main(int argc, char *argv[]) { // the main function checks for command line options and the verbose option. All numbers are read in from stdin. inorder runs the search and anyorder sorts the number.
  if (argc < 3 || argc > 4) {
    cerr << "Usage: ./Prog6 -inorder|-anyorder -1|-N [-verbose] < numbers.txt" << endl;
    return 1;
  }

  string order_mode = argv[1];
  string find_mode = argv[2];

  if (order_mode != "-inorder" && order_mode != "-anyorder") {
    cerr << "Error: Invalid order argument. Use -inorder or -anyorder." << endl;
    return 1;
  }

  if (find_mode == "-N") {
    findall = true;
  } else if (find_mode == "-1") {
    findall = false;
  } else {
    cerr << "Error: Invalid find mode argument. Use -1 or -N." << endl;
    return 1;
  }

  if (argc == 4) {
    if (string(argv[3]) == "-verbose") {
      verbose = true;
    } else {
      cerr << "Error: Invalid optional argument. Use -verbose." << endl;
      return 1;
    }
  }

  vector<int> numbers;
  int val;
  while (cin >> val) {
    numbers.push_back(val);
  }

  if (numbers.empty()) {
    cerr << "Error: No numbers provided." << endl;
    return 1;
  }

  vector<int> operations;

  if (order_mode == "-inorder") {
    find_solution(numbers, operations);
  } else if (order_mode == "-anyorder") {
    std::sort(numbers.begin(), numbers.end() - 1);
    do {
      find_solution(numbers, operations);
    } while (std::next_permutation(numbers.begin(), numbers.end() - 1));
  }

  return 0;
}