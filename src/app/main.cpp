#include "core/Application.h"

#include <exception>
#include <iostream>

int main()
{
    try
    {
        gk1::Application app;
        return app.run();
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Fatal error: " << ex.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "Fatal error: unknown exception\n";
    }

    return EXIT_FAILURE;
}