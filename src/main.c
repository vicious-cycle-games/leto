#include <interface/window.h>
int main(void)
{
    leto_window_info_t window_info;
    LetoCreateWindow(&window_info, "Leto");

    LetoDestroyWindow(&window_info);
    return 0;
}
