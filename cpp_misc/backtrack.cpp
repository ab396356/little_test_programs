//
// Backtracking.
//
//  https://en.wikipedia.org/wiki/Backtracking
//  https://en.wikipedia.org/wiki/Call_stack
//  https://en.wikipedia.org/wiki/Recursion_(computer_science)
//  https://en.wikipedia.org/wiki/Stack_(abstract_data_type)
//  https://en.wikipedia.org/wiki/Queue_(abstract_data_type)
//  https://en.wikipedia.org/wiki/Depth-first_search
//  https://en.wikipedia.org/wiki/Breadth-first_search
//
// "Backtracking" is a technique used to find a solution (e.g. a password) when
// validity constraints are known (e.g. the password can contain at most three
// decimal digits).
//
// The constraints will be used to reject sets of invalid candidates, thereby
// speeding up the search process; the algorithm will skip generating candidates
// which are known to not satisfy the constraints.
//
// In contrast, a brute force algorithm will generate all possible candidates.
// (A backtracking algorithm which never rejects any candidates becomes a brute
// force algorithm.)
//
// Backtracking algorithms are often implemented using recursion. This is done
// because it simplifies the code in languages such as C, whereby the programmer
// uses a (call) stack "for free" to hold the algorithm's state, i.e. the old
// candidates to which the algorithm needs to "backtrack" in order to generate
// their children candidates.
//
// -----------------------------------------------------------------------------
//
// The basic C-style pseudocode for recursive backtracking is:
//
//  ///
//  /// @brief Performs recursive backtracking.
//  /// @param [in] c               Current candidate to be checked.
//  ///
//  void backtrack(candidate c)
//  {
//      //
//      // c isn't a solution, and its children cannot
//      // become solutions either, so go no further
//      //
//      if (reject(c))
//          return;
//
//      //
//      // c is a solution!
//      //
//      if (accept(c))
//          /* use, save, print, etc. */ ;
//
//      //
//      // does c have a child? i.e. "001" -> "001a"
//      // if not, c_child gets LINEAGE_IS_OVER
//      //
//      // (in practice "LINEAGE_IS_OVER" can be any special value,
//      // such as NULL, that is interpreted as the lack of a child)
//      //
//      candidate c_child = first_child(c);
//
//      while (c_child != LINEAGE_IS_OVER)
//      {
//          //
//          // verify the child candidate, and its children, and so on...
//          //
//          backtrack(c_child);
//
//          //
//          // does c_child have a sibling? i.e. "001a" -> "001b"
//          // if not, c_child gets LINEAGE_IS_OVER
//          //
//          c_child = next_child(c_child);
//      }
//  }
//
// -----------------------------------------------------------------------------
//
// The basic C-style pseudocode for non-recursive backtracking is:
//
//  ///
//  /// @brief Performs non-recursive backtracking.
//  /// @param [in] start           Beginning candidate, to start with.
//  ///
//  void backtrack(candidate start)
//  {
//      //
//      // because recursion isn't used anymore, we do not get a Call Stack
//      // in which to hold our current state "for free" so we must "manually"
//      // use a Stack container, that implements the following operations:
//      //
//      //  void        stack::push(candidate)  // adds a new "c" to the top
//      //  void        stack::pop()            // removes the current top
//      //  candidate   stack::top()            // returns the current top
//      //  bool        stack::empty()          // returns if the stack is empty
//      //
//      // note that the container isn't technically required to be a Stack;
//      // for instance it could be a Queue but then the algorithm will perform
//      // a "Breadth-First Search" (BFS) which means it will accumulate a large
//      // number of children (in our case 60+ million) and use a lot of memory
//      // (1+ GB) compared to the Stack version, which will perform a
//      // "Depth-First Search" (DFS) and use much less memory (~500 KB)
//      //
//      stack<candidate> stk;
//
//      stk.push(start);
//
//      while (!stk.empty())
//      {
//          if (reject(stk.top()))
//          {
//              stk.pop();
//              continue;
//          }
//
//          if (accept(stk.top()))
//              /* use, save, print, etc. */ ;
//
//          candidate child = first_child(stk.top());
//
//          stk.pop();
//
//          if (child != LINEAGE_IS_OVER)
//          {
//              stk.push(child);
//
//              candidate sibling = next_child(stk.top());
//
//              while (sibling != LINEAGE_IS_OVER)
//              {
//                  stk.push(sibling);
//                  sibling = next_child(stk.top());
//              }
//          }
//      }
//  }
//
// -----------------------------------------------------------------------------
//
// The following program will use backtracking to find all passwords with the
// following example constraints:
//
//  (1) only lowercase latin letters (a-z) and decimal digits (0-9) are allowed
//  (2) there must be at least two digits in a password
//  (3) there must be at least one letter 'b' in a password
//  (4) the minimum length of a password is 3
//  (5) the maximum length of a password is 5
//
// The program implements:
//
//  (a) traditional recursive backtrack:        backtrack_rec()
//  (b) stack-based non-recursive backtrack:    backtrack_stk()
//  (c) queue-based non-recursive backtrack:    backtrack_que()
//

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <locale>
#include <new>
#include <queue>
#include <stack>
#include <stdexcept>
#include <string>

namespace
{

///
/// @brief Returns whether a candidate and its children should be rejected.
/// @details Rejection occurs if the candidate:
///  (1) contains characters which aren't either lowercase latin letter (a-z)
///      or decimal digit (0-9), such as 'A' or '?', OR
///  (2) is at least of length 5 yet it doesn't contain at least two digits, OR
///  (3) is at least of length 5 yet it doesn't contain at least one letter 'b'
/// @note The function `first_child()` shall not generate candidate strings
///  of length greater than 5.
/// @param [in] c                   Candidate to be verified.
/// @returns Whether or not rejection has occurred.
/// @retval true                    The candidate was rejected.
/// @retval false                   The candidate was not rejected.
///
bool reject(const std::string &c)
try
{
    for (char ch: c)
    {
        if (!std::islower(ch, std::locale::classic()) &&
            !std::isdigit(ch, std::locale::classic()))
        {
            return true;
        }
    }

    if (c.length() >= 5)
    {
        if (std::count(c.begin(), c.end(), 'b') < 1 ||
            std::count_if(c.begin(), c.end(),
                [](const char &ch) -> bool
                {
                    return std::isdigit(ch, std::locale::classic());
                }) < 2)
        {
            return true;
        }
    }

    return false;
}
catch (const std::bad_alloc &e)
{
    std::cerr << "`std::bad_alloc` exception in `" << __func__ << "`: ";
    std::cerr << e.what() << std::endl;
    throw;
}
catch (const std::runtime_error &e)
{
    std::cerr << "`std::runtime_error` exception in `" << __func__ << "`: ";
    std::cerr << e.what() << std::endl;
    throw;
}
catch (...)
{
    std::cerr << "unknown exception in `" << __func__ << '`' << std::endl;
    throw;
}

///
/// @brief Returns if a candidate is accepted as a solution.
/// @details Acceptance occurs if the candidate:
///  (0) wasn't rejected previously (this is a precondition), AND
///  (1) has minimum length 3, AND
///  (2) has maximum length 5, AND
///  (3) contains at least two digits, AND
///  (4) contains at least one letter 'b'
/// @pre `reject(c) == false`
/// @param [in] c                   Candidate to be verified.
/// @returns Whether or not acception has occurred.
/// @retval true                    The candidate was accepted.
/// @retval false                   The candidate was not accepted.
///
bool accept(const std::string &c)
try
{
    assert(reject(c) == false);

    if (c.length() >= 3 &&
        c.length() <= 5 &&
        std::count(c.begin(), c.end(), 'b') >= 1 &&
        std::count_if(c.begin(), c.end(),
            [](const char &ch) -> bool
            {
                return std::isdigit(ch, std::locale::classic());
            }) >= 2)
    {
        return true;
    }

    return false;
}
catch (const std::bad_alloc &e)
{
    std::cerr << "`std::bad_alloc` exception in `" << __func__ << "`: ";
    std::cerr << e.what() << std::endl;
    throw;
}
catch (const std::runtime_error &e)
{
    std::cerr << "`std::runtime_error` exception in `" << __func__ << "`: ";
    std::cerr << e.what() << std::endl;
    throw;
}
catch (...)
{
    std::cerr << "unknown exception in `" << __func__ << '`' << std::endl;
    throw;
}

///
/// @brief The set of valid characters to be used in a new candidate.
/// @details This global serves to "specialize" the candidate child and sibling
///  generation functions `first_child()` and `next_child()`.
///
const std::string valid_chars("abcdefghijklmnopqrstuvwxyz0123456789");

///
/// @brief Retrieves the first child of the candidate.
/// @details The child is returned as a pointer but must be dereferenced and
///  stored as a string as soon as it's retrieved, if not null.
/// @todo Use `std::optional` from C++17 in the future?
/// @pre `valid_chars.empty() == false`
/// @param [in] c                   Candidate for which to get the child.
/// @returns Pointer to the child.
/// @retval nullptr                 If there are no children.
///
const std::string * first_child(const std::string &c)
try
{
    assert(valid_chars.empty() == false);

    static std::string child;

    if (c.length() >= 5)
        return nullptr;

    child = c + valid_chars.front();
    return &child;
}
catch (...)
{
    std::cerr << "unknown exception in `" << __func__ << '`' << std::endl;
    throw;
}

///
/// @brief Retrieves the next sibling of the candidate.
/// @details The sibling is returned as a pointer but must be dereferenced and
///  stored as a string as soon as it's retrieved, if not null.
/// @todo Use `std::optional` from C++17 in the future?
/// @pre `valid_chars.empty() == false`
/// @pre `c.empty() == false`
/// @param [in] c                   Candidate for which to get the sibling.
/// @returns Pointer to the sibling.
/// @retval nullptr                 If there are no siblings.
///
const std::string * next_child(const std::string &c)
try
{
    assert(valid_chars.empty() == false);
    assert(c.empty() == false);

    static std::string sibling;

    if (c.back() == valid_chars.back())
        return nullptr;

    sibling = c;
    sibling.back() = valid_chars.at(valid_chars.find(c.back()) + 1);
    return &sibling;
}
catch (const std::out_of_range &e)
{
    std::cerr << "`std::out_of_range` exception in `" << __func__ << "`: ";
    std::cerr << e.what() << std::endl;
    throw;
}
catch (...)
{
    std::cerr << "unknown exception in `" << __func__ << '`' << std::endl;
    throw;
}

///
/// @brief Performs recursive backtracking.
/// @param [in] c                   Current candidate to be checked.
///
void backtrack_rec(const std::string &c)
try
{
    if (reject(c))
        return;

    if (accept(c))
        std::cout << c << '\n';

    const std::string *p_child(first_child(c));

    while (p_child != nullptr)
    {
        const std::string c_child(*p_child);

        backtrack_rec(c_child);
        p_child = next_child(c_child);
    }
}
catch (...)
{
    std::cerr << "unknown exception in `" << __func__ << '`' << std::endl;
    throw;
}

///
/// @brief Performs non-recursive backtracking, using a Stack.
/// @param [in] start               Beginning candidate, to start with.
///
void backtrack_stk(const std::string &start)
try
{
    std::stack<std::string> stk;

    stk.push(start);

    while (!stk.empty())
    {
        if (reject(stk.top()))
        {
            stk.pop();
            continue;
        }

        if (accept(stk.top()))
            std::cout << stk.top() << '\n';

        const std::string *p_child(first_child(stk.top()));

        stk.pop();

        if (p_child != nullptr)
        {
            stk.push(*p_child);

            const std::string *p_sibling(next_child(stk.top()));

            while (p_sibling != nullptr)
            {
                stk.push(*p_sibling);
                p_sibling = next_child(stk.top());
            }
        }
    }
}
catch (...)
{
    std::cerr << "unknown exception in `" << __func__ << '`' << std::endl;
    throw;
}

///
/// @brief Performs non-recursive backtracking, using a Queue.
/// @warning This function is very memory intensive!
/// @param [in] start               Beginning candidate, to start with.
///
void backtrack_que(const std::string &start)
try
{
    std::queue<std::string> que;

    que.push(start);

    while (!que.empty())
    {
        if (reject(que.front()))
        {
            que.pop();
            continue;
        }

        if (accept(que.front()))
            std::cout << que.front() << '\n';

        const std::string *p_child(first_child(que.front()));

        que.pop();

        if (p_child != nullptr)
        {
            que.push(*p_child);

            const std::string *p_sibling(next_child(que.back()));

            while (p_sibling != nullptr)
            {
                que.push(*p_sibling);
                p_sibling = next_child(que.back());
            }
        }
    }
}
catch (...)
{
    std::cerr << "unknown exception in `" << __func__ << '`' << std::endl;
    throw;
}

} // unnamed namespace

///
/// @brief Enters the program.
/// @returns Whether or not the operation was successful.
/// @retval EXIT_SUCCESS            Operation success.
/// @retval EXIT_FAILURE            Operation failure.
///
int main()
try
{
    backtrack_rec("");
//  backtrack_stk("");
//  backtrack_que(""); // WARN: uses lots of memory!
    return EXIT_SUCCESS;
}
catch (...)
{
    std::cerr << "unknown exception in `" << __func__ << '`' << std::endl;
    return EXIT_FAILURE;
}
