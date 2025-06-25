/*
 * Visualize some graphs as an example use of sebsjames/mathplot
 */
#include <iostream>
#include <memory>

#include <mplot/Visual.h>         // mplot::Visual - the scene class
#include <mplot/GraphVisual.h>    // mplot::GraphVisual - the 2D graph class
#include <mplot/DatasetStyle.h>   // mplot::DatasetStyle - setting style attributes for graphs
#include <mplot/colour.h>         // access to mplot::colour namespace
#include <mplot/unicode.h>        // mplot::unicode - Unicode text handling
#include <sm/vec>                 // sm::vec - a static-sized vector (like std::array) with maths
#include <sm/vvec>                // sm::vvec - a dynamic vector (like std::vector) with maths

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

int main()
{
    int rtn = 0;

    // Create a mplot::Visual. This is linked to a window on your desktop when the program runs
    mplot::Visual v(1536, 1536, "A variety of graph formats");
    // Set scene translation to position the graphs in the centre of the window (See Ctrl-z output to stdout)
    v.setSceneTrans (sm::vec<float,3>({-1.21382f, 0.199316f, -5.9f}));


    // Some positioning values used to place each of the GraphVisuals:
    constexpr float step = 1.4f;
    constexpr float row2 = 1.2f;

    // Dear ImGui setup

    v.setContext(); // Set the OpenGL context before ImGui initialization
    v.renderSwapsBuffers (false); // With ImGui, we manually swapBuffers()
    const char* glsl_version = "#version 410 core";
    bool show_demo_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    ImGui_ImplGlfw_InitForOpenGL(v.getWindow(), true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    try {

        // Some example data in mathplot's 'mathematical vector' class, vvec.
        sm::vvec<float> absc =  {-.5, -.4, -.3, -.2, -.1, 0, .1, .2, .3, .4, .5, .6, .7, .8};
        sm::vvec<float> data = absc.pow(3);

        // Graph 1
        auto gv = std::make_unique<mplot::GraphVisual<float>>(sm::vec<float>{0,0,0});
        v.bindmodel (gv); // Necessary boilerplate line (hooks up some callbacks in gv)

        // GraphVisuals have a 'data set style' object
        mplot::DatasetStyle ds;
        ds.linecolour = mplot::colour::purple; // See mplot/colour.h for all the colours or
                                               // http://www.cloford.com/resources/colours/500col.htm
        ds.linewidth = 0.015f;
        ds.markerstyle = mplot::markerstyle::diamond;
        ds.markercolour = mplot::colour::deepskyblue2;
        gv->setdata (absc, data, ds);

        gv->axisstyle = mplot::axisstyle::L;

        // Set labels to include greek characters:
        namespace uc = mplot::unicode;
        gv->xlabel = "Include unicode symbols like this: " + uc::toUtf8 (uc::alpha);
        gv->ylabel = "Unicode for Greek gamma is 0x03b3: " + uc::toUtf8 (uc::gamma);

        gv->setthickness (0.001f);
        gv->finalize();
        v.addVisualModel (gv);

        // Graph 2
        gv = std::make_unique<mplot::GraphVisual<float>> (sm::vec<float>({step,0,0}));
        v.bindmodel (gv);
        sm::vvec<float> data2 = absc.pow(2);
        ds.linecolour = mplot::colour::royalblue;
        ds.markerstyle = mplot::markerstyle::hexagon;
        ds.markercolour = mplot::colour::black;
        gv->setdata (absc, data2, ds);
        gv->axisstyle = mplot::axisstyle::box;
        gv->ylabel = "mm";
        gv->xlabel = "Abscissa (notice that mm is not rotated)";
        gv->setthickness (0.005f);
        gv->finalize();
        v.addVisualModel (gv);

        gv = std::make_unique<mplot::GraphVisual<float>> (sm::vec<float>({0,-row2,0}));
        v.bindmodel (gv);
        sm::vvec<float> data3 = absc.pow(4);
        gv->setsize (1,0.8);
        ds.linecolour = mplot::colour::cobaltgreen;
        ds.markerstyle = mplot::markerstyle::circle;
        ds.markercolour = mplot::colour::white;
        ds.markersize = 0.02f;
        ds.markergap = 0.0f;
        gv->setdata (absc, data3, ds);
        gv->axisstyle = mplot::axisstyle::boxfullticks;
        gv->tickstyle = mplot::tickstyle::ticksin;
        gv->ylabel = "mmi";
        gv->xlabel = "mmi is just long enough to be rotated";
        gv->setthickness (0.001f);
        gv->finalize();
        v.addVisualModel (gv);

        gv = std::make_unique<mplot::GraphVisual<float>> (sm::vec<float>({step,-row2,0}));
        v.bindmodel (gv);
        absc.resize(1000, 0.0f);
        for (int i = 0; i < 1000; ++i) {
            absc[i] = static_cast<float>(i-500) * 0.01f;
        }
        gv->setsize (1,0.8);
        ds.linecolour = mplot::colour::crimson;
        ds.markerstyle = mplot::markerstyle::none;
        ds.markergap = 0.0f;
        ds.linewidth = 0.005f;
        gv->setdata (absc, absc.pow(5), ds);
        gv->axisstyle = mplot::axisstyle::cross;
        gv->setthickness (0.002f);
        gv->finalize();
        v.addVisualModel (gv);

        // Display until user closes window
        while (!v.readyToFinish()) {
            v.waitevents (0.017);
            v.render();

            v.setContext(); // Ensure context for ImGui

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // (From example) Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
            {
                static float f = 0.0f;
                static int counter = 0;

                ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

                ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
                ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
                ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

                if (ImGui::Button("Button")) { // Buttons return true when clicked (most widgets return true when edited/activated)
                    counter++;
                }
                ImGui::SameLine();
                ImGui::Text("counter = %d", counter);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
                ImGui::End();
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            // After ImGui render, swap buffers
            v.swapBuffers();
        }

    } catch (const std::exception& e) {
        std::cerr << "Caught exception: " << e.what() << std::endl;
        rtn = -1;
    }

    return rtn;
}
