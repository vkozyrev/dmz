#ifndef DMZ_INPUT_PLUGIN_MOUSE_EVENT_TO_MESSAGE_DOT_H
#define DMZ_INPUT_PLUGIN_MOUSE_EVENT_TO_MESSAGE_DOT_H

#include <dmzInputObserverUtil.h>
#include <dmzRenderModulePick.h>
#include <dmzRuntimeDefinitions.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimeMessaging.h>
#include <dmzRuntimePlugin.h>
#include <dmzTypesHandleContainer.h>
#include <dmzTypesHashTableStringTemplate.h>

namespace dmz {

   class Config;
   class ConfigIterator;
   class Data;
   class DataBinder;

   class InputPluginMouseEventToMessage :
         public Plugin,
         public InputObserverUtil {

      public:
         //! \cond
         struct AttrStruct;
         struct ConverterStruct;
         struct Converter2DStruct;

         InputPluginMouseEventToMessage (const PluginInfo &Info, Config &local);
         ~InputPluginMouseEventToMessage ();

         // Plugin Interface
         virtual void discover_plugin (const Plugin *PluginPtr);
         virtual void start_plugin ();
         virtual void stop_plugin ();
         virtual void shutdown_plugin ();
         virtual void remove_plugin (const Plugin *PluginPtr);

         // Input Observer Interface
         virtual void update_channel_state (const Handle Channel, const Boolean State);

         virtual void receive_mouse_event (
            const Handle Channel,
            const InputEventMouse &Value);

      protected:
         void _send (const Message &Message, HandleContainer &targets);
         void _get_targets (const String &Name, Config &config, HandleContainer &targets);

         AttrStruct *_create_attributes (
            Config &config,
            DataBinder &binder,
            Vector *position,
            Handle *object);

         Converter2DStruct *_create_converter2d_basic (
            HashTableStringTemplate<Converter2DStruct> &table,
            ConfigIterator &it,
            Config &converterConfig,
            Config &config);

         Converter2DStruct *_create_converter2d (
            HashTableStringTemplate<Converter2DStruct> &table,
            ConfigIterator &it,
            Config &config);

         ConverterStruct *_create_converter (Config &local);
         void _init (Config &local);

         HashTableStringTemplate<ConverterStruct> _table;
         ConverterStruct *_start;
         ConverterStruct *_current;

         RenderModulePick *_pickMod;
         Definitions _defs;
         Int32 _activeCount;
         HandleContainer _sourceTable;
         HandleContainer _targetTable;
         Message _activateMessage;
         HandleContainer _activateTargetTable;
         Message _deactivateMessage;
         HandleContainer _deactivateTargetTable;
         Log _log;
         //! \endcond

      private:
         InputPluginMouseEventToMessage ();
         InputPluginMouseEventToMessage (const InputPluginMouseEventToMessage &);
         InputPluginMouseEventToMessage &operator= (
            const InputPluginMouseEventToMessage &);
   };
};

#endif // DMZ_INPUT_PLUGIN_MOUSE_EVENT_TO_MESSAGE_DOT_H