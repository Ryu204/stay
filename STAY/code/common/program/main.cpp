#include <filesystem>
#include <iostream>

#include <json/json.h>

#include "application.hpp"
#include "asset/folderWatcher.hpp"
#include "asset/asset.hpp"
class Ass : public stay::asset::Asset
{
    public:
        Ass()
            : stay::asset::Asset{std::filesystem::current_path()/"asset", "data/out"}
        {}
        void load() override
        {
            std::cout << "Loaded" << std::endl;
        }
    protected:
        void onModify() override
        {
            std::cout << "overrided" << std::endl;
        }

        void onMove(const std::filesystem::path& newRelative) override
        {
            std::cout << "moved to " << newRelative << std::endl;
        }
};
int main()
{
    try
    {
        Ass ass{};
        using fw = stay::asset::FolderWatcher;
        fw watch{std::filesystem::current_path()/"asset"};
        watch.add(ass);
        while(true)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Escape))
                break;
            watch.flushChanges();
        }
        // stay::program::Application app;
        // app.run();
        return 0;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }
    catch(...)
    {
        std::cerr << "Unknown error!\n";
        return -2;
    }
}