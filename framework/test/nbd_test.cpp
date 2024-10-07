#include "nbd.hpp"
#include "nbd_command_write.hpp"
#include "nbd_command_read.hpp"
#include "nbd_command_flush.hpp"
#include "nbd_command_trim.hpp"
#include "nbd_command_disc.hpp"
#include "nbd_key_task.hpp"
#include "nbd_proxy.hpp"

//./busexmp 128M /dev/nbd0
int main(int argc, char const *argv[])
{
    try
    {    
        (void) argc;
        std::vector<CPF::p_recipes> recipes;
        std::vector<CPF::t_listened_inputs> listened_inputs;
        std::string nbd_path =  argv[1];
        size_t size = 4 * 1024 * 1024;
        NBD nbd(size, nbd_path);
        nbd.ListenNbd();

        int fd = nbd.GetFd();

        recipes.push_back(CPF::p_recipes(NBD_CMD_WRITE, []() { return std::make_shared<NBDCommandWrite>(); }));
        recipes.push_back(CPF::p_recipes(NBD_CMD_READ, []() { return std::make_shared<NBDCommandRead>(); }));
        recipes.push_back(CPF::p_recipes(NBD_CMD_FLUSH, []() { return std::make_shared<NBDCommandFlush>(); }));
        recipes.push_back(CPF::p_recipes(NBD_CMD_TRIM, []() { return std::make_shared<NBDCommandTrim>(); }));
        recipes.push_back(CPF::p_recipes(NBD_CMD_DISC, []() { return std::make_shared<NBDCommandDisc>(); }));

        std::shared_ptr<IInputProxy> input_proxy1 = std::make_shared<NbdProxy>(fd, nbd);

        listened_inputs.push_back(CPF::t_listened_inputs(fd, CPF::WRITE, input_proxy1));
        listened_inputs.push_back(CPF::t_listened_inputs(fd, CPF::READ, input_proxy1));

        CPF cpf(recipes, listened_inputs, "/home/yiratromano/git/projects/final_project/framework/plugins");

        cpf.Run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }


    return 0;
}