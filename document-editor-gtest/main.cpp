#include <iostream>

#include "application.hpp"
#include "command.hpp"
#include <boost/di.hpp>

using namespace std;
namespace di = boost::di;

int main_1()
{
    Terminal terminal;
    Document doc;

    Application app{terminal};

    app.add_command(PrintCmd::ID, std::make_shared<PrintCmd>(doc, terminal));
    app.add_command(AddText::ID, std::make_shared<AddText>(doc, terminal));

    return app.run();
}

int main()
{
    Terminal terminal;

    auto injector = di::make_injector(
        di::bind<Console>.to(terminal)
    );

    auto app = injector.create<Application>();
    app.add_command(PrintCmd::ID, injector.create<std::shared_ptr<PrintCmd>>());
    app.add_command(AddText::ID, injector.create<std::shared_ptr<AddText>>());

    app.run();
}