// ----------------------------------------------------------------------------
// -                        Open3D: www.open3d.org                            -
// ----------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2018 www.open3d.org
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------------------------------------------------------

#include "open3d/t/pipelines/kernel/RGBDOdometry.h"

namespace open3d {
namespace t {
namespace pipelines {
namespace kernel {
namespace odometry {

void CreateVertexMap(const core::Tensor &depth_map,
                     const core::Tensor &intrinsics,
                     core::Tensor &vertex_map,
                     float depth_scale,
                     float depth_max) {
    core::Device device = depth_map.GetDevice();
    if (device != intrinsics.GetDevice()) {
        utility::LogError(
                "Inconsistent device between depth_map ({}) vs intrinsics ({})",
                device.ToString(), intrinsics.GetDevice().ToString());
    }

    if (device.GetType() == core::Device::DeviceType::CPU) {
        CreateVertexMapCPU(depth_map, intrinsics, vertex_map, depth_scale,
                           depth_max);
    } else if (device.GetType() == core::Device::DeviceType::CUDA) {
#ifdef BUILD_CUDA_MODULE
        CreateVertexMapCUDA(depth_map, intrinsics, vertex_map, depth_scale,
                            depth_max);
#else
        utility::LogError("Not compiled with CUDA, but CUDA device is used.");
#endif
    } else {
        utility::LogError("Unimplemented device.");
    }
}

void CreateNormalMap(const core::Tensor &vertex_map,
                     core::Tensor &normal_map,
                     float depth_scale,
                     float depth_max,
                     float depth_diff) {
    core::Device device = vertex_map.GetDevice();

    if (device.GetType() == core::Device::DeviceType::CPU) {
        CreateNormalMapCPU(vertex_map, normal_map, depth_scale, depth_max,
                           depth_diff);
    } else if (device.GetType() == core::Device::DeviceType::CUDA) {
#ifdef BUILD_CUDA_MODULE
        CreateNormalMapCUDA(vertex_map, normal_map, depth_scale, depth_max,
                            depth_diff);
#else
        utility::LogError("Not compiled with CUDA, but CUDA device is used.");
#endif
    } else {
        utility::LogError("Unimplemented device.");
    }
}
}  // namespace odometry
}  // namespace kernel
}  // namespace pipelines
}  // namespace t
}  // namespace open3d
