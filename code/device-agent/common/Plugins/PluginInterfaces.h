// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once
#include "..\DMInterfaces.h"
#include "Message.h"

namespace Microsoft { namespace Azure { namespace DeviceManagement { namespace Common {

    struct HandlerInfo
    {
        std::string id;
        std::string type;
        std::vector<std::string> dependencies;
    };

    // Common Plug-in Interface

    typedef int(__stdcall *PluginCreatePtr)();

    static const char* PluginCreateStr = "PluginCreate";

    // Marshalling Interface

    typedef int(__stdcall *PluginInvokePtr)(const char* jsonInputString, char** jsonOutputString);
    typedef int(__stdcall *PluginDeleteBufferPtr)(char *pluginBuffer);

    typedef int(__stdcall *PluginReverseInvokePtr)(const char* jsonInputString, char** jsonOutputString);
    typedef int(__stdcall *PluginReverseDeleteBufferPtr)(const char* buffer);

    typedef int(__stdcall *PluginSetReverseInvokePtr)(PluginReverseInvokePtr ReverseInvoke);
    typedef int(__stdcall *PluginSetReverseDeleteBufferPtr)(PluginReverseDeleteBufferPtr PluginReverseDeleteBuffer);

    static const char* PluginInvokeStr = "PluginInvoke";
    static const char* PluginDeleteBufferStr = "PluginDeleteBuffer";

    static const char* PluginSetReverseInvokeStr = "PluginSetReverseInvoke";
    static const char* PluginSetReverseDeleteBufferStr = "PluginSetReverseDeleteBuffer";

    // Direct (No Marshalling) Interface

    typedef std::vector<HandlerInfo>(__stdcall *PluginGetHandlersInfoPtr)();

    typedef std::shared_ptr<IRawHandler>(__stdcall *PluginCreateRawHandlerPtr)(const std::string& id);
    typedef void(__stdcall *PluginDestroyRawHandlerPtr)(const std::string& id);

    static const char* PluginGetHandlersInfoStr = "PluginGetHandlersInfo";

    static const char* PluginCreateRawHandlerStr = "PluginCreateRawHandler";
    static const char* PluginDestroyRawHandlerStr = "PluginDestroyRawHandler";

    // IPlugin
    class IPlugin
    {
    public:

        virtual void Load() = 0;

        virtual std::vector<HandlerInfo> GetHandlersInfo() const = 0;

        // Raw Interfaces

        virtual std::shared_ptr<IRawHandler> CreateRawHandler(
            const std::string& id) = 0;

        virtual void DestroyRawHandler(
            const std::string& id) = 0;

        virtual void Unload() = 0;

        virtual ~IPlugin() {}
    };

    class IPluginStateMonitor
    {
    public:
        virtual void OnPluginLoaded() = 0;

        virtual ~IPluginStateMonitor() {}
    };

    // IPluginBinaryProxy
    class IPluginBinaryProxy
    {
    public:
        virtual void Load() = 0;

        virtual Json::Value Invoke(
            const std::string& targetType,
            const std::string& targetId,
            const std::string& targetMethod,
            const Json::Value& targetParameters) = 0;

        virtual void Unload() = 0;

        virtual ~IPluginBinaryProxy() {};
    };

    // IPluginTransport
    class IPluginTransport
    {
    public:
        virtual void Initialize() = 0;
        virtual std::shared_ptr<Message> SendAndGetResponse(std::shared_ptr<Message> message) = 0;
        virtual void SetClientInterface(PluginReverseInvokePtr, PluginReverseDeleteBufferPtr) = 0;
        virtual void SetPluginInterface(PluginCreatePtr, PluginInvokePtr, PluginDeleteBufferPtr) = 0;
        virtual HANDLE GetShutdownNotificationEvent() = 0;
        virtual void CloseTransport() = 0;

        virtual ~IPluginTransport() {}
    };

}}}}
