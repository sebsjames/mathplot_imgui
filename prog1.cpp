/*
 * Visualize some graphs as an example use of sebsjames/mathplot
 */
#include <iostream>
#include <memory>

#include <mplot/Visual.h>         // mplot::Visual - the scene class
#include <mplot/GeodesicVisual.h> // mplot::GeodesicVisual
#include <sm/vec>                 // sm::vec - a static-sized vector (like std::array) with maths

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// The Dear ImGui Demo window (and its code imgui_demo.cpp) is useful for finding out about widgets etc
#define COMPILE_DEMO_WINDOW 1

// In this example, we extend mplot::Visual to add the ability to display a Dear ImGui frame in our window
struct imgui_visual final : public mplot::Visual<>
{
    // In our constructor, we carry out the ImGui setup
    imgui_visual (int width, int height, const std::string& title) : mplot::Visual<> (width, height, title)
    {
        // Additional Dear ImGui setup
        this->setContext(); // Set the OpenGL context before Dear ImGui initialization
        this->renderSwapsBuffers (false); // With Dear ImGui, we manually swapBuffers(), so set this false
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        ImGui_ImplGlfw_InitForOpenGL (this->window, true);
        ImGui_ImplOpenGL3_Init();

        // Because mplot::Visual provides fonts, we can load them into Dear ImGui. This
        // program will have created /tmp/DejaVuSans.ttf as it first loads.
        constexpr int size_in_pixels = 24;
        io.Fonts->AddFontFromFileTTF ("/tmp/DejaVuSans.ttf", size_in_pixels);
    }

    // Draw the GUI frame for your Visual. This frame then updates the state stored in
    // your visual and you can update your graphs/visualizations accordingly
    void gui_draw()
    {
        if (this->show_gui == true) {
            // The Dear ImGui scheme seems to be to rebuild the frame on each rendering
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

#ifdef COMPILE_DEMO_WINDOW
            if (this->show_imgui_demo) { ImGui::ShowDemoWindow (&this->show_imgui_demo); }
#endif
            // Our Options window
            ImGui::Begin("Options (Esc to toggle)");
            ImGui::Text("These are some options for your Geodesic.");
            if (ImGui::SliderInt("Iterations", &this->geodesic_iterations, 0, 5)) {
                this->needs_visualmodel_rebuild = true;
            }
            if (ImGui::Combo ("Colour map", &this->colour_map_index, this->colour_map_options, num_colours)) {
                this->needs_visualmodel_rebuild = true;
            }
            if (ImGui::Checkbox ("Lighting", &this->lighting)) {
                this->lightingEffects (this->lighting);
            }
#ifdef COMPILE_DEMO_WINDOW
            ImGui::Checkbox ("Show demo window", &this->show_imgui_demo);
#endif
            ImGui::End();

            // Now render
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData (ImGui::GetDrawData());
        }
    }

    /*
     * Public members - state for the GUI
     */

    // Show the GUI?
    bool show_gui = true;

#ifdef COMPILE_DEMO_WINDOW
    // Show the Dear ImGui demo window?
    bool show_imgui_demo = false;
#endif

    // Use lighting?
    bool lighting = true;

    // The number of iterations to run the geodesic polynomial algorithm for (0 gives an icosahedron)
    int geodesic_iterations = 0;

    // A selection of linear colourmaps from mplot/ColourMap.h
    static constexpr size_t num_colours = 6;
    const char* colour_map_options[num_colours] = { "jet", "plasma", "devon", "acton", "berlin", "lajolla" };
    int colour_map_index = 0; // Currently selected colourmap

    // Do the mplot::VisualModels need to be re-built?
    bool needs_visualmodel_rebuild = true;

protected:
    // This is the mathplot method for binding keys
    void key_callback_extra (int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) override
    {
        // Use escape to toggle the GUI
        if (key == mplot::key::escape && action == mplot::keyaction::press) { this->show_gui = this->show_gui ? false : true; }

        if (key == mplot::key::h && action == mplot::keyaction::press) {
            std::cout << "ImGui extra help:\n";
            std::cout << "Esc: Toggle GUI window\n";
        }
    }
    // We have to overload the mouse_button_callback, so that mouse press events that
    // occur on the ImGui frame are discarded
    void mouse_button_callback (int button, int action, int mods = 0)
    {
        ImGuiIO& io = ImGui::GetIO();
        // Forward to ImGui
        io.AddMouseButtonEvent (button, (action > 0));
        // If ImGui not focussed, do usual mathplot callback
        if (!io.WantCaptureMouse) {
            mplot::VisualBase<>::mouse_button_callback (button, action, mods);
        }
    }
};

// A function to create mathplot VisualModels. In this case, a single icosahedral geodesic polygon - a GeodesicVisual
mplot::GeodesicVisual<float>* make_visualmodels (imgui_visual& v, mplot::GeodesicVisual<float>* ptr)
{
    if (ptr != nullptr) { v.removeVisualModel (ptr); }
    sm::vec<float, 3> offset = { 0, 0, 0 };
    auto gv1 = std::make_unique<mplot::GeodesicVisual<float>> (offset, 0.9f);
    v.bindmodel (gv1);
    gv1->iterations = v.geodesic_iterations;
    std::string lbl = std::string("iterations = ") + std::to_string(gv1->iterations);
    gv1->addLabel (lbl, {0, -1, 0}, mplot::TextFeatures(0.06f));
    gv1->cm.setType (std::string(v.colour_map_options[v.colour_map_index]));
    gv1->finalize();
    // re-colour after construction
    auto gv1p = v.addVisualModel (gv1);
    float imax_mult = 1.0f / static_cast<float>(4);
    // sequential colouring:
    size_t sz1 = gv1p->data.size();
    gv1p->data.linspace (0.0f, 1 + v.geodesic_iterations * imax_mult, sz1);
    gv1p->reinitColours();
    v.needs_visualmodel_rebuild = false;
    return gv1p;
}

int main()
{
    imgui_visual v(1536, 1536, "Mathplot with an ImGui");

    mplot::GeodesicVisual<float>* myvm = nullptr;

    // Display until user closes window
    while (!v.readyToFinish()) {
        v.waitevents (0.018); // Wait or poll for events
        if (v.needs_visualmodel_rebuild == true) {
            myvm = make_visualmodels (v, myvm);
        }
        v.render();           // Render mathplot objects
        v.gui_draw();         // Render Dear ImGui frame(s)
        v.swapBuffers();      // Swap buffers
    }
}
