#include "dlpack_container.h"

#ifndef SKIP_PYTHON
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include "make_string.hpp"

namespace py = pybind11;

class PyDLPackContainer: public DLPackContainer {
    public:
        PyDLPackContainer(py::array_t<float, py::array::c_style | py::array::forcecast> array, bool copy):
                DLPackContainer(
                    (int)array.ndim(), (int64_t*)array.shape(), (void*)array.data(),
                    DLDataTypeCode::kDLFloat, 32, 1,
                    DLDeviceType::kDLCPU, 0, NULL, copy) {
            if (sizeof(ssize_t) != sizeof(int64_t)) {
                throw std::runtime_error(MakeString(
                    "sizeof(ssize_t)=", sizeof(ssize_t), " != sizeof(int64_t)=",
                    sizeof(int64_t), "."));
            }
        }
        ~PyDLPackContainer() {}
};


static void DlpackCapsuleDestructor(PyObject* data) {
    DLManagedTensor* dlmanaged_tensor = (DLManagedTensor*)PyCapsule_GetPointer(data, "dltensor");
    if (dlmanaged_tensor) {
        // The dlmanaged_tensor has not been consumed, call deleter ourselves.
        dlmanaged_tensor->deleter(const_cast<DLManagedTensor*>(dlmanaged_tensor));
    }
    else {
        // The dlmanaged_tensor has been consumed,
        // PyCapsule_GetPointer has set an error indicator.
        PyErr_Clear();
    }
}


PYBIND11_MODULE(dlpack_container_python, m) {
	m.doc() = "Implements a container following DLPack structure (see :eplg:`DLPack`).";

    py::enum_<DLDeviceType>(m, "DLDeviceType", "DL Device Type")
        .value("kDLCPU", DLDeviceType::kDLCPU, "CPU device")
        .value("kDLCUDA", DLDeviceType::kDLCUDA, "CUDA GPU device")
        .value("kDLCUDAHost", DLDeviceType::kDLCUDAHost, "Pinned CUDA CPU memory by cudaMallocHost")
        .value("kDLOpenCL", DLDeviceType::kDLOpenCL, "OpenCL devices")
        .value("kDLVulkan", DLDeviceType::kDLVulkan, "Vulkan buffer for next generation graphics")
        .value("kDLMetal", DLDeviceType::kDLMetal, "Metal for Apple GPU")
        .value("kDLVPI", DLDeviceType::kDLVPI, "Verilog simulator buffer")
        .value("kDLROCM", DLDeviceType::kDLROCM, "ROCm GPUs for AMD GPUs")
        .value("kDLROCMHost", DLDeviceType::kDLROCMHost, "Pinned ROCm CPU memory allocated by hipMallocHost")
        .value("kDLExtDev", DLDeviceType::kDLExtDev,
                "Reserved extension device type, used for quickly test extension device "
                "The semantics can differ depending on the implementation.")
        .value("kDLCUDAManaged", DLDeviceType::kDLCUDAManaged, "CUDA managed/unified memory allocated by cudaMallocManaged")
        .value("kDLOneAPI", DLDeviceType::kDLOneAPI,
            "Unified shared memory allocated on a oneAPI non-partititioned "
            "device. Call to oneAPI runtime is required to determine the device "
            "type, the USM allocation type and the sycl context it is bound to.")
        .export_values();

    py::class_<DLDevice>(m, "DLDevice", "DL Device")
        .def_readwrite("device_type", &DLDevice::device_type, "Device type")
        .def_readwrite("device_id", &DLDevice::device_id, "Device index");
    
    py::enum_<DLDataTypeCode>(m, "DLDataTypeCode", "The type code options DLDataType")
        .value("kDLInt", DLDataTypeCode::kDLInt, "signed integer")
        .value("kDLUInt", DLDataTypeCode::kDLUInt, "unsigned integer")
        .value("kDLFloat", DLDataTypeCode::kDLFloat, "IEEE floating point")
        .value("kDLOpaqueHandle", DLDataTypeCode::kDLOpaqueHandle,
            "Opaque handle type, reserved for testing purposes. "
            "Frameworks need to agree on the handle data type for the exchange "
            "to be well-defined.")
        .value("kDLBfloat", DLDataTypeCode::kDLBfloat, "bfloat16")
        .value("kDLComplex", DLDataTypeCode::kDLComplex, 
            "complex number (C/C++/Python layout: compact struct per complex number)")
        .export_values();
    
    py::class_<DLDataType>(m, "DLDataType",
            "Examples \n"
            "- float: type_code = 2, bits = 32, lanes=1\n"
            "- float4(vectorized 4 float): type_code = 2, bits = 32, lanes=4\n"
            "- int8: type_code = 0, bits = 8, lanes=1\n"
            "- std::complex<float>: type_code = 5, bits = 64, lanes = 1")
        .def_readwrite("code", &DLDataType::code,
            "Type code of base types. "
            "We keep it uint8_t instead of DLDataTypeCode for minimal memory "
            "footprint, but the value should be one of DLDataTypeCode enum values.")
        .def_readwrite("bits", &DLDataType::bits, "Number of bits, common choices are 8, 16, 32.")
        .def_readwrite("lanes", &DLDataType::lanes, "Number of lanes in the type, used for vector types.");

	py::class_<PyDLPackContainer> container (m, "PyDLPackContainer", 
    #if defined(__APPLE__)
    "DL Pack container, may contain almost everything."
    #else
    R"pbdoc(PyDLPackContainer container, may contain almost everything.

:param value: array to copy or to hold or pointer onto
:param copy: if True, the constructor copies the data,
    if False, the pointer is just borrowed.

.. warning::
    If copy is False, the developer must know what it is doing as this pointer
    may be deleted if the object is not used by python anymore.
)pbdoc"
    #endif
        );

    container.def(py::init<py::array_t<float, py::array::c_style | py::array::forcecast>, bool>(),
                  "Copies the floats into the container if copy is True otherwise just keep a pointer onto it.",
                  py::arg("value"), py::arg("copy")=true);

    container.def_property_readonly("dtype", &DLPackContainer::dtype, "Returns the data type.");
    container.def_property_readonly("device", &DLPackContainer::device, "Returns the device.");
    container.def("empty", &DLPackContainer::empty, "Tells if it is empty");
    container.def_property_readonly("shape_size", &DLPackContainer::shape_size, "Returns the number of elements.");
    container.def_property_readonly("data_size", &DLPackContainer::data_size,
        "Returns the data size (shape_size multiplied by the element type).");
    container.def_property_readonly("element_size", &DLPackContainer::element_size, "Element size.");
    
    // dlpack    
    container.def("__dlpack__", [](PyDLPackContainer* self, py::object /* stream */) -> py::object {
            return py::reinterpret_steal<py::object>(PyCapsule_New(self, "dltensor", DlpackCapsuleDestructor));
        }, py::arg("stream")=py::none(),
        "Returns a DLPack representing the container (part of __dlpack__ protocol). "
        "This method does not copy the pointer shape, instead, "
        "it copies the pointer value. The container must persist "
        "until the dlpack structure is consumed.");

    container.def("__dlpack_device__", [](PyDLPackContainer* self) -> py::tuple {
            DLDevice device = self->device();
            return py::make_tuple((int)device.device_type, device.device_id);
        }, "Returns a tuple of integers, (device, device index) (part of __dlpack__ protocol).");

    container.def_static("from_dlpack", [](py::object data) {
            DLManagedTensor* dlpack = (DLManagedTensor*)PyCapsule_GetPointer(data.ptr(), "dltensor");
        
            std::function<void(void*)> deleter = [dlpack](void* p) {
                dlpack->deleter(dlpack);
                free(p);
            };

            DLPackContainer* cont = new DLPackContainer(
                        dlpack->dl_tensor.ndim,
                        dlpack->dl_tensor.shape,
                        dlpack->dl_tensor.data,
                        (DLDataTypeCode)dlpack->dl_tensor.dtype.code,
                        dlpack->dl_tensor.dtype.bits,
                        dlpack->dl_tensor.dtype.lanes,
                        dlpack->dl_tensor.device.device_type,
                        dlpack->dl_tensor.device.device_id,
                        NULL, //(deleter_fct)deleter,
                        false);

            // Make sure this capsule will never be used again.
            PyCapsule_SetName(data.ptr(), "used_dltensor");
            return (PyDLPackContainer*)cont;
        });
}

#endif
