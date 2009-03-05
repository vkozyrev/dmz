#ifndef DMZ_RENDER_MODULE_OVERLAY_OSG_DOT_H
#define DMZ_RENDER_MODULE_OVERLAY_OSG_DOT_H

#include <dmzRenderModuleOverlay.h>
#include <dmzRenderPortalSize.h>
#include <dmzRuntimeConfig.h>
#include <dmzRuntimeHandle.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimePlugin.h>
#include <dmzRuntimeResources.h>
#include <dmzTypesBase.h>
#include <dmzTypesHashTableHandleTemplate.h>
#include <dmzTypesHashTableStringTemplate.h>
#include <dmzTypesVector.h>

#include <osg/Camera>
#include <osg/Group>
#include <osg/Image>
#include <osg/Switch>
#include <osg/Matrix>
#include <osg/MatrixTransform>

namespace dmz {

   class RenderModuleCoreOSG;

   class RenderModuleOverlayOSG :
         public Plugin,
         public RenderModuleOverlay,
         public PortalSizeObserver {

      public:
         class LayoutAxis {

            public:
               virtual ~LayoutAxis () {;}
               virtual Float64 update (const Int32 Value) = 0;

            protected:
               LayoutAxis () {;}

            private:
               LayoutAxis (const LayoutAxis &);
               LayoutAxis &operator= (const LayoutAxis &);
         };

         RenderModuleOverlayOSG (const PluginInfo &Info, Config &local);
         ~RenderModuleOverlayOSG ();

         // Plugin Interface
         virtual void update_plugin_state (
            const PluginStateEnum State,
            const UInt32 Level);

         virtual void discover_plugin (
            const PluginDiscoverEnum Mode,
            const Plugin *PluginPtr);

         // RenderModuleOverlay Interface
         virtual Handle lookup_node_handle (const String &Name);

         virtual Handle lookup_node_clone_sub_handle (
            const Handle CloneHandle,
            const String &Name);

         virtual String lookup_node_name (const Handle Overlay);
         virtual RenderOverlayTypeEnum lookup_node_type (const Handle Overlay);

         virtual Boolean is_of_node_type (
            const Handle Overlay,
            const RenderOverlayTypeEnum Type);

         virtual Handle clone_template (const String &Name);

         // Overlay Group API
         virtual Boolean add_group_child (const Handle Parent, const Handle Child);
         virtual Boolean remove_group_child (const Handle Parent, const Handle Child);
         virtual Int32 lookup_group_child_count (const Handle Overlay);

         // Overlay Switch API
         virtual Boolean store_switch_state (
            const Handle Overlay,
            const Int32 Which,
            const Boolean SwitchState);

         virtual Boolean store_all_switch_state (
            const Handle Overlay,
            const Boolean SwitchState);

         virtual Boolean enable_single_switch_state (
            const Handle Overlay,
            const Int32 Which);

         virtual Boolean lookup_switch_state (
            const Handle Overlay,
            const Int32 Which);

         // Overlay Transform API
         virtual Boolean store_transform_position (
            const Handle Overlay,
            const Float64 ValueX,
            const Float64 ValueY);

         virtual Boolean lookup_transform_position (
            const Handle Overlay,
            Float64 &valueX,
            Float64 &valueY);

         virtual Boolean store_transform_rotation (
            const Handle Overlay,
            const Float64 Value);

         virtual Boolean lookup_transform_rotation (
            const Handle Overlay,
            Float64 &value);

         virtual Boolean store_transform_scale (
            const Handle Overlay,
            const Float64 ValueX,
            const Float64 ValueY);

         virtual Boolean lookup_transform_scale (
            const Handle Overlay,
            Float64 &valueX,
            Float64 &valueY);

         // PortalSizeObserver Interface
         virtual void update_portal_size (
            const Handle PortalHandle,
            const Int32 TheX,
            const Int32 TheY);

      protected:
         struct TextureStruct {

            osg::ref_ptr<osg::Image> img;
         };

         struct NodeStruct {

            const String Name;
            const RenderOverlayTypeEnum Type;
            const RuntimeHandle RHandle;
            const Handle VHandle;
            osg::ref_ptr<osg::Node> node;

            NodeStruct (const String &TheName, RuntimeContext *context, osg::Node *ptr) :
                  Name (TheName),
                  Type (RenderOverlayNode),
                  RHandle ("OSG Overlay Node", context),
                  VHandle (RHandle.get_runtime_handle ()) { node = ptr; }

            NodeStruct (
                  const String &TheName,
                  const RenderOverlayTypeEnum TheType,
                  const String &TypeName,
                  RuntimeContext *context,
                  osg::Node *ptr) :
                  Name (TheName),
                  Type (TheType),
                  RHandle (TypeName, context),
                  VHandle (RHandle.get_runtime_handle ()) { node = ptr; }
         };

         struct GroupStruct : public NodeStruct {

            osg::ref_ptr<osg::Group> group;

            GroupStruct (const String &Name, RuntimeContext *context, osg::Group *ptr) :
                  NodeStruct (
                     Name,
                     RenderOverlayGroup,
                     "OSG Overlay Group",
                     context,
                     ptr) {

               group = ptr;
            }

            GroupStruct (
                  const String &Name,
                  const RenderOverlayTypeEnum TheType,
                  const String &TypeName,
                  RuntimeContext *context,
                  osg::Group *ptr) :
                  NodeStruct (Name, Type, TypeName, context, ptr) { group = ptr; }
         };

         struct SwitchStruct : public GroupStruct {

            osg::ref_ptr<osg::Switch> switchNode;

            SwitchStruct (const String &Name, RuntimeContext *context, osg::Switch *ptr) :
                  GroupStruct (
                     Name,
                     RenderOverlaySwitch,
                     "OSG Overlay Switch",
                     context,
                     ptr) {

               switchNode = ptr;
            }
         };

         struct TransformStruct : public GroupStruct {

            osg::ref_ptr<osg::MatrixTransform> transform;
            Vector pos;
            Vector scale;
            Float64 rot;

            TransformStruct (
                  const String &Name,
                  RuntimeContext *context,
                  osg::MatrixTransform *ptr) :
                  GroupStruct (
                     Name,
                     RenderOverlayTransform,
                     "OSG Overlay Transform",
                     context,
                     ptr),
                  rot (0.0) { transform = ptr; }
         };

         struct CloneStruct {

            CloneStruct *next;
            HashTableStringTemplate<NodeStruct> nameTable;

            CloneStruct () : next (0) {;}
         };

         struct LayoutStruct {

            LayoutAxis &xaxis;
            LayoutAxis &yaxis;
            TransformStruct &ts;

            LayoutStruct (
                  LayoutAxis &theXAxis,
                  LayoutAxis &theYAxis,
                  TransformStruct &theTS) :
                  xaxis (theXAxis),
                  yaxis (theYAxis),
                  ts (theTS) {;}

            ~LayoutStruct () { delete &xaxis; delete &yaxis; }
         };

         void _update_layout (const Int32 TheX, const Int32 TheY);

         void _apply_transform (TransformStruct &ts);

         LayoutAxis *_create_axis (const String &Prefix, Config &layout);
         TextureStruct *_create_texture (const String &Name);

         Boolean _register_node (NodeStruct *ptr);
         Boolean _register_group (GroupStruct *ptr);
         Boolean _register_switch (SwitchStruct *ptr);
         Boolean _register_transform (TransformStruct *ptr);

         void _add_children (osg::ref_ptr<osg::Group> &parent, Config &node);
         void _add_node (osg::ref_ptr<osg::Group> &parent, Config &node);
         void _add_group (osg::ref_ptr<osg::Group> &parent, Config &node);
         void _add_switch (osg::ref_ptr<osg::Group> &parent, Config &node);
         void _add_transform (osg::ref_ptr<osg::Group> &parent, Config &node);
         void _add_box (osg::ref_ptr<osg::Group> &parent, Config &node);
         void _add_clone (osg::ref_ptr<osg::Group> &parent, Config &node);

         void _init_colors (Config &local);
         void _init_templates (Config &local);
         void _init_layout (Config &local);
         void _init (Config &local);

         Log _log;
         Resources _rc;
         RenderModuleCoreOSG *_core;

         osg::ref_ptr<osg::Camera> _camera;
         osg::ref_ptr<osg::Group> _rootNode;
         CloneStruct *_cloneStack;
         HashTableStringTemplate<TextureStruct> _textureTable;

         HashTableStringTemplate<osg::Vec4> _colorTable;
         HashTableStringTemplate<Config> _templateTable;
         HashTableStringTemplate<NodeStruct> _nodeNameTable;
         HashTableHandleTemplate<NodeStruct> _nodeTable;
         HashTableHandleTemplate<GroupStruct> _groupTable;
         HashTableHandleTemplate<SwitchStruct> _switchTable;
         HashTableHandleTemplate<TransformStruct> _transformTable;
         HashTableHandleTemplate<CloneStruct> _cloneTable;
         HashTableHandleTemplate<LayoutStruct> _layoutTable;

      private:
         RenderModuleOverlayOSG ();
         RenderModuleOverlayOSG (const RenderModuleOverlayOSG &);
         RenderModuleOverlayOSG &operator= (const RenderModuleOverlayOSG &);

   };
};

#endif // DMZ_RENDER_MODULE_OVERLAY_OSG_DOT_H
