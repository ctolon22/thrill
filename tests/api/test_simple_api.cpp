#include "gtest/gtest.h"
#include "c7a/api/dia.hpp"
#include "c7a/api/context.hpp"
#include "c7a/api/function_stack.hpp"

TEST(DISABLED_DIASimple, InputTest1ReadInt) {
     //auto read_int = [](std::string line) { return std::stoi(line); };

     //c7a::Context ctx;

     // auto initial = ctx.ReadFromFileSystem("tests/inputs/test1", read_int);

     // assert(initial.NodeString() == "[DIANode/State:NEW/Type:i]");

     // assert(initial.Size() == 4);
}

TEST(DISABLED_DIASimple, InputTest1ReadDouble) {
     //auto read_double = [](std::string line) { return std::stod(line); };

     //c7a::Context ctx;

     // auto initial = ctx.ReadFromFileSystem("tests/inputs/test1", read_double);

     // assert(initial.NodeString() == "[DIANode/State:NEW/Type:d]");

     // assert(initial.Size() == 4);

}

TEST(DISABLED_DIASimple, InputTest1Write) {

     //auto read_int = [](std::string line) { return std::stoi(line); };
     //auto write_int = [](int element) { return element; };

     //c7a::Context ctx;

     // auto initial = ctx.ReadFromFileSystem("tests/inputs/test1", read_int);
     // ctx.WriteToFileSystem(initial, "tests/inputs/test1_result", write_int);
     // auto copy = ctx.ReadFromFileSystem("tests/inputs/test1_result", read_int);

     // assert(copy.NodeString() == "[DIANode/State:NEW/Type:i]");

     // assert(copy.Size() == 4);
}

TEST(DIASimple, ReduceStringEquality) {

    using c7a::DIA;
    using c7a::Context;
    using c7a::FunctionStack;

    auto doubles = Context().ReadFromFileSystem("tests/inputs/test1", [](std::string line) {
            return std::stod(line);
        });

    auto key_ex = [](double in) { return (int) in; };
    auto red_fn = [](double in1, double in2) { return in1 + in2; };
    auto map_fn = [](double input) {
            std::cout << "Map" << std::endl;
            return input;
        };
    auto map2_fn = [](double input) {
            std::cout << "Map2" << std::endl;
            return input;
        };
    auto fmap_fn = [](double input, std::function<void(double)> emit_func) {
            std::cout << "FlatMap" << std::endl;
            emit_func(input);
            emit_func(input);
        };

    auto duplicates = doubles.Map(map_fn);
    auto duplicates2 = duplicates.Map(map_fn);
    auto red_duplicates = duplicates2.Reduce(key_ex, red_fn);
    auto red_duplicates2 = duplicates.Reduce(key_ex, red_fn);

    std::cout << "==============" << std::endl;
    std::cout << "FunctionStack" << std::endl;
    std::cout << "==============" << std::endl;
    FunctionStack<> stack;
    auto new_stack = stack.push(map_fn);
    auto new_stack2 = new_stack.push(map2_fn);
    auto pair = new_stack2.pop();
    pair.first(1.0);
    std::cout << std::endl;

    std::cout << "==============" << std::endl;
    std::cout << "Tree" << std::endl;
    std::cout << "==============" << std::endl;
    duplicates.PrintNodes();
    std::cout << std::endl;

    std::cout << "==============" << std::endl;
    std::cout << "Execution" << std::endl;
    std::cout << "==============" << std::endl;
    std::cout << "First Reduce:" << std::endl;
    (red_duplicates.get_node())->execute();
    std::cout << std::endl;
    std::cout << "Second Reduce:" << std::endl;
    (red_duplicates2.get_node())->execute();
    // auto duplicates3 = red_duplicates.Map(map_fn);
    // auto red_duplicates2 = duplicates3.Reduce(key_ex, red_fn);
}
