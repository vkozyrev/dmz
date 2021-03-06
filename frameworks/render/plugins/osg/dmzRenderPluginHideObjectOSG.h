#ifndef DMZ_RENDER_PLUGIN_HIDE_OBJECT_OSG_DOT_H
#define DMZ_RENDER_PLUGIN_HIDE_OBJECT_OSG_DOT_H

#include <dmzObjectObserverUtil.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimePlugin.h>

namespace dmz {

   class RenderModuleCoreOSG;

   class RenderPluginHideObjectOSG :
         public Plugin,
         public ObjectObserverUtil {

      public:
         RenderPluginHideObjectOSG (const PluginInfo &Info, Config &local);
         ~RenderPluginHideObjectOSG ();

         // Plugin Interface
         virtual void update_plugin_state (
            const PluginStateEnum State,
            const UInt32 Level);

         virtual void discover_plugin (
            const PluginDiscoverEnum Mode,
            const Plugin *PluginPtr);

         // Object Observer Interface
         virtual void update_object_flag (
            const UUID &Identity,
            const Handle ObjectHandle,
            const Handle AttributeHandle,
            const Boolean Value,
            const Boolean *PreviousValue);

      protected:
         void _init (Config &local);

         Log _log;
         RenderModuleCoreOSG *_core;
         UInt32 _cullMask;

      private:
         RenderPluginHideObjectOSG ();
         RenderPluginHideObjectOSG (const RenderPluginHideObjectOSG &);
         RenderPluginHideObjectOSG &operator= (const RenderPluginHideObjectOSG &);

   };
};

#endif // DMZ_RENDER_PLUGIN_HIDE_OBJECT_OSG_DOT_H
