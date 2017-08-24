#pragma once

#include <Python.h>
#include <MI++.h>
#include <memory>

class PythonMICallbacks : public MI::Callbacks
{
private:
    PyObject* m_indicationResult = NULL;
public:
    PythonMICallbacks(PyObject* indicationResult);
    bool WriteError(std::shared_ptr<MI::Operation> operation, std::shared_ptr<const MI::Instance> instance);
    void IndicationResult(std::shared_ptr<MI::Operation> operation, std::shared_ptr<const MI::Instance> instance,
        const std::string& bookmark, const std::string& machineID, bool moreResults, MI_Result resultCode,
        const std::string& errorString, std::shared_ptr<const MI::Instance> errorDetails);
    ~PythonMICallbacks();
};
