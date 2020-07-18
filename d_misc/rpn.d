//
// Reverse Polish calculator, D2.
//
//  https://en.wikipedia.org/wiki/Reverse_Polish_notation
//
// Example: the "natural" infix notation expressing
//      5 + 3 * 2
// is normally interpreted as
//      5 + (3 * 2)     == 11
// as per operator precedence rules. Unfortunately this means that the user
// needs to add parentheses for expressing the alternative intent:
//      (5 + 3) * 2     == 16
// However by using RPN, either one of the above can be expressed unambiguously
// without parentheses:
//      5 3 2 * +       == 11
//      5 3 + 2 *       == 16
//

import std.array;
import std.conv;
import std.getopt;
import std.stdio;

enum string validOps = "+-*/%";
alias OpndType = real;

void printHelp(string name)
{
    writeln("Reverse Polish calculator");
    write("supported operations:");

    foreach (o; validOps)
    {
        write(" ", o);
    }

    writeln("\ntype of operands: ", OpndType.stringof);
    writefln("usage examples:\n\t%s 5 3 + 2 *\n\t%s 12.1 11 /\n\t%s -6 -5 *",
             name, name, name);
    writeln("NOTE: proper spacing is of utmost importance!");
}

void main(string[] args)
{
    if (getopt(args, std.getopt.config.passThrough).helpWanted
            || args.length == 1)
    {
        printHelp(args[0]);
        return;
    }

    string spacedArgs = args[1 .. $].join(" ");
    OpndType[] stack;

    foreach (op; spacedArgs.split)
    {
LabeledSwitch:
        switch (op) // operator or operand?
        {
            static foreach (o; validOps)
            {
            case [o]:
                stack = stack[0 .. $ - 2] ~
                        mixin("stack[$ - 2]" ~ o ~ "stack[$ - 1]");
                break LabeledSwitch;
            }

        default:
            // assume it's an operand
            stack ~= to!OpndType(op);
            break;
        }
    }

    writeln(stack.front);
}
