#include "../core/utils/include/Menu.h"

int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    int restart;
    do
    {
        Menu::run();
        cout << "Хотите перезапустить программу? (0 - нет / 1 - да): ";
        restart = getValidNumericValue(cin, 0, 1);
        
    } while(restart != 0);

    return 0;
}
