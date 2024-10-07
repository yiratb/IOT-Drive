/****************************************************                                                       
 *  Written by: Yirat Romano                         
 *                                                  
 *  Reviewed by: Alon                        
 *                                                  
 *  Completed on - 01/07/24                        
 *                                                  
 ****************************************************/
#include <iostream>

#include "dir_monitor.hpp"
#include "../test/plugins_test.hpp"
#include "factory.hpp"

int main()
{
    try
    {
        Factory<Base, size_t> *f = Singleton<Factory<Base, size_t>>::GetInstance();
        std::cout << "Address in main" << f << std::endl;
        std::string plugins_dir = "/home/gil/Downloads/yirat.romano-master-projects-final_project/projects/final_project/framework/plugins";

        DirMonitor monitor(plugins_dir);

        DllLoader observer1;
        monitor.GetDispatcher().Subscribe(&observer1);

        monitor.StartMonitor();

        std::this_thread::sleep_for(std::chrono::seconds(10)); // Run for 10 seconds for testing

        std::shared_ptr<Base> obj = f->Create(3);
        obj->Print();

    } 
    catch (const std::exception& e) 
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

