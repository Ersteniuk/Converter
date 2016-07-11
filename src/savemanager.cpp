#include "savemanager.h"

SaveManager::~SaveManager()
{
    thread.wait();//ждем к окончанию записи
}
