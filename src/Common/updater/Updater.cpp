#include <iostream>
#include <cassert>
#include "Updater.h"
#include "../utils/utils.h"

bool is_finished = false;

Updater::Updater(int time_step, int client_id) : m_time_step(time_step), m_cid(client_id)
{
}

void Updater::run()
{
    while (!is_finished)
    {
        update();
        utils::sleep(m_time_step);
    }
}

void Updater::setFinished()
{
    is_finished = true;
}
