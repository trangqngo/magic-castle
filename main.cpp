// #define COW_PATCH_FRAMERATE
// #define COW_PATCH_FRAMERATE_SLEEP
#include "include.cpp"


void project() {

    // entrance meshes  
    IndexedTriangleMesh3D entrance_zero = library.meshes.box;
    IndexedTriangleMesh3D entrance_one = library.meshes.box;
    IndexedTriangleMesh3D entrance_two = library.meshes.box;

    // level one mesh
    IndexedTriangleMesh3D lvl_one_mesh = library.meshes.box;

    // level two mesh
    IndexedTriangleMesh3D lvl_two_mesh = library.meshes.box;

    // ground mesh (zero)
    IndexedTriangleMesh3D ground_mesh = library.meshes.box;
    vec3 ground_scale = V3(2.0, 1.0, 2.0);
    vec3 ground_translation = V3(0.0, -1.1, 0.0);

    // elements on level one 
    IndexedTriangleMesh3D c0 = library.meshes.box; // columns
    IndexedTriangleMesh3D c1 = library.meshes.cone; // top


    // char mesh 
    // IndexedTriangleMesh3D char_mesh = library.meshes.sphere; // how to add face?
    Soup3D char_mesh = library.soups.axes;
    vec3 char_origin = V3(0.0, 0.0, 0.0); //char's initial origin

    // Soup3D char_mesh = library.soups.axes;
    float char_speed = 0.03;
    real char_angle = RAD(0.0);

    // detecting enter entrance 
    real epsilon = 0.5;

    // level: 0-2 (0 is the ground, the lowest)
    int level = 0;

    bool paused = false;
    Camera3D camera = { 15.0, RAD(45.0), RAD(45.0), RAD(-30.0) };


    while (cow_begin_frame()) {
        camera_move(&camera);
        mat4 P = camera_get_P(&camera);
        mat4 V = camera_get_V(&camera);
        mat4 M_char = M4_Translation(char_origin) * M4_RotationAboutYAxis(char_angle) * M4_Scaling(0.3);

        {
            // gui

        }

        // draw elements

        // level zero: ground
        mat4 M_ground = M4_Translation(ground_translation) * M4_Scaling(ground_scale);
        ground_mesh.draw(P, V, M_ground, monokai.green);

        // level one
        mat4 M_lvl_one = M4_Translation(0.0, 1.5, -5.0) * M4_Scaling(1.5, 0.7, 1.5);
        lvl_one_mesh.draw(P, V, M_lvl_one, monokai.gray);

        // level two
        mat4 M_lvl_two = M4_Translation(-1.0, 4.0, 1.5) * M4_Scaling(1.0, 0.5, 1.0);
        lvl_two_mesh.draw(P, V, M_lvl_two, monokai.blue);

        // entrance zero
        vec3 v_zero = transformPoint(M_ground, V3(-1.0, 1.0, 1.0));
        mat4 M_entrance_zero = M4_Translation(v_zero - V3(-0.2, -0.2, 0.2)) * M4_Scaling(0.2, 0.2, 0.2);
        entrance_zero.draw(P, V, M_entrance_zero, monokai.red);

        // entrance one
        vec3 v_one = transformPoint(M_lvl_one, V3(1.0, 1.0, -1.0));
        mat4 M_entrance_one = M4_Translation(v_one - V3(0.2, -0.2, -0.2)) * M4_Scaling(0.2, 0.2, 0.2);
        entrance_one.draw(P, V, M_entrance_one, monokai.red);

        // entrance two
        vec3 v_two = transformPoint(M_lvl_two, V3(-1.0, 1.0, -1.0));
        mat4 M_entrance_two = M4_Translation(v_two - V3(-0.2, -0.2, -0.2)) * M4_Scaling(0.2, 0.2, 0.2);
        entrance_two.draw(P, V, M_entrance_two, monokai.red);
    

        // char_mesh.draw(P * V * M_char, monokai.red);

        //  draw character
        mat4 T = globals.Identity;
        // char origin in world's coords 
        vec3 char_origin_w = transformPoint(M_char, char_origin); 

        // TODO: right now rotating around yaxis at center of each level
        // and appear not at centers of level 
       
        {
            if (level == 0)
            {
               
            }
            else if (level == 1)
            {
                // center of level one in world coords
                vec3 v_appear_one = transformPoint(M_lvl_one, V3(0.0, 1.1, 0.0));
                T = M4_Translation(v_appear_one - char_origin_w);
            }
            else if (level == 2)
            {
                vec3 v_appear_two = transformPoint(M_lvl_two, V3(0.0, 1.1, 0.0));
                T = M4_Translation(v_appear_two - char_origin_w);
            }
        }
        gui_printf("level %d", level);
        // M_char: M4_Translation(char_origin) * M4_RotationAboutYAxis(char_angle) * M4_Scaling(0.3)
        char_mesh.draw(P * V * T * M_char, monokai.red);

        // char_mesh.draw(P, V, M_char, monokai.red); //sphere - how to add face

        // update char_speed depending on levels 
        // if (level == 2) {
        //     char_speed = -0.5 * char_speed_init;
        // } else if (level == 1) {
        //     char_speed = -0.7 * char_speed_init;
        // } else {
        //     char_speed = 1 * char_speed_init;
        // }
        { // character movement (bound by terrain)
          // bound: check if outer shell touches bound of ground-- TODO when char is sphere 

            // if in entrance zero 
            // bound of entrance zero 
            // vec3 v_bound_zero = transformPoint(M_ground, V3(-1.0, 1.0, 1.0));
            // gui_printf("v.x %f", v_bound_zero.x);
            // gui_printf("v.y %f", v_bound_zero.y);
            // gui_printf("v.z %f", v_bound_zero.z);
            if (globals.key_pressed['j']) {
                level = (level + 1) % 3; 
                gui_printf("level %d", level);
            } else if (globals.key_pressed['k']) {
                if (level == 0) {
                    level = 2;
                } else {
                    level -= 1;
                }
            }

           

            // print char position (in world coord) for debug
            // gui_printf("char.x %f", char_origin_w.x);
            // gui_printf("char.y %f", char_origin_w.y);
            // gui_printf("char.z %f", char_origin_w.z);

            // move between levels - must go into entrance
            // if (level == 0) {
            //     vec3 v_bound_zero = transformPoint(M_ground, V3(-1.0, 1.0, 1.0));
            //     if (char_origin_w.x < (v_bound_zero.x + epsilon) &&
            //         char_origin_w.z > (v_bound_zero.z - epsilon) &&
            //         globals.key_pressed['j'])
            //     {
            //        level += 1; 
            //     }
            // } else if (level == 1) {
            //     vec3 v_bound_one = transformPoint(M_entrance_one, V3(-1.0, -1.0, 1.0));
            //     if (char_origin_w.x < (v_bound_one.x + epsilon) &&
            //         char_origin_w.z > (v_bound_one.z - epsilon))
            //     {
            //        if (globals.key_pressed['j']) {
            //            level += 1;
            //        }
            //        else if (globals.key_pressed['k']) {
            //             level -= 1;
            //        }
            //     }
            // } else {

            // }

            // draw elements on level one
            { // columns
                vec3 v_c0 = transformPoint(M_lvl_one, V3(-1.0, 1.0, -1.0));
                mat4 M_c0 = M4_Translation(v_c0 - V3(-0.05, -0.5, -0.05)) * M4_Scaling(0.05, 0.5, 0.05);
                c0.draw(P, V, M_c0, monokai.red);                                     // back left
                c0.draw(P, V, M4_Translation(V3(0.0, 0.0, 0.6)) * M_c0, monokai.red); // front left
                c0.draw(P, V, M4_Translation(V3(0.6, 0.0, 0.0)) * M_c0, monokai.red); // back right
                c0.draw(P, V, M4_Translation(V3(0.6, 0.0, 0.6)) * M_c0, monokai.red); // front right
                //c1.draw(P, V, globals.Identity, monokai.red);                         // top - cylinder
                                                                                      
            }

            { // character's movement
            
            if (globals.key_held['w'])
            {
                char_origin.x += char_speed * sin(char_angle);
                char_origin.z += char_speed * cos(char_angle);
      
            }
            else if (globals.key_held['s'])
            {
                char_origin.x -= char_speed * sin(char_angle);
                char_origin.z -= char_speed * cos(char_angle);
            }
            else if (globals.key_held['a'])
            {
                char_angle += 0.05;
            }
            else if (globals.key_held['d'])
            {
                char_angle -= 0.05;
            }
            }
            
            
            // // TEST: transformation stuff
            // vec3 v= transformPoint(M_entrance_one, V3(-1.0, -1.0, 1.0));
            //   eso_begin(P*V*globals.Identity, SOUP_POINTS);
            //  eso_color(monokai.white);
            //  gui_printf("v_test.x %f", v.x);
            //  gui_printf("v_test.y %f", v.y);
            //  gui_printf("v_test.z %f", v.z);
            //  eso_vertex(v);
            //  eso_end();
        }
        
    }
}

int main() {
    APPS {
        APP(project);
    }
    return 0;
}





