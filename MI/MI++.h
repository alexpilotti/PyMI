#pragma once

#include <MI.h>
#include <string>
#include <tuple>
#include <map>
#include <vector>
#include <memory>
#include "MIValue.h"

namespace MI
{
    class Session;
    class Instance;
    class Operation;
    class Class;
    class Serializer;
    class OperationOptions;
    class DestinationOptions;

    class Callbacks
    {
    public:
        virtual bool WriteError(std::shared_ptr<Operation> operation, std::shared_ptr<const Instance> instance)
        {
            return true;
        }
        virtual void WriteMessage(std::shared_ptr<Operation> operation, unsigned channel, const std::string& message)
        {
        }
        virtual void WriteProgress(std::shared_ptr<Operation> operation, const std::string& activity,
            const std::string& currentOperation, const std::string& statusDescription, unsigned percentageComplete,
            unsigned secondsRemaining)
        {
        }
        virtual void ClassResult(std::shared_ptr<Operation> operation, std::shared_ptr<const Class> miClass, bool moreResults,
            MI_Result resultCode, const std::string& errorString, std::shared_ptr<const Instance> errorDetails)
        {
        }
        virtual void InstanceResult(std::shared_ptr<Operation> operation, std::shared_ptr<const Instance> instance, bool moreResults,
            MI_Result resultCode, const std::string& errorString, std::shared_ptr<const Instance> errorDetails)
        {
        }
        virtual void IndicationResult(std::shared_ptr<Operation> operation, std::shared_ptr<const Instance> instance,
            const std::string& bookmark, const std::string& machineID, bool moreResults, MI_Result resultCode,
            const std::string& errorString, std::shared_ptr<const Instance> errorDetails)
        {
        }
        virtual void StreamedParameterResult(std::shared_ptr<Operation> operation, const std::string& parameterName,
            MI_Type resultType, const MI_Value& result)
        {
        }
        virtual ~Callbacks()
        {
        }
    };

    class Application
    {
    private:
        MI_Application m_app;
        Application(const Application &obj) {}

        friend class Session;

    public:
        Application(const std::string& appId = "");
        void Close();
        bool IsClosed();
        virtual ~Application();
        std::shared_ptr<Instance> NewInstance(const std::string& className);
        std::shared_ptr<Instance> NewMethodParamsInstance(const Class& miClass, const std::string& methodName);
        std::shared_ptr<Instance> NewInstanceFromClass(const std::string& className, const Class& miClass);
        std::shared_ptr<Session> NewSession(const std::string& protocol = "", const std::string& computerName = ".",
            std::shared_ptr<DestinationOptions> destinationOptions = nullptr);
        std::shared_ptr<OperationOptions> NewOperationOptions();
        std::shared_ptr<DestinationOptions> NewDestinationOptions();
        std::shared_ptr<Serializer> NewSerializer();
    };

    class OperationOptions
    {
    private:
        MI_OperationOptions m_operationOptions;
        OperationOptions(MI_OperationOptions operationOptions) : m_operationOptions(operationOptions) {}
        OperationOptions(const OperationOptions &obj) {}

        friend Application;
        friend Session;

    public:
        std::shared_ptr<OperationOptions> Clone() const;
        void SetTimeout(const MI_Interval& timeout);
        MI_Interval GetTimeout();
        void SetCustomOption(const std::string& optionName,
                             MI_Type optionValueType,
                             const MIValue& optionValue,
                             MI_Boolean mustComply);
        void Delete();
        virtual ~OperationOptions();
    };

    class DestinationOptions
    {
    private:
        MI_DestinationOptions m_destinationOptions;
        DestinationOptions(MI_DestinationOptions destinationOptions) : m_destinationOptions(destinationOptions) {}
        DestinationOptions(const DestinationOptions &obj) {}

        friend Application;

    public:
        std::shared_ptr<DestinationOptions> Clone() const;
        void SetTimeout(const MI_Interval& timeout);
        MI_Interval GetTimeout();
        void SetUILocale(const std::string& locale);
        std::string GetUILocale();
        void SetTransport(const std::string& transport);
        std::string GetTransport();
        void AddCredentials(const std::string& authType,
                            const std::string& certThumbprint);
        void AddCredentials(const std::string& authType, const std::string& domain,
                            const std::string& username, const std::string& password);
        void Delete();
        virtual ~DestinationOptions();
    };

    class Session
    {
    private:
        MI_Session m_session;
        Session(MI_Session session) : m_session(session) {}
        Session(const Session &obj) {}

        friend Application;

    public:
        std::shared_ptr<Operation> ExecQuery(const std::string& ns, const std::string& query,
                                             const std::string& dialect = "WQL",
                                             std::shared_ptr<OperationOptions> operationOptions = nullptr);
        std::shared_ptr<Operation> InvokeMethod(
            Instance& instance, const std::string& methodName, std::shared_ptr<const Instance> inboundParams,
            std::shared_ptr<OperationOptions> operationOptions = nullptr);
        std::shared_ptr<Operation> InvokeMethod(
            const std::string& ns, const std::string& className, const std::string& methodName, std::shared_ptr<const Instance>,
            std::shared_ptr<OperationOptions> operationOptions = nullptr);
        void CreateInstance(const std::string& ns, const Instance& instance,
            std::shared_ptr<OperationOptions> operationOptions = nullptr);
        void ModifyInstance(const std::string& ns, const Instance& instance,
            std::shared_ptr<OperationOptions> operationOptions = nullptr);
        void DeleteInstance(const std::string& ns, const Instance& instance,
                            std::shared_ptr<OperationOptions> operationOptions = nullptr);
        std::shared_ptr<Operation> GetClass(const std::string& ns, const std::string& className);
        std::shared_ptr<Operation> GetInstance(const std::string& ns, const Instance& keyInstance);
        std::shared_ptr<Operation> GetAssociators(const std::string& ns, const Instance& instance, const std::string& assocClass = "",
            const std::string& resultClass = "", const std::string& role = "",
            const std::string& resultRole = "", bool keysOnly = false,
            std::shared_ptr<OperationOptions> operationOptions = nullptr);
        std::shared_ptr<Operation> Subscribe(const std::string& ns, const std::string& query, std::shared_ptr<Callbacks> callback = nullptr,
            std::shared_ptr<OperationOptions> operationOptions = nullptr, const std::string& dialect = "WQL");
        void Close();
        bool IsClosed();
        virtual ~Session();
    };

    struct Qualifier
    {
    public:
        std::string m_name;
        MI_Type m_type;
        MI_Value m_value;
        MI_Uint32 m_flags;
    };

    struct BaseElementInfo
    {
    public:
        std::string m_name;
        unsigned m_index;
        MI_Type m_type;
    };

    struct ParameterInfo : public BaseElementInfo
    {
    public:
        std::map<std::string, std::shared_ptr<Qualifier>> m_qualifiers;
    };

    struct MethodInfo
    {
    public:
        std::string m_name;
        unsigned m_index;
        std::map<std::string, std::shared_ptr<Qualifier>> m_qualifiers;
        std::map<std::string, std::shared_ptr<ParameterInfo>> m_parameters;
    };

    struct BaseElementInfoWithFlags : public BaseElementInfo
    {
    public:
        MI_Uint32 m_flags;
    };

    struct ValueElement : public BaseElementInfoWithFlags
    {
    public:
        MI_Value m_value;
    };

    struct ClassElement : public ValueElement
    {
    public:
        MI_Boolean m_valueExists;
        std::map<std::string, std::shared_ptr<Qualifier>> m_qualifiers;
    };

    class ScopedItem;

    class ScopeContextOwner
    {
    public:
        virtual void RemoveFromScopeContext(ScopedItem* item) = 0;
    };

    class ScopedItem
    {
    private:
        ScopeContextOwner* m_scopeOwner = nullptr;
    public:
        ScopedItem(ScopeContextOwner* scopeOwner) : m_scopeOwner(scopeOwner) {};
        virtual void SetOutOfScope() { m_scopeOwner = nullptr; };
        virtual void RemoveFromScopeContext();
        virtual ~ScopedItem();
    };

    class Class : private ScopedItem
    {
    private:
        MI_Class* m_class = nullptr;
        bool m_ownsInstance = false;
        std::shared_ptr<std::vector<std::string>> m_key = nullptr;

        Class(const Class &obj) : ScopedItem(nullptr) {} // Use Clone

        friend Application;
        friend Instance;
        friend Operation;
        friend Serializer;

    public:
        Class(MI_Class* miClass, bool ownsInstance, ScopeContextOwner* scopeOwner = nullptr) :
            m_class(miClass), m_ownsInstance(ownsInstance), ScopedItem(scopeOwner) {}
        unsigned GetElementsCount() const;
        std::shared_ptr<const std::vector<std::string>> GetKey();
        std::shared_ptr<ClassElement> operator[] (const std::string& name) const;
        std::shared_ptr<ClassElement> operator[] (unsigned index) const;
        unsigned GetMethodCount() const;
        std::shared_ptr<MethodInfo> GetMethodInfo(const std::string& name) const;
        std::shared_ptr<MethodInfo> GetMethodInfo(unsigned index) const;
        std::string GetClassName() const;
        std::string GetNameSpace() const;
        std::string GetServerName() const;
        std::string GetParentClassName() const;
        std::shared_ptr<Class> GetParentClass() const;
        std::shared_ptr<Class> Clone() const;
        void SetOutOfScope();
        void Delete();
        virtual ~Class();
    };

    class Instance : private ScopedItem
    {
    private:
        MI_Instance* m_instance = nullptr;
        bool m_ownsInstance = false;
        std::shared_ptr<const std::vector<std::string>> m_keyElementNames = nullptr;

        Instance(const Instance &obj) : ScopedItem(nullptr) {} // Use Clone
        const std::vector<std::string>& GetKeyElementNames();

        friend Application;
        friend Operation;
        friend Session;
        friend Serializer;

    public:
        Instance(MI_Instance* instance, bool ownsInstance, ScopeContextOwner* scopeOwner = nullptr) :
            m_instance(instance), m_ownsInstance(ownsInstance), ScopedItem(scopeOwner) {}
        MI_Instance* GetMIObject() { return this->m_instance; }
        std::shared_ptr<Instance> Clone() const;
        std::shared_ptr<Class> GetClass() const;
        std::string GetClassName() const;
        std::string GetNameSpace() const;
        std::string GetServerName() const;
        unsigned GetElementsCount() const;
        std::string GetPath();
        std::shared_ptr<ValueElement> operator[] (const std::string& name) const;
        std::shared_ptr<ValueElement> operator[] (unsigned index) const;
        void AddElement(const std::string& name, const MIValue& value);
        void SetElement(const std::string& name, const MIValue& value);
        void SetElement(unsigned index, const MIValue& value);
        MI_Type GetElementType(const std::string& name) const;
        MI_Type GetElementType(unsigned index) const;
        void ClearElement(const std::string& name);
        void ClearElement(unsigned index);
        void SetOutOfScope();
        void Delete();
        virtual ~Instance();
    };

    class Operation : private ScopeContextOwner
    {
    private:
        MI_Operation m_operation;
        MI_Boolean m_hasMoreResults = TRUE;
        bool m_ownsInstance = false;
        ScopedItem* m_currentItem = nullptr;

        Operation(const Operation &obj) {}
        void RemoveFromScopeContext(ScopedItem* item);
        void SetCurrentItem(ScopedItem* currentItem);

        friend Session;

    public:
        Operation(MI_Operation& operation, bool ownsInstance=true) : m_operation(operation), m_ownsInstance(ownsInstance) {}
        std::shared_ptr<Instance> GetNextInstance();
        std::shared_ptr<Class> GetNextClass();
        std::shared_ptr<Instance> GetNextIndication();
        bool HasMoreResults() { return m_hasMoreResults != FALSE; }
        void Cancel();
        void Close();
        bool IsClosed();
        virtual ~Operation();
    };

    class Serializer
    {
    private:
        MI_Serializer m_serializer;
        Serializer(const Serializer &obj) {} // Use Clone
        Serializer(MI_Serializer& serializer) : m_serializer(serializer) {}

        friend Application;

    public:
        std::string SerializeInstance(const Instance& instance, bool includeClass=false);
        std::string SerializeClass(const Class& miClass, bool deep=false);
        void Close();
        bool IsClosed();
        virtual ~Serializer();
    };
};
