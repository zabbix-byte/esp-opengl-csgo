#include <GLFW/glfw3.h>
#include "process.h"
#include "opgl_screen_handler.h"
#include "memory_offsets.h"

using namespace std;

/* Code make by zabbix | zabbix@ztrunks.space */

int max_cs_go_players = 64;

int main()
{
    GLFWwindow *window;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_FLOATING, true);
    glfwWindowHint(GLFW_MAXIMIZED, true);
    glfwWindowHint(GLFW_RESIZABLE, false);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);
    glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, true);


    int x = 0;
    int y = 0;
    get_screen_resolution(x, y);


    window = glfwCreateWindow(x, y, "CSGO OpenGL ESP - ZT", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }


    int proc_id = get_process_id(L"csgo.exe");

    if (proc_id)
    {
        cout << "CsGo Process ID: " << proc_id << endl;
    }

    uintptr_t base_module = get_module_base_address(proc_id, L"client.dll");

    if (base_module)
    {
        cout << "Base Module loaded: " << base_module << endl;
    }


    uintptr_t local_player = base_module + offsets::addr_local_player;


    HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS, 0, proc_id);

    vector<unsigned int> team_offsets{ offsets::net_team_num };
    uintptr_t team_addr = f_dmmaddy(proc, local_player, team_offsets);

    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        ViewMatrix Matrix = Memory::Read<ViewMatrix>(proc, (base_module + offsets::addr_view_matrix));
        
        for (int i = 0; i < max_cs_go_players; ++i) {
            /* 0x10 is the distrance between the entites */
            /* is this line basicly why check all the players data */
            uintptr_t entity = Memory::Read<uintptr_t>(proc, (base_module + offsets::addr_entity_lit) + i * 0x10);

            if (entity == NULL) continue;

            uintptr_t health = Memory::Read<uintptr_t>(proc, (entity + offsets::net_health));

            if (health <= 0) continue;

            uintptr_t entity_team = Memory::Read<uintptr_t>(proc,entity + offsets::net_team_num);
            uintptr_t local_team = Memory::Read<uintptr_t>(proc, team_addr);

            Vec3 vec_origin = Memory::Read<Vec3>(proc, (entity + offsets::net_vec_origin));
            Vec3 head_origin;
            head_origin.X = vec_origin.X;
            head_origin.Y = vec_origin.Y;
            /* the height of the charancter is 75.0 */
            head_origin.Z = vec_origin.Z + 75.0f;


            Vec2 head_coords;
            Vec2 feet_coords;

            if (!world_to_screen(vec_origin, feet_coords, Matrix.Matrix)) continue;
            if (!world_to_screen(head_origin, head_coords, Matrix.Matrix)) continue;

            float height = head_coords.Y - feet_coords.Y;
            float width = height / 4.0F;
            

            if (entity_team != local_team) {
                glBegin(GL_LINES);
                glColor3f(182.0f, 0.0f, 182.0f);
                glVertex2f(0.0f, -1.0f);
                glVertex2f(feet_coords.X, feet_coords.Y);
                glEnd();

                glBegin(GL_LINES);
                glVertex2f(feet_coords.X - (width / 2.0f), feet_coords.Y);
                glVertex2f(feet_coords.X - (width / 2.0f), head_coords.Y);
                glEnd();

                glBegin(GL_LINES);
                glVertex2f(feet_coords.X - (width / 2.0f), head_coords.Y);
                glVertex2f(feet_coords.X + (width / 2.0f), head_coords.Y);
                glEnd();


                glBegin(GL_LINES);
                glVertex2f(feet_coords.X + (width / 2.0f), head_coords.Y);
                glVertex2f(feet_coords.X + (width / 2.0f), feet_coords.Y);
                glEnd();


                glBegin(GL_LINES);
                glVertex2f(feet_coords.X + (width / 2.0f), feet_coords.Y);
                glVertex2f(feet_coords.X - (width / 2.0f), feet_coords.Y);
                glEnd();
            }
            


        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}