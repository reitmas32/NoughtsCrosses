#include <iostream>
//#include <core/cmp/cmp_base.tpp>
#include <Moon/include/tools/platform_info.hpp>
#include <Moon/include/tools/time_step.hpp>

#include <Moon/template/core/ent/ent_base.tpp>
#include <Moon/template/core/ent/ent.tpp>
#include <Moon/template/core/ent/ent_storage.tpp>

#include <Moon_Terminal/include/gtx.hpp>
#include <Moon_Terminal/include/input_sys.hpp>
#include <Moon_Terminal/include/render_sys.hpp>
#include <Moon_Terminal/include/position_cmp.hpp>
#include <Moon_Terminal/include/sprite_cmp.hpp>

struct Board_t : Moon::Core::Entity_t<Board_t>
{
    Board_t() : Moon::Core::Entity_t<Board_t>() {}
    Board_t(Moon::Alias::EntityId eid) : Moon::Core::Entity_t<Board_t>(eid) {}
    ~Board_t() = default;
};

int mapped_x(int pos){
    switch(pos){
        case 0:
        case 3:
        case 6:
            return 1;
        case 1:
        case 4:
        case 7:
            return 5;
        case 2:
        case 5:
        case 8:
            return 9;
    }
}

int mapped_y(int pos){
    switch(pos){
        case 0:
        case 1:
        case 2:
            return 1;
        case 3:
        case 4:
        case 5:
            return 5;
        case 6:
        case 7:
        case 8:
            return 9;
    }
}

struct Box_t : Moon::Core::Entity_t<Box_t>
{
    int pos = 0;
    Box_t() : Moon::Core::Entity_t<Box_t>() {}
    Box_t(Moon::Alias::EntityId eid, int pos) : Moon::Core::Entity_t<Box_t>(eid), pos{pos} {
    }
    ~Box_t() = default;
};

struct Enemy_t : Moon::Core::Entity_t<Enemy_t>
{
    Enemy_t() : Moon::Core::Entity_t<Enemy_t>() {}
    Enemy_t(Moon::Alias::EntityId eid) : Moon::Core::Entity_t<Enemy_t>(eid) {}
    ~Enemy_t() = default;
};

void makeGtx(Moon::Terminal::Gtx_t & gtx)
{
    auto &board = gtx.addEntity<Board_t>();
    gtx.addComponentById<Board_t, Moon::Terminal::Sprite_t>(board.eid, std::vector<char>{
        ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ',
        ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ',
        ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ',
        '-', '-', '-', '|', '-', '-', '-', '|', '-', '-', '-',
        ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ',
        ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ',
        ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ',
        '-', '-', '-', '|', '-', '-', '-', '|', '-', '-', '-',
        ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ',
        ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ',
        ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ',}, 11, 11);
    gtx.addComponentById<Board_t, Moon::Terminal::Position_t<std::uint8_t>>(board.eid, 0, 0);
}

void selectBox(Moon::Terminal::Gtx_t & gtx, bool turn){

    if(!turn) return;

    int pos = -1;
    switch(gtx.last_key){
        case Moon::Terminal::MOON_KEY_1:
            pos = 0;
            break;
        case Moon::Terminal::MOON_KEY_2:
            pos = 1;
            break;
        case Moon::Terminal::MOON_KEY_3:
            pos = 2;
            break;
        case Moon::Terminal::MOON_KEY_4:
            pos = 3;
            break;
        case Moon::Terminal::MOON_KEY_5:
            pos = 4;
            break;
        case Moon::Terminal::MOON_KEY_6:
            pos = 5;
            break;
        case Moon::Terminal::MOON_KEY_7:
            pos = 6;
            break;
        case Moon::Terminal::MOON_KEY_8:
            pos = 7;
            break;
        case Moon::Terminal::MOON_KEY_9:
            pos = 8;
            break;
    }

    if(pos != -1){
        auto &ent = gtx.addEntity<Box_t>(pos);
        gtx.addComponentById<Box_t, Moon::Terminal::Sprite_t>(ent.eid, std::vector<char>{'O'}, 1, 1);
        gtx.addComponentById<Box_t, Moon::Terminal::Position_t<std::uint8_t>>(ent.eid, mapped_x(ent.pos), mapped_y(ent.pos));
    }
}

int main()
{
    //GameContext
    auto gtx = Moon::Terminal::Gtx_t();

    //Systems
    auto sysRender = Moon::Terminal::RenderSys_t();
    auto inputSys = Moon::Terminal::InputSys_t();

    //Init GameContext
    makeGtx(gtx);

    //Times
    Moon::Tools::TimeStep_t time;
    uint64_t nanos;
    Moon::Tools::TimeStep_t time_network;
    uint64_t nanos_network;

    //Options Setup
    bool turn = true;
    int last_pos = -1;
    int id; // random num
    while (gtx.last_key != Moon::Terminal::MOON_KEY_ESC)
    {
        //Render Update
        nanos = 1000000000 / 30;
        if (time.ellapsed() > nanos)
        {
            sysRender.update(&gtx);
            time.start();
        }

        //Network Update
        nanos_network = 1000000000;
        if (time_network.ellapsed() > nanos_network)
        {
            //sysNetwork.update(&gtx);
            //if es mi turno envio mi opcion si ya la tengo despues de enviarlo reinicio last_pos = -1 y turn = false
            //if no es mi turno actualizo los datos si la opcion turno es igual a mi id cambio turn = true
            time_network.start();
        }

        //InputUpdate
        inputSys.update(&gtx);
        selectBox(gtx, turn);

        //Sleep one nanosecond
        std::this_thread::sleep_for (std::chrono::nanoseconds(1));
    }

    //a.draw(5,6);
    return 0;
}
