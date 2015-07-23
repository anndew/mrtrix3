/*
   Copyright 2014 Brain Research Institute, Melbourne, Australia

   Written by Robert E. Smith, 2015.

   This file is part of MRtrix.

   MRtrix is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   MRtrix is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with MRtrix.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef __gui_mrview_tool_connectome_connectome_h__
#define __gui_mrview_tool_connectome_connectome_h__

#include <map>
#include <vector>

#include "bitset.h"
#include "point.h"

#include "gui/opengl/gl.h"
#include "gui/opengl/lighting.h"
#include "gui/opengl/shader.h"
#include "gui/lighting_dock.h"
#include "gui/mrview/adjust_button.h"
#include "gui/mrview/colourmap_button.h"
#include "gui/mrview/image.h"
#include "gui/mrview/mode/base.h"
#include "gui/mrview/tool/base.h"
#include "gui/shapes/cube.h"
#include "gui/shapes/cylinder.h"
#include "gui/shapes/sphere.h"
#include "gui/color_button.h"
#include "gui/projection.h"

#include "image/buffer_preload.h"
#include "image/buffer_scratch.h"

#include "mesh/mesh.h"

#include "connectome/mat2vec.h"
#include "connectome/config/config.h"
#include "connectome/lut.h"

#include "gui/mrview/tool/connectome/colourmap_observers.h"
#include "gui/mrview/tool/connectome/edge.h"
#include "gui/mrview/tool/connectome/file_data_vector.h"
#include "gui/mrview/tool/connectome/node.h"
#include "gui/mrview/tool/connectome/node_list.h"
#include "gui/mrview/tool/connectome/node_overlay.h"
#include "gui/mrview/tool/connectome/selection.h"
#include "gui/mrview/tool/connectome/shaders.h"
#include "gui/mrview/tool/connectome/types.h"




namespace MR
{
  namespace GUI
  {
    namespace MRView
    {
      namespace Tool
      {

        class Connectome : public Base
        {
            Q_OBJECT

            enum class node_visibility_matrix_operator_t { ANY, ALL };
            enum class node_property_matrix_operator_t { MIN, MEAN, SUM, MAX };

          public:

            Connectome (Dock* parent);

            virtual ~Connectome ();

            void draw (const Projection& transform, bool is_3D, int axis, int slice) override;
            void draw_colourbars() override;
            size_t visible_number_colourbars () override;

            node_t num_nodes() const { return nodes.size() ? nodes.size() - 1 : 0; }
            size_t num_edges() const { return edges.size(); }

            static void add_commandline_options (MR::App::OptionList& options);
            virtual bool process_commandline_option (const MR::App::ParsedOption& opt) override;

          private slots:
            void image_open_slot();
            void lut_open_slot (int);
            void config_open_slot();
            void hide_all_slot();

            void lighting_change_slot (int);
            void lighting_settings_slot();
            void lighting_parameter_slot();
            void crop_to_slab_toggle_slot (int);
            void crop_to_slab_parameter_slot();
            void show_node_list_slot();
            void node_selection_settings_changed_slot();

            void node_visibility_selection_slot (int);
            void node_geometry_selection_slot (int);
            void node_colour_selection_slot (int);
            void node_size_selection_slot (int);
            void node_alpha_selection_slot (int);

            void node_visibility_matrix_operator_slot (int);
            void node_visibility_parameter_slot();
            void sphere_lod_slot (int);
            void overlay_interp_slot (int);
            void node_colour_matrix_operator_slot (int);
            void node_fixed_colour_change_slot();
            void node_colour_parameter_slot();
            void node_size_matrix_operator_slot (int);
            void node_size_value_slot();
            void node_size_parameter_slot();
            void node_alpha_matrix_operator_slot (int);
            void node_alpha_value_slot (int);
            void node_alpha_parameter_slot();

            void edge_visibility_selection_slot (int);
            void edge_geometry_selection_slot (int);
            void edge_colour_selection_slot (int);
            void edge_size_selection_slot (int);
            void edge_alpha_selection_slot (int);

            void edge_visibility_parameter_slot();
            void cylinder_lod_slot (int);
            void edge_colour_change_slot();
            void edge_colour_parameter_slot();
            void edge_size_value_slot();
            void edge_size_parameter_slot();
            void edge_alpha_value_slot (int);
            void edge_alpha_parameter_slot();

          protected:

            QPushButton *image_button, *hide_all_button;
            QComboBox *lut_combobox;
            QPushButton *config_button;

            QCheckBox *lighting_checkbox;
            QPushButton *lighting_settings_button;
            QCheckBox *crop_to_slab_checkbox;
            QLabel *crop_to_slab_label;
            AdjustButton *crop_to_slab_button;
            QLabel *show_node_list_label;
            QPushButton *show_node_list_button;

            QComboBox *node_visibility_combobox;
            QComboBox *node_visibility_matrix_operator_combobox;
            QLabel *node_visibility_warning_icon;
            QLabel *node_visibility_threshold_label;
            AdjustButton *node_visibility_threshold_button;
            QCheckBox *node_visibility_threshold_invert_checkbox;

            QComboBox *node_geometry_combobox;
            QLabel *node_geometry_sphere_lod_label;
            QSpinBox *node_geometry_sphere_lod_spinbox;
            QCheckBox *node_geometry_overlay_interp_checkbox;
            QLabel *node_geometry_overlay_3D_warning_icon;

            QComboBox *node_colour_combobox;
            QComboBox *node_colour_matrix_operator_combobox;
            QColorButton *node_colour_fixedcolour_button;
            ColourMapButton *node_colour_colourmap_button;
            QLabel *node_colour_range_label;
            AdjustButton *node_colour_lower_button, *node_colour_upper_button;

            QComboBox *node_size_combobox;
            QComboBox *node_size_matrix_operator_combobox;
            AdjustButton *node_size_button;
            QLabel *node_size_range_label;
            AdjustButton *node_size_lower_button, *node_size_upper_button;
            QCheckBox *node_size_invert_checkbox;

            QComboBox *node_alpha_combobox;
            QComboBox *node_alpha_matrix_operator_combobox;
            QSlider *node_alpha_slider;
            QLabel *node_alpha_range_label;
            AdjustButton *node_alpha_lower_button, *node_alpha_upper_button;
            QCheckBox *node_alpha_invert_checkbox;

            QComboBox *edge_visibility_combobox;
            QLabel *edge_visibility_warning_icon;
            QLabel *edge_visibility_threshold_label;
            AdjustButton *edge_visibility_threshold_button;
            QCheckBox *edge_visibility_threshold_invert_checkbox;

            QComboBox *edge_geometry_combobox;
            QLabel *edge_geometry_cylinder_lod_label;
            QSpinBox *edge_geometry_cylinder_lod_spinbox;
            QCheckBox *edge_geometry_line_smooth_checkbox;

            QComboBox *edge_colour_combobox;
            QColorButton *edge_colour_fixedcolour_button;
            ColourMapButton *edge_colour_colourmap_button;
            QLabel *edge_colour_range_label;
            AdjustButton *edge_colour_lower_button, *edge_colour_upper_button;

            QComboBox *edge_size_combobox;
            AdjustButton *edge_size_button;
            QLabel *edge_size_range_label;
            AdjustButton *edge_size_lower_button, *edge_size_upper_button;
            QCheckBox *edge_size_invert_checkbox;

            QComboBox *edge_alpha_combobox;
            QSlider *edge_alpha_slider;
            QLabel *edge_alpha_range_label;
            AdjustButton *edge_alpha_lower_button, *edge_alpha_upper_button;
            QCheckBox *edge_alpha_invert_checkbox;

          private:

            NodeShader node_shader;
            EdgeShader edge_shader;


            // For the sake of viewing nodes as an overlay, need to ALWAYS
            // have access to the parcellation image
            std::unique_ptr< MR::Image::BufferPreload<node_t> > buffer;


            std::vector<Node> nodes;
            std::vector<Edge> edges;


            // For converting connectome matrices to vectors
            MR::Connectome::Mat2Vec mat2vec;


            // If a lookup table is provided, this container will store the
            //   properties of each node as provided in that file (e.g. name & colour)
            Node_map lut;

            // If a connectome configuration file is provided, this will map
            //   each structure name to an index in the parcellation image;
            //   this can then be used to produce the lookup table
            std::vector<std::string> config;

            // If both a LUT and a config file have been provided, this provides
            //   a direct vector mapping from image node index to a position in
            //   the lookup table, pre-generated
            std::vector<Node_map::const_iterator> lut_mapping;


            // Fixed lighting settings from the main window, and popup dialog
            GL::Lighting lighting;
            std::unique_ptr<LightingDock> lighting_dock;


            // Node selection
            Tool::Dock* node_list;


            // Used when the geometry of node visualisation is a sphere
            Shapes::Sphere sphere;
            GL::VertexArrayObject sphere_VAO;

            // Used when the geometry of node visualisation is a cube
            Shapes::Cube cube;
            GL::VertexArrayObject cube_VAO;

            // Used when the geometry of node visualisation is an image overlay
            std::unique_ptr<NodeOverlay> node_overlay;

            // Used when the geometry of edge visualisation is a cylinder
            Shapes::Cylinder cylinder;
            GL::VertexArrayObject cylinder_VAO;


            // Settings related to slab cropping
            bool is_3D, crop_to_slab;
            float slab_thickness;


            // Settings for colour bars
            ColourMap::Renderer colourbar_renderer;
            bool show_node_colour_bar, show_edge_colour_bar;


            // Current node visualisation settings
            node_visibility_t node_visibility;
            node_geometry_t node_geometry;
            node_colour_t node_colour;
            node_size_t node_size;
            node_alpha_t node_alpha;

            // Values that need to be stored locally w.r.t. node visualisation
            BitSet selected_nodes;
            node_t selected_node_count;
            NodeSelectionSettings node_selection_settings;

            bool have_meshes;
            node_visibility_matrix_operator_t node_visibility_matrix_operator;
            node_property_matrix_operator_t node_colour_matrix_operator, node_size_matrix_operator, node_alpha_matrix_operator;
            Point<float> node_fixed_colour;
            size_t node_colourmap_index;
            bool node_colourmap_invert;
            float node_fixed_alpha;
            float node_size_scale_factor;
            float voxel_volume;
            FileDataVector node_values_from_file_visibility;
            FileDataVector node_values_from_file_colour;
            FileDataVector node_values_from_file_size;
            FileDataVector node_values_from_file_alpha;


            // Current edge visualisation settings
            edge_visibility_t edge_visibility;
            edge_geometry_t edge_geometry;
            edge_colour_t edge_colour;
            edge_size_t edge_size;
            edge_alpha_t edge_alpha;

            // Other values that need to be stored w.r.t. edge visualisation
            bool have_exemplars, have_streamtubes;
            Point<float> edge_fixed_colour;
            size_t edge_colourmap_index;
            bool edge_colourmap_invert;
            float edge_fixed_alpha;
            float edge_size_scale_factor;
            FileDataVector edge_values_from_file_visibility;
            FileDataVector edge_values_from_file_colour;
            FileDataVector edge_values_from_file_size;
            FileDataVector edge_values_from_file_alpha;

            // Limits on drawing line thicknesses from OpenGL
            GLint line_thickness_range_aliased[2];
            GLint line_thickness_range_smooth[2];


            // Classes to receive inputs from the colourmap buttons and act accordingly
            NodeColourObserver node_colourmap_observer;
            EdgeColourObserver edge_colourmap_observer;


            // Helper functions
            void clear_all();
            void enable_all (const bool);
            void initialise (const std::string&);

            void draw_nodes (const Projection&);
            void draw_edges (const Projection&);

            bool import_vector_file (FileDataVector&, const std::string&);
            bool import_matrix_file (FileDataVector&, const std::string&);

            void load_properties();

            void calculate_node_visibility();
            void calculate_node_colours();
            void calculate_node_sizes();
            void calculate_node_alphas();

            void update_node_overlay();

            void calculate_edge_visibility();
            void calculate_edge_colours();
            void calculate_edge_sizes();
            void calculate_edge_alphas();

            // Helper functions for determining actual node / edge visual properties
            //   given current selection status
            void node_selection_changed (const std::vector<node_t>&);

            bool         node_visibility_given_selection (const node_t);
            Point<float> node_colour_given_selection     (const node_t);
            float        node_size_given_selection       (const node_t);
            float        node_alpha_given_selection      (const node_t);
            bool         edge_visibility_given_selection (const Edge&);
            Point<float> edge_colour_given_selection     (const Edge&);
            float        edge_size_given_selection       (const Edge&);
            float        edge_alpha_given_selection      (const Edge&);

            void get_meshes();
            void get_exemplars();
            void get_streamtubes();

            bool use_lighting() const;
            bool use_alpha_nodes() const;
            bool use_alpha_edges() const;

            float calc_line_width (const float, const bool) const;

            friend class NodeColourObserver;
            friend class EdgeColourObserver;

            friend class NodeShader;
            friend class EdgeShader;

            friend class Node_list;
            friend class Node_list_model;

        };



      }
    }
  }
}

#endif




