#include "ccv.h"
#include "ccv_internal.h"
#include "nnc/ccv_nnc.h"
#include "nnc/ccv_nnc_easy.h"
#include "nnc/ccv_nnc_internal.h"
#ifdef USE_OPENMP
#include <omp.h>
#endif
#ifdef USE_DISPATCH
#include <dispatch/dispatch.h>
#endif
#include "../_ccv_nnc_cpu_ref.h"

void _ccv_nnc_tensor_transfer_cpu_ref_f16(const ccv_nnc_tensor_view_t* const a, ccv_nnc_tensor_view_t* const b)
{
	// Assuming this is float 32.
	assert(a->info.datatype == b->info.datatype);
	if (!CCV_IS_TENSOR_VIEW(a) && !CCV_IS_TENSOR_VIEW(b))
	{
		// Super optimal case, just do memcpy.
		memcpy(b->data.u8, a->data.u8, ccv_nnc_tensor_count(a->info) * CCV_GET_DATA_TYPE_SIZE(a->info.datatype));
		return;
	}
	int dim[CCV_NNC_MAX_DIM_ALLOC];
	int ainc[CCV_NNC_MAX_DIM_ALLOC];
	int binc[CCV_NNC_MAX_DIM_ALLOC];
	ccv_nnc_tensor_view_get_dim(a, dim);
	assert(ccv_nnc_tensor_view_check_dim(b, dim));
	ccv_nnc_tensor_view_get_inc(a, ainc);
	ccv_nnc_tensor_view_get_inc(b, binc);
	assert(CCV_NNC_MAX_DIM == 2); // Need to change this logic for CCV_NNC_MAX_DIM == other number.
	int i[CCV_NNC_MAX_DIM + 2];
	ccv_float16_t* ap = a->data.f16;
	ccv_float16_t* bp = b->data.f16;
	if (ainc[3] == dim[3] && binc[3] == dim[3])
	{
		// Special casing if the ainc[3] is the same as dim[3] (do memcpy for the last two dim)
		for (i[0] = 0; i[0] < dim[0]; i[0]++)
		{
			for (i[1] = 0; i[1] < dim[1]; i[1]++)
			{
				memcpy(bp, ap, dim[2] * dim[3] * sizeof(ccv_float16_t));
				ap += ainc[2] * ainc[3];
				bp += binc[2] * binc[3];
			}
			ap += (ainc[1] - dim[1]) * ainc[2] * ainc[3];
			bp += (binc[1] - dim[1]) * binc[2] * binc[3];
		}
		return;
	}
	// Non-optimal case, need to do skip copy.
	for (i[0] = 0; i[0] < dim[0]; i[0]++)
	{
		for (i[1] = 0; i[1] < dim[1]; i[1]++)
		{
			for (i[2] = 0; i[2] < dim[2]; i[2]++)
			{
				memcpy(bp, ap, dim[3] * sizeof(ccv_float16_t));
				ap += ainc[3];
				bp += binc[3];
			}
			ap += (ainc[2] - dim[2]) * ainc[3];
			bp += (binc[2] - dim[2]) * binc[3];
		}
		ap += (ainc[1] - dim[1]) * ainc[2] * ainc[3];
		bp += (binc[1] - dim[1]) * binc[2] * binc[3];
	}
}

void _ccv_nnc_tensor_transfer_cpu_ref_f32(const ccv_nnc_tensor_view_t* const a, ccv_nnc_tensor_view_t* const b)
{
	// Assuming this is float 32.
	assert(a->info.datatype == b->info.datatype);
	if (!CCV_IS_TENSOR_VIEW(a) && !CCV_IS_TENSOR_VIEW(b))
	{
		// Super optimal case, just do memcpy.
		memcpy(b->data.u8, a->data.u8, ccv_nnc_tensor_count(a->info) * CCV_GET_DATA_TYPE_SIZE(a->info.datatype));
		return;
	}
	int dim[CCV_NNC_MAX_DIM_ALLOC];
	int ainc[CCV_NNC_MAX_DIM_ALLOC];
	int binc[CCV_NNC_MAX_DIM_ALLOC];
	ccv_nnc_tensor_view_get_dim(a, dim);
	assert(ccv_nnc_tensor_view_check_dim(b, dim));
	ccv_nnc_tensor_view_get_inc(a, ainc);
	ccv_nnc_tensor_view_get_inc(b, binc);
	assert(CCV_NNC_MAX_DIM == 2); // Need to change this logic for CCV_NNC_MAX_DIM == other number.
	int i[CCV_NNC_MAX_DIM + 2];
	float* ap = a->data.f32;
	float* bp = b->data.f32;
	if (ainc[3] == dim[3] && binc[3] == dim[3])
	{
		// Special casing if the ainc[3] is the same as dim[3] (do memcpy for the last two dim)
		for (i[0] = 0; i[0] < dim[0]; i[0]++)
		{
			for (i[1] = 0; i[1] < dim[1]; i[1]++)
			{
				memcpy(bp, ap, dim[2] * dim[3] * sizeof(float));
				ap += ainc[2] * ainc[3];
				bp += binc[2] * binc[3];
			}
			ap += (ainc[1] - dim[1]) * ainc[2] * ainc[3];
			bp += (binc[1] - dim[1]) * binc[2] * binc[3];
		}
		return;
	}
	// Non-optimal case, need to do skip copy.
	for (i[0] = 0; i[0] < dim[0]; i[0]++)
	{
		for (i[1] = 0; i[1] < dim[1]; i[1]++)
		{
			for (i[2] = 0; i[2] < dim[2]; i[2]++)
			{
				memcpy(bp, ap, dim[3] * sizeof(float));
				ap += ainc[3];
				bp += binc[3];
			}
			ap += (ainc[2] - dim[2]) * ainc[3];
			bp += (binc[2] - dim[2]) * binc[3];
		}
		ap += (ainc[1] - dim[1]) * ainc[2] * ainc[3];
		bp += (binc[1] - dim[1]) * binc[2] * binc[3];
	}
}

void _ccv_nnc_tensor_transfer_cpu_ref_f64(const ccv_nnc_tensor_view_t* const a, ccv_nnc_tensor_view_t* const b)
{
	// Assuming this is float 32.
	assert(a->info.datatype == b->info.datatype);
	if (!CCV_IS_TENSOR_VIEW(a) && !CCV_IS_TENSOR_VIEW(b))
	{
		// Super optimal case, just do memcpy.
		memcpy(b->data.u8, a->data.u8, ccv_nnc_tensor_count(a->info) * CCV_GET_DATA_TYPE_SIZE(a->info.datatype));
		return;
	}
	int dim[CCV_NNC_MAX_DIM_ALLOC];
	int ainc[CCV_NNC_MAX_DIM_ALLOC];
	int binc[CCV_NNC_MAX_DIM_ALLOC];
	ccv_nnc_tensor_view_get_dim(a, dim);
	assert(ccv_nnc_tensor_view_check_dim(b, dim));
	ccv_nnc_tensor_view_get_inc(a, ainc);
	ccv_nnc_tensor_view_get_inc(b, binc);
	assert(CCV_NNC_MAX_DIM == 2); // Need to change this logic for CCV_NNC_MAX_DIM == other number.
	int i[CCV_NNC_MAX_DIM + 2];
	double* ap = a->data.f64;
	double* bp = b->data.f64;
	if (ainc[3] == dim[3] && binc[3] == dim[3])
	{
		// Special casing if the ainc[3] is the same as dim[3] (do memcpy for the last two dim)
		for (i[0] = 0; i[0] < dim[0]; i[0]++)
		{
			for (i[1] = 0; i[1] < dim[1]; i[1]++)
			{
				memcpy(bp, ap, dim[2] * dim[3] * sizeof(double));
				ap += ainc[2] * ainc[3];
				bp += binc[2] * binc[3];
			}
			ap += (ainc[1] - dim[1]) * ainc[2] * ainc[3];
			bp += (binc[1] - dim[1]) * binc[2] * binc[3];
		}
		return;
	}
	// Non-optimal case, need to do skip copy.
	for (i[0] = 0; i[0] < dim[0]; i[0]++)
	{
		for (i[1] = 0; i[1] < dim[1]; i[1]++)
		{
			for (i[2] = 0; i[2] < dim[2]; i[2]++)
			{
				memcpy(bp, ap, dim[3] * sizeof(double));
				ap += ainc[3];
				bp += binc[3];
			}
			ap += (ainc[2] - dim[2]) * ainc[3];
			bp += (binc[2] - dim[2]) * binc[3];
		}
		ap += (ainc[1] - dim[1]) * ainc[2] * ainc[3];
		bp += (binc[1] - dim[1]) * binc[2] * binc[3];
	}
}

void _ccv_nnc_tensor_set_cpu_ref(ccv_nnc_tensor_view_t* const a, const float b)
{
	// Assuming this is float 32.
	int dim[CCV_NNC_MAX_DIM_ALLOC];
	int ainc[CCV_NNC_MAX_DIM_ALLOC];
	int x;
	if (!CCV_IS_TENSOR_VIEW(a))
	{
		// Super optimal case, just do one for-loop for sum.
		const int tensor_count = ccv_nnc_tensor_count(a->info);
		for (x = 0; x < tensor_count; x++)
			a->data.f32[x] = b;
		return;
	}
	assert(CCV_NNC_MAX_DIM == 2); // Need to change this logic for CCV_NNC_MAX_DIM == other number.
	ccv_nnc_tensor_view_get_dim(a, dim);
	ccv_nnc_tensor_view_get_inc(a, ainc);
	int i[CCV_NNC_MAX_DIM + 2];
	float* ap = a->data.f32;
	const int count = dim[2] * dim[3];
	if (ainc[3] == dim[3])
	{
		// Special casing if the ainc[3] is the same as dim[3]
		for (i[0] = 0; i[0] < dim[0]; i[0]++)
		{
			for (i[1] = 0; i[1] < dim[1]; i[1]++)
			{
				for (x = 0; x < count; x++)
					ap[x] = b;
				ap += ainc[2] * ainc[3];
			}
			ap += (ainc[1] - dim[1]) * ainc[2] * ainc[3];
		}
		return;
	}
	// Non-optimal case, need to do skip copy.
	for (i[0] = 0; i[0] < dim[0]; i[0]++)
	{
		for (i[1] = 0; i[1] < dim[1]; i[1]++)
		{
			for (i[2] = 0; i[2] < dim[2]; i[2]++)
			{
				for (x = 0; x < dim[3]; x++)
					ap[x] = b;
				ap += ainc[3];
			}
			ap += (ainc[2] - dim[2]) * ainc[3];
		}
		ap += (ainc[1] - dim[1]) * ainc[2] * ainc[3];
	}
}

static int _ccv_nnc_data_transfer(const ccv_nnc_cmd_t cmd, const ccv_nnc_hint_t hint, const int flags, ccv_nnc_tensor_t* const* const inputs, const int input_size, ccv_nnc_tensor_t* const* const outputs, const int output_size, ccv_nnc_stream_context_t* const stream_context)
{
	int i;
	for (i = 0; i < ccv_min(input_size, output_size); i++)
	{
		const ccv_nnc_tensor_view_t* a = (ccv_nnc_tensor_view_t*)inputs[i];
		ccv_nnc_tensor_view_t* b = (ccv_nnc_tensor_view_t*)outputs[i];
		assert(a->info.datatype == b->info.datatype);
		if (a != b) // Only do transfer if these are two different tensors.
		{
			if (a->info.datatype == CCV_16F)
				_ccv_nnc_tensor_transfer_cpu_ref_f16(a, b);
			else if (a->info.datatype == CCV_32F || a->info.datatype == CCV_32S)
				_ccv_nnc_tensor_transfer_cpu_ref_f32(a, b);
			else if (a->info.datatype == CCV_64F)
				_ccv_nnc_tensor_transfer_cpu_ref_f64(a, b);
		}
	}
	return CCV_NNC_EXEC_SUCCESS;
}

REGISTER_COMMAND_BACKEND(CCV_NNC_DATA_TRANSFER_FORWARD, CCV_NNC_BACKEND_CPU_REF)(ccv_nnc_cmd_backend_registry_t* const registry)
{
	registry->tensor_formats = CCV_TENSOR_FORMAT_NCHW | CCV_TENSOR_FORMAT_NHWC | CCV_TENSOR_FORMAT_CHWN;
	registry->tensor_datatypes = CCV_64F | CCV_32F | CCV_16F | CCV_32S;
	registry->tensor_memory = CCV_TENSOR_CPU_MEMORY;
	registry->algorithms = 1;
	registry->exec = _ccv_nnc_data_transfer;
}

REGISTER_COMMAND_BACKEND(CCV_NNC_DATA_TRANSFER_BACKWARD, CCV_NNC_BACKEND_CPU_REF)(ccv_nnc_cmd_backend_registry_t* const registry)
{
	registry->tensor_formats = CCV_TENSOR_FORMAT_NCHW | CCV_TENSOR_FORMAT_NHWC | CCV_TENSOR_FORMAT_CHWN;
	registry->tensor_datatypes = CCV_64F | CCV_32F | CCV_16F | CCV_32S;
	registry->tensor_memory = CCV_TENSOR_CPU_MEMORY;
	registry->algorithms = 1;
	registry->exec = _ccv_nnc_data_transfer;
}

static int _ccv_nnc_set_forw(const ccv_nnc_cmd_t cmd, const ccv_nnc_hint_t hint, const int flags, ccv_nnc_tensor_t* const* const inputs, const int input_size, ccv_nnc_tensor_t* const* const outputs, const int output_size, ccv_nnc_stream_context_t* const stream_context)
{
	int i;
	if (cmd.info.blas.a[0] == 0)
		for (i = 0; i < output_size; i++)
			ccv_nnc_tensor_zero(outputs[i]);
	else
		for (i = 0; i < output_size; i++)
			_ccv_nnc_tensor_set_cpu_ref((ccv_nnc_tensor_view_t*)outputs[i], cmd.info.blas.a[0]);
	return CCV_NNC_EXEC_SUCCESS;
}

static int _ccv_nnc_set_back(const ccv_nnc_cmd_t cmd, const ccv_nnc_hint_t hint, const int flags, ccv_nnc_tensor_t* const* const inputs, const int input_size, ccv_nnc_tensor_t* const* const outputs, const int output_size, ccv_nnc_stream_context_t* const stream_context)
{
	int i;
	for (i = 0; i < output_size; i++)
		ccv_nnc_tensor_zero(outputs[i]);
	return CCV_NNC_EXEC_SUCCESS;
}

REGISTER_COMMAND_BACKEND(CCV_NNC_SET_FORWARD, CCV_NNC_BACKEND_CPU_REF)(ccv_nnc_cmd_backend_registry_t* const registry)
{
	registry->tensor_formats = CCV_TENSOR_FORMAT_NCHW | CCV_TENSOR_FORMAT_NHWC | CCV_TENSOR_FORMAT_CHWN;
	registry->tensor_datatypes = CCV_32F;
	registry->tensor_memory = CCV_TENSOR_CPU_MEMORY;
	registry->algorithms = 1;
	registry->exec = _ccv_nnc_set_forw;
}

REGISTER_COMMAND_BACKEND(CCV_NNC_SET_BACKWARD, CCV_NNC_BACKEND_CPU_REF)(ccv_nnc_cmd_backend_registry_t* const registry)
{
	registry->tensor_formats = CCV_TENSOR_FORMAT_NCHW | CCV_TENSOR_FORMAT_NHWC | CCV_TENSOR_FORMAT_CHWN;
	registry->tensor_datatypes = CCV_32F;
	registry->tensor_memory = CCV_TENSOR_CPU_MEMORY;
	registry->algorithms = 1;
	registry->exec = _ccv_nnc_set_back;
}

static void _ccv_nnc_tensor_nhwc_nchw(const ccv_nnc_tensor_view_t* a, ccv_nnc_tensor_view_t* b)
{
	// Assuming this is float 32.
	int ainc[CCV_NNC_MAX_DIM + 2];
	int binc[CCV_NNC_MAX_DIM + 2];
	int k;
	// In case it is Toll-free bridged matrix object (NHWC format is possible).
	const int a_nd = ccv_nnc_tensor_nd(a->info.dim);
	const int b_nd = ccv_nnc_tensor_nd(b->info.dim);
	const int a_offset = CCV_NNC_MAX_DIM + 2 - a_nd;
	assert(a_offset == 0 || a_offset == 1);
	const int b_offset = CCV_NNC_MAX_DIM + 2 - b_nd;
	assert(b_offset == 0 || b_offset == 1);
	ccv_nnc_tensor_view_get_inc(a, ainc);
	ccv_nnc_tensor_view_get_inc(b, binc);
	// Comparing N
	assert((a_offset == 0 ? a->info.dim[0] : 1) == (b_offset == 0 ? b->info.dim[0] : 1));
	const int n = (a_offset == 0 ? a->info.dim[0] : 1);
	// Comparing C
	assert(a->info.dim[a_nd - 1] == b->info.dim[1 - b_offset]);
	const int c = a->info.dim[a_nd - 1];
	// Comparing HW
	int hw[CCV_NNC_MAX_DIM];
	for (k = 0; k < CCV_NNC_MAX_DIM; k++)
	{
		assert(a->info.dim[k + 1 - a_offset] == b->info.dim[k + 2 - b_offset]);
		hw[k] = a->info.dim[k + 1 - a_offset];
	}
	assert(CCV_NNC_MAX_DIM == 2); // Need to change this logic for CCV_NNC_MAX_DIM == other number.
	int i[CCV_NNC_MAX_DIM + 2];
	float* ap = a->data.f32;
	float* bp = b->data.f32;
	// Non-optimal case, need to do skip copy.
	for (i[0] = 0; i[0] < n; i[0]++)
	{
		for (i[3] = 0; i[3] < c; i[3]++)
		{
			float* apu = ap + i[3];
			for (i[1] = 0; i[1] < hw[0]; i[1]++)
			{
				for (i[2] = 0; i[2] < hw[1]; i[2]++)
					bp[i[2]] = apu[i[2] * ainc[3]];
				apu += ainc[2] * ainc[3];
				bp += binc[3];
			}
			bp += (binc[2] - hw[0]) * binc[3];
		}
		ap += ainc[1] * ainc[2] * ainc[3];
		bp += (binc[1] - c) * binc[2] * binc[3];
	}
}

static void _ccv_nnc_tensor_nchw_nhwc(const ccv_nnc_tensor_view_t* a, ccv_nnc_tensor_view_t* b)
{
	// Assuming this is float 32.
	int ainc[CCV_NNC_MAX_DIM + 2];
	int binc[CCV_NNC_MAX_DIM + 2];
	int k;
	// In case it is Toll-free bridged matrix object (NHWC format is possible).
	const int a_nd = ccv_nnc_tensor_nd(a->info.dim);
	const int b_nd = ccv_nnc_tensor_nd(b->info.dim);
	const int a_offset = CCV_NNC_MAX_DIM + 2 - a_nd;
	assert(a_offset == 0 || a_offset == 1);
	const int b_offset = CCV_NNC_MAX_DIM + 2 - b_nd;
	assert(b_offset == 0 || b_offset == 1);
	ccv_nnc_tensor_view_get_inc(a, ainc);
	ccv_nnc_tensor_view_get_inc(b, binc);
	// Comparing N
	assert((a_offset == 0 ? a->info.dim[0] : 1) == (b_offset == 0 ? b->info.dim[0] : 1));
	const int n = (a_offset == 0 ? a->info.dim[0] : 1);
	// Comparing C
	assert(a->info.dim[1 - a_offset] == b->info.dim[b_nd - 1]);
	const int c = a->info.dim[1 - a_offset];
	// Comparing HW
	int hw[CCV_NNC_MAX_DIM];
	for (k = 0; k < CCV_NNC_MAX_DIM; k++)
	{
		assert(a->info.dim[k + 2 - a_offset] == b->info.dim[k + 1 - b_offset]);
		hw[k] = a->info.dim[k + 2 - a_offset];
	}
	assert(CCV_NNC_MAX_DIM == 2); // Need to change this logic for CCV_NNC_MAX_DIM == other number.
	int i[CCV_NNC_MAX_DIM + 2];
	float* ap = a->data.f32;
	float* bp = b->data.f32;
	// Non-optimal case, need to do skip copy.
	for (i[0] = 0; i[0] < n; i[0]++)
	{
		for (i[3] = 0; i[3] < c; i[3]++)
		{
			float* bpu = bp + i[3];
			for (i[1] = 0; i[1] < hw[0]; i[1]++)
			{
				for (i[2] = 0; i[2] < hw[1]; i[2]++)
					bpu[i[2] * binc[3]] = ap[i[2]];
				ap += ainc[3];
				bpu += binc[2] * binc[3];
			}
			ap += (ainc[2] - hw[0]) * ainc[3];
		}
		ap += (ainc[1] - c) * ainc[2] * ainc[3];
		bp += binc[1] * binc[2] * binc[3];
	}
}

static int _ccv_nnc_format_transform(const ccv_nnc_cmd_t cmd, const ccv_nnc_hint_t hint, const int flags, ccv_nnc_tensor_t* const* const inputs, const int input_size, ccv_nnc_tensor_t* const* const outputs, const int output_size, ccv_nnc_stream_context_t* const stream_context)
{
	assert(output_size <= input_size);
	int i;
	for (i = 0; i < output_size; i++)
	{
		const ccv_nnc_tensor_view_t* a = (ccv_nnc_tensor_view_t*)inputs[i];
		ccv_nnc_tensor_view_t* b = (ccv_nnc_tensor_view_t*)outputs[i];
		assert(a != b); // Cannot do inplace transform.
		if (a->info.format == b->info.format) {
			// If it is the same, just do a normal data transfer.
			_ccv_nnc_tensor_transfer_cpu_ref_f32(a, b);
		} else if (a->info.format == CCV_TENSOR_FORMAT_NHWC && b->info.format == CCV_TENSOR_FORMAT_NCHW) {
			_ccv_nnc_tensor_nhwc_nchw(a, b);
		} else if (a->info.format == CCV_TENSOR_FORMAT_NHWC && b->info.format == CCV_TENSOR_FORMAT_CHWN) {
		} else if (a->info.format == CCV_TENSOR_FORMAT_NCHW && b->info.format == CCV_TENSOR_FORMAT_NHWC) {
			_ccv_nnc_tensor_nchw_nhwc(a, b);
		} else if (a->info.format == CCV_TENSOR_FORMAT_NCHW && b->info.format == CCV_TENSOR_FORMAT_CHWN) {
		} else if (a->info.format == CCV_TENSOR_FORMAT_CHWN && b->info.format == CCV_TENSOR_FORMAT_NHWC) {
		} else if (a->info.format == CCV_TENSOR_FORMAT_CHWN && b->info.format == CCV_TENSOR_FORMAT_NCHW) {
		}
	}
	return CCV_NNC_EXEC_SUCCESS;
}

REGISTER_COMMAND_BACKEND(CCV_NNC_FORMAT_TRANSFORM_FORWARD, CCV_NNC_BACKEND_CPU_REF)(ccv_nnc_cmd_backend_registry_t* const registry)
{
	registry->tensor_formats = CCV_TENSOR_FORMAT_NCHW | CCV_TENSOR_FORMAT_NHWC | CCV_TENSOR_FORMAT_CHWN;
	registry->tensor_datatypes = CCV_32F | CCV_32S;
	registry->tensor_memory = CCV_TENSOR_CPU_MEMORY;
	registry->algorithms = 1;
	registry->exec = _ccv_nnc_format_transform;
}

REGISTER_COMMAND_BACKEND(CCV_NNC_FORMAT_TRANSFORM_BACKWARD, CCV_NNC_BACKEND_CPU_REF)(ccv_nnc_cmd_backend_registry_t* const registry)
{
	registry->tensor_formats = CCV_TENSOR_FORMAT_NCHW | CCV_TENSOR_FORMAT_NHWC | CCV_TENSOR_FORMAT_CHWN;
	registry->tensor_datatypes = CCV_32F | CCV_32S;
	registry->tensor_memory = CCV_TENSOR_CPU_MEMORY;
	registry->algorithms = 1;
	registry->exec = _ccv_nnc_format_transform;
}

static int _ccv_nnc_transpose(const ccv_nnc_cmd_t cmd, const ccv_nnc_hint_t hint, const int flags, ccv_nnc_tensor_t* const* const inputs, const int input_size, ccv_nnc_tensor_t* const* const outputs, const int output_size, ccv_nnc_stream_context_t* const stream_context)
{
	assert(output_size <= input_size);
	int k;
	for (k = 0; k < output_size; k++)
	{
		const ccv_nnc_tensor_view_t* const a = (ccv_nnc_tensor_view_t*)inputs[k];
		ccv_nnc_tensor_view_t* const b = (ccv_nnc_tensor_view_t*)outputs[k];
		const int a_nd = ccv_nnc_tensor_nd(a->info.dim);
		const int b_nd = ccv_nnc_tensor_nd(b->info.dim);
		assert(a_nd == b_nd);
		assert(a_nd <= CCV_NNC_MAX_DIM + 2); // I can only handle maximum 4.
		assert(a_nd >= 2 && b_nd >= 2); // You cannot transpose if it is less than 2.
		assert(a->info.dim[cmd.info.transpose.axis[0]] == b->info.dim[cmd.info.transpose.axis[1]]);
		assert(a->info.dim[cmd.info.transpose.axis[1]] == b->info.dim[cmd.info.transpose.axis[0]]);
		int x;
		for (x = 0; x < a_nd; x++)
			if (x != cmd.info.transpose.axis[0] && x != cmd.info.transpose.axis[1])
				{ assert(a->info.dim[x] == b->info.dim[x]); }
		size_t astride[CCV_NNC_MAX_DIM + 2];
		size_t bstride[CCV_NNC_MAX_DIM + 2];
		int dim[CCV_NNC_MAX_DIM + 2];
		for (x = b_nd; x < CCV_NNC_MAX_DIM + 2; x++)
			dim[x] = 1;
		for (x = 0; x < b_nd; x++)
			dim[x] = b->info.dim[x];
		// Don't use ccv_nnc_tensor_view_get_inc or get_dim because these will prefill beginning to 1:
		// for example, if the dimension is [2, 4], it will fill to [1, 1, 2, 4] so the axis index will
		// be messed up.
		const int* const ainc = CCV_IS_TENSOR_VIEW(a) ? a->inc : a->info.dim;
		const int* const binc = CCV_IS_TENSOR_VIEW(b) ? b->inc : b->info.dim;
		for (x = a_nd - 1; x < CCV_NNC_MAX_DIM + 2; x++)
			astride[x] = 1;
		for (x = a_nd - 2; x >= 0; x--)
			astride[x] = astride[x + 1] * ainc[x + 1];
		for (x = b_nd - 1; x < CCV_NNC_MAX_DIM + 2; x++)
			bstride[x] = 1;
		for (x = b_nd - 2; x >= 0; x--)
			bstride[x] = bstride[x + 1] * binc[x + 1];
		const float* const ap = a->data.f32;
		float* const bp = b->data.f32;
		int i[CCV_NNC_MAX_DIM + 2];
		int j[CCV_NNC_MAX_DIM + 2] = {
			0, 1, 2, 3
		};
		CCV_SWAP(j[cmd.info.transpose.axis[0]], j[cmd.info.transpose.axis[1]], x);
		for (i[0] = 0; i[0] < dim[0]; i[0]++)
		{
			float* const bp0 = bp + i[0] * bstride[0];
			for (i[1] = 0; i[1] < dim[1]; i[1]++)
			{
				float* const bp1 = bp0 + i[1] * bstride[1];
				for (i[2] = 0; i[2] < dim[2]; i[2]++)
				{
					float* const bp2 = bp1 + i[2] * bstride[2];
					for (i[3] = 0; i[3] < dim[3]; i[3]++)
						bp2[i[3]] = ap[i[j[0]] * astride[0] + i[j[1]] * astride[1] + i[j[2]] * astride[2] + i[j[3]] * astride[3]];
				}
			}
		}
	}
	return CCV_NNC_EXEC_SUCCESS;
}

REGISTER_COMMAND_BACKEND(CCV_NNC_TRANSPOSE_FORWARD, CCV_NNC_BACKEND_CPU_REF)(ccv_nnc_cmd_backend_registry_t* const registry)
{
	registry->tensor_formats = CCV_TENSOR_FORMAT_NCHW | CCV_TENSOR_FORMAT_NHWC | CCV_TENSOR_FORMAT_CHWN;
	registry->tensor_datatypes = CCV_32F;
	registry->tensor_memory = CCV_TENSOR_CPU_MEMORY;
	registry->algorithms = 1;
	registry->exec = _ccv_nnc_transpose;
}

REGISTER_COMMAND_BACKEND(CCV_NNC_TRANSPOSE_BACKWARD, CCV_NNC_BACKEND_CPU_REF)(ccv_nnc_cmd_backend_registry_t* const registry)
{
	registry->tensor_formats = CCV_TENSOR_FORMAT_NCHW | CCV_TENSOR_FORMAT_NHWC | CCV_TENSOR_FORMAT_CHWN;
	registry->tensor_datatypes = CCV_32F;
	registry->tensor_memory = CCV_TENSOR_CPU_MEMORY;
	registry->algorithms = 1;
	registry->exec = _ccv_nnc_transpose;
}

static int _ccv_nnc_datatype_conversion(const ccv_nnc_cmd_t cmd, const ccv_nnc_hint_t hint, const int flags, ccv_nnc_tensor_t* const* const inputs, const int input_size, ccv_nnc_tensor_t* const* const outputs, const int output_size, ccv_nnc_stream_context_t* const stream_context)
{
	assert(output_size <= input_size);
	int i;
	for (i = 0; i < output_size; i++)
	{
		const ccv_nnc_tensor_view_t* a = (ccv_nnc_tensor_view_t*)inputs[i];
		ccv_nnc_tensor_view_t* b = (ccv_nnc_tensor_view_t*)outputs[i];
		assert(a != b); // Cannot do inplace transform.
		assert(a->info.format == b->info.format);
		if (a->info.datatype == b->info.datatype) {
			// If it is the same, just do a normal data transfer.
			if (a->info.datatype == CCV_16F)
				_ccv_nnc_tensor_transfer_cpu_ref_f16(a, b);
			else if (a->info.datatype == CCV_32F)
				_ccv_nnc_tensor_transfer_cpu_ref_f32(a, b);
			else if (a->info.datatype == CCV_64F)
				_ccv_nnc_tensor_transfer_cpu_ref_f64(a, b);
		} else if (a->info.datatype == CCV_32F && b->info.datatype == CCV_16F) {
			assert(!CCV_IS_TENSOR_VIEW(a));
			assert(!CCV_IS_TENSOR_VIEW(b));
			const size_t tensor_count = ccv_nnc_tensor_count(a->info);
			assert(tensor_count == ccv_nnc_tensor_count(b->info));
			ccv_float_to_half_precision(a->data.f32, (uint16_t*)b->data.f16, tensor_count);
		} else if (a->info.datatype == CCV_16F && b->info.datatype == CCV_32F) {
			assert(!CCV_IS_TENSOR_VIEW(a));
			assert(!CCV_IS_TENSOR_VIEW(b));
			const int tensor_count = ccv_nnc_tensor_count(a->info);
			assert(tensor_count == ccv_nnc_tensor_count(b->info));
			ccv_half_precision_to_float((uint16_t*)a->data.f16, b->data.f32, tensor_count);
		} else if (a->info.datatype == CCV_64F && b->info.datatype == CCV_32F) {
			assert(!CCV_IS_TENSOR_VIEW(a));
			assert(!CCV_IS_TENSOR_VIEW(b));
			const size_t tensor_count = ccv_nnc_tensor_count(a->info);
			assert(tensor_count == ccv_nnc_tensor_count(b->info));
			int i;
			for (i = 0; i < tensor_count; i++)
				b->data.f32[i] = (float)a->data.f64[i];
		} else if (a->info.datatype == CCV_32F && b->info.datatype == CCV_64F) {
			assert(!CCV_IS_TENSOR_VIEW(a));
			assert(!CCV_IS_TENSOR_VIEW(b));
			const int tensor_count = ccv_nnc_tensor_count(a->info);
			assert(tensor_count == ccv_nnc_tensor_count(b->info));
			for (i = 0; i < tensor_count; i++)
				b->data.f64[i] = (double)a->data.f32[i];
		} else if (a->info.datatype == CCV_64F && b->info.datatype == CCV_16F) {
			assert(!CCV_IS_TENSOR_VIEW(a));
			assert(!CCV_IS_TENSOR_VIEW(b));
			const size_t tensor_count = ccv_nnc_tensor_count(a->info);
			assert(tensor_count == ccv_nnc_tensor_count(b->info));
			ccv_double_to_half_precision(a->data.f64, (uint16_t*)b->data.f16, tensor_count);
		} else if (a->info.datatype == CCV_16F && b->info.datatype == CCV_64F) {
			assert(!CCV_IS_TENSOR_VIEW(a));
			assert(!CCV_IS_TENSOR_VIEW(b));
			const int tensor_count = ccv_nnc_tensor_count(a->info);
			assert(tensor_count == ccv_nnc_tensor_count(b->info));
			ccv_half_precision_to_double((uint16_t*)a->data.f16, b->data.f64, tensor_count);
		}
	}
	return CCV_NNC_EXEC_SUCCESS;
}

REGISTER_COMMAND_BACKEND(CCV_NNC_DATATYPE_CONVERSION_FORWARD, CCV_NNC_BACKEND_CPU_REF)(ccv_nnc_cmd_backend_registry_t* const registry)
{
	registry->tensor_formats = CCV_TENSOR_FORMAT_NCHW | CCV_TENSOR_FORMAT_NHWC | CCV_TENSOR_FORMAT_CHWN;
	registry->tensor_datatypes = CCV_64F | CCV_32F | CCV_16F;
	registry->tensor_memory = CCV_TENSOR_CPU_MEMORY;
	registry->algorithms = 1;
	registry->exec = _ccv_nnc_datatype_conversion;
}

REGISTER_COMMAND_BACKEND(CCV_NNC_DATATYPE_CONVERSION_BACKWARD, CCV_NNC_BACKEND_CPU_REF)(ccv_nnc_cmd_backend_registry_t* const registry)
{
	registry->tensor_formats = CCV_TENSOR_FORMAT_NCHW | CCV_TENSOR_FORMAT_NHWC | CCV_TENSOR_FORMAT_CHWN;
	registry->tensor_datatypes = CCV_64F | CCV_32F | CCV_16F;
	registry->tensor_memory = CCV_TENSOR_CPU_MEMORY;
	registry->algorithms = 1;
	registry->exec = _ccv_nnc_datatype_conversion;
}

static void _ccv_nnc_masked_fill_cpu_ref_f(const float p, const float q, ccv_nnc_tensor_view_t* const a, ccv_nnc_tensor_view_t* const b, ccv_nnc_tensor_view_t* const c)
{
	int cdim[CCV_NNC_MAX_DIM_ALLOC];
	ccv_nnc_tensor_view_get_dim(a, cdim); // Fill in cdim first.
	ccv_nnc_tensor_view_get_broadcast_dim(b, cdim);
	assert(ccv_nnc_tensor_view_check_broadcast_dim(a, cdim));
	assert(ccv_nnc_tensor_view_check_broadcast_dim(b, cdim));
	const int a_check_dim = ccv_nnc_tensor_view_check_dim(a, cdim);
	const int b_check_dim = ccv_nnc_tensor_view_check_dim(b, cdim);
	// Assuming this is float 32.
	int adim[CCV_NNC_MAX_DIM_ALLOC];
	int bdim[CCV_NNC_MAX_DIM_ALLOC];
	ccv_nnc_tensor_view_get_dim(a, adim);
	ccv_nnc_tensor_view_get_dim(b, bdim);
	int ainc[CCV_NNC_MAX_DIM_ALLOC];
	int binc[CCV_NNC_MAX_DIM_ALLOC];
	int cinc[CCV_NNC_MAX_DIM_ALLOC];
	assert(ccv_nnc_tensor_view_check_dim(c, cdim));
	int x;
	if (!CCV_IS_TENSOR_VIEW(a) && !CCV_IS_TENSOR_VIEW(b) && !CCV_IS_TENSOR_VIEW(c) && a_check_dim && b_check_dim)
	{
		const int tensor_count = ccv_nnc_tensor_count(a->info);
		// Super optimal case, just do one for-loop for sum.
		for (x = 0; x < tensor_count; x++)
			c->data.f32[x] = (b->data.f32[x] == p) ? q : a->data.f32[x];
		return;
	}
	assert(CCV_NNC_MAX_DIM == 2); // Need to change this logic for CCV_NNC_MAX_DIM == other number.
	ccv_nnc_tensor_view_get_inc(a, ainc);
	ccv_nnc_tensor_view_get_inc(b, binc);
	ccv_nnc_tensor_view_get_inc(c, cinc);
	int i[CCV_NNC_MAX_DIM + 2];
	float* ap = a->data.f32;
	float* bp = b->data.f32;
	float* cp = c->data.f32;
	const int count = cdim[2] * cdim[3];
	if (ainc[3] == cdim[3] && binc[3] == cdim[3] && cinc[3] == cdim[3] && adim[2] == cdim[2] && bdim[2] == cdim[2])
	{
		// Special casing if the ainc[3] is the same as dim[3]
		for (i[0] = 0; i[0] < cdim[0]; i[0]++)
		{
			float* const ap0 = adim[0] == 1 ? ap : ap + i[0] * ainc[1] * ainc[2] * ainc[3];
			float* const bp0 = bdim[0] == 1 ? bp : bp + i[0] * binc[1] * binc[2] * binc[3];
			for (i[1] = 0; i[1] < cdim[1]; i[1]++)
			{
				float* const ap1 = adim[1] == 1 ? ap0 : ap0 + i[1] * ainc[2] * ainc[3];
				float* const bp1 = bdim[1] == 1 ? bp0 : bp0 + i[1] * binc[2] * binc[3];
				for (x = 0; x < count; x++)
					cp[x] = (bp1[x] == p) ? q : ap1[x];
				cp += cinc[2] * cinc[3];
			}
			cp += (cinc[1] - cdim[1]) * cinc[2] * cinc[3];
		}
		return;
	}
	// Non-optimal case, need to do skip copy and handle broadcasting.
	for (i[0] = 0; i[0] < cdim[0]; i[0]++)
	{
		float* const ap0 = adim[0] == 1 ? ap : ap + i[0] * ainc[1] * ainc[2] * ainc[3];
		float* const bp0 = bdim[0] == 1 ? bp : bp + i[0] * binc[1] * binc[2] * binc[3];
		for (i[1] = 0; i[1] < cdim[1]; i[1]++)
		{
			float* const ap1 = adim[1] == 1 ? ap0 : ap0 + i[1] * ainc[2] * ainc[3];
			float* const bp1 = bdim[1] == 1 ? bp0 : bp0 + i[1] * binc[2] * binc[3];
			for (i[2] = 0; i[2] < cdim[2]; i[2]++)
			{
				float* const ap2 = adim[2] == 1 ? ap1 : ap1 + i[2] * ainc[3];
				float* const bp2 = bdim[2] == 1 ? bp1 : bp1 + i[2] * binc[3];
				if (adim[3] == 1)
					for (x = 0; x < cdim[3]; x++)
						cp[x] = (bp2[x] == p) ? q : ap2[0];
				else if (bdim[3] == 1)
					if (bp2[0] == p)
						for (x = 0; x < cdim[3]; x++)
							cp[x] = q;
					else
						for (x = 0; x < cdim[3]; x++)
							cp[x] = ap2[x];
				else
					for (x = 0; x < cdim[3]; x++)
						cp[x] = (bp2[x] == p) ? q : ap2[x];
				cp += cinc[3];
			}
			cp += (cinc[2] - cdim[2]) * cinc[3];
		}
		cp += (cinc[1] - cdim[1]) * cinc[2] * cinc[3];
	}
}

static void _ccv_nnc_masked_fill_cpu_ref_s(const int p, const float q, ccv_nnc_tensor_view_t* const a, ccv_nnc_tensor_view_t* const b, ccv_nnc_tensor_view_t* const c)
{
	int cdim[CCV_NNC_MAX_DIM_ALLOC];
	ccv_nnc_tensor_view_get_dim(a, cdim); // Fill in cdim first.
	ccv_nnc_tensor_view_get_broadcast_dim(b, cdim);
	assert(ccv_nnc_tensor_view_check_broadcast_dim(a, cdim));
	assert(ccv_nnc_tensor_view_check_broadcast_dim(b, cdim));
	const int a_check_dim = ccv_nnc_tensor_view_check_dim(a, cdim);
	const int b_check_dim = ccv_nnc_tensor_view_check_dim(b, cdim);
	// Assuming this is float 32.
	int adim[CCV_NNC_MAX_DIM_ALLOC];
	int bdim[CCV_NNC_MAX_DIM_ALLOC];
	ccv_nnc_tensor_view_get_dim(a, adim);
	ccv_nnc_tensor_view_get_dim(b, bdim);
	int ainc[CCV_NNC_MAX_DIM_ALLOC];
	int binc[CCV_NNC_MAX_DIM_ALLOC];
	int cinc[CCV_NNC_MAX_DIM_ALLOC];
	assert(ccv_nnc_tensor_view_check_dim(c, cdim));
	int x;
	if (!CCV_IS_TENSOR_VIEW(a) && !CCV_IS_TENSOR_VIEW(b) && !CCV_IS_TENSOR_VIEW(c) && a_check_dim && b_check_dim)
	{
		const int tensor_count = ccv_nnc_tensor_count(a->info);
		// Super optimal case, just do one for-loop for sum.
		for (x = 0; x < tensor_count; x++)
			c->data.f32[x] = (b->data.i32[x] == p) ? q : a->data.f32[x];
		return;
	}
	assert(CCV_NNC_MAX_DIM == 2); // Need to change this logic for CCV_NNC_MAX_DIM == other number.
	ccv_nnc_tensor_view_get_inc(a, ainc);
	ccv_nnc_tensor_view_get_inc(b, binc);
	ccv_nnc_tensor_view_get_inc(c, cinc);
	int i[CCV_NNC_MAX_DIM + 2];
	float* ap = a->data.f32;
	int* bp = b->data.i32;
	float* cp = c->data.f32;
	const int count = cdim[2] * cdim[3];
	if (ainc[3] == cdim[3] && binc[3] == cdim[3] && cinc[3] == cdim[3] && adim[2] == cdim[2] && bdim[2] == cdim[2])
	{
		// Special casing if the ainc[3] is the same as dim[3]
		for (i[0] = 0; i[0] < cdim[0]; i[0]++)
		{
			float* const ap0 = adim[0] == 1 ? ap : ap + i[0] * ainc[1] * ainc[2] * ainc[3];
			int* const bp0 = bdim[0] == 1 ? bp : bp + i[0] * binc[1] * binc[2] * binc[3];
			for (i[1] = 0; i[1] < cdim[1]; i[1]++)
			{
				float* const ap1 = adim[1] == 1 ? ap0 : ap0 + i[1] * ainc[2] * ainc[3];
				int* const bp1 = bdim[1] == 1 ? bp0 : bp0 + i[1] * binc[2] * binc[3];
				for (x = 0; x < count; x++)
					cp[x] = (bp1[x] == p) ? q : ap1[x];
				cp += cinc[2] * cinc[3];
			}
			cp += (cinc[1] - cdim[1]) * cinc[2] * cinc[3];
		}
		return;
	}
	// Non-optimal case, need to do skip copy and handle broadcasting.
	for (i[0] = 0; i[0] < cdim[0]; i[0]++)
	{
		float* const ap0 = adim[0] == 1 ? ap : ap + i[0] * ainc[1] * ainc[2] * ainc[3];
		int* const bp0 = bdim[0] == 1 ? bp : bp + i[0] * binc[1] * binc[2] * binc[3];
		for (i[1] = 0; i[1] < cdim[1]; i[1]++)
		{
			float* const ap1 = adim[1] == 1 ? ap0 : ap0 + i[1] * ainc[2] * ainc[3];
			int* const bp1 = bdim[1] == 1 ? bp0 : bp0 + i[1] * binc[2] * binc[3];
			for (i[2] = 0; i[2] < cdim[2]; i[2]++)
			{
				float* const ap2 = adim[2] == 1 ? ap1 : ap1 + i[2] * ainc[3];
				int* const bp2 = bdim[2] == 1 ? bp1 : bp1 + i[2] * binc[3];
				if (adim[3] == 1)
					for (x = 0; x < cdim[3]; x++)
						cp[x] = (bp2[x] == p) ? q : ap2[0];
				else if (bdim[3] == 1)
					if (bp2[0] == p)
						for (x = 0; x < cdim[3]; x++)
							cp[x] = q;
					else
						for (x = 0; x < cdim[3]; x++)
							cp[x] = ap2[x];
				else
					for (x = 0; x < cdim[3]; x++)
						cp[x] = (bp2[x] == p) ? q : ap2[x];
				cp += cinc[3];
			}
			cp += (cinc[2] - cdim[2]) * cinc[3];
		}
		cp += (cinc[1] - cdim[1]) * cinc[2] * cinc[3];
	}
}

static int _ccv_nnc_masked_fill_forw(const ccv_nnc_cmd_t cmd, const ccv_nnc_hint_t hint, const int flags, ccv_nnc_tensor_t* const* const inputs, const int input_size, ccv_nnc_tensor_t* const* const outputs, const int output_size, ccv_nnc_stream_context_t* const stream_context)
{
	assert(input_size >= 2);
	assert(inputs[0]);
	assert(inputs[1]);
	assert(outputs[0]);
	if (inputs[1]->info.datatype == CCV_32F)
		_ccv_nnc_masked_fill_cpu_ref_f(cmd.info.blas.a[0], cmd.info.blas.a[1], (ccv_nnc_tensor_view_t*)inputs[0], (ccv_nnc_tensor_view_t*)inputs[1], (ccv_nnc_tensor_view_t*)outputs[0]);
	else if (inputs[1]->info.datatype == CCV_32S)
		_ccv_nnc_masked_fill_cpu_ref_s((int)(cmd.info.blas.a[0] + 0.5), cmd.info.blas.a[1], (ccv_nnc_tensor_view_t*)inputs[0], (ccv_nnc_tensor_view_t*)inputs[1], (ccv_nnc_tensor_view_t*)outputs[0]);
	return CCV_NNC_EXEC_SUCCESS;
}

static int _ccv_nnc_masked_fill_back(const ccv_nnc_cmd_t cmd, const ccv_nnc_hint_t hint, const int flags, ccv_nnc_tensor_t* const* const inputs, const int input_size, ccv_nnc_tensor_t* const* const outputs, const int output_size, ccv_nnc_stream_context_t* const stream_context)
{
	assert(input_size >= 3);
	if (inputs[2]->info.datatype == CCV_32F)
		_ccv_nnc_masked_fill_cpu_ref_f(cmd.info.blas.a[0], 0, (ccv_nnc_tensor_view_t*)inputs[0], (ccv_nnc_tensor_view_t*)inputs[2], (ccv_nnc_tensor_view_t*)outputs[0]);
	else if (inputs[2]->info.datatype == CCV_32S)
		_ccv_nnc_masked_fill_cpu_ref_s((int)(cmd.info.blas.a[0] + 0.5), 0, (ccv_nnc_tensor_view_t*)inputs[0], (ccv_nnc_tensor_view_t*)inputs[2], (ccv_nnc_tensor_view_t*)outputs[0]);
	// TODO: doesn't really support taking gradient on mask.
	// if (output_size >= 2 && outputs[1])
	return CCV_NNC_EXEC_SUCCESS;
}

REGISTER_COMMAND_BACKEND(CCV_NNC_MASKED_FILL_FORWARD, CCV_NNC_BACKEND_CPU_REF)(ccv_nnc_cmd_backend_registry_t* const registry)
{
	registry->tensor_formats = CCV_TENSOR_FORMAT_NCHW | CCV_TENSOR_FORMAT_NHWC | CCV_TENSOR_FORMAT_CHWN;
	registry->tensor_datatypes = CCV_32F | CCV_32S;
	registry->tensor_memory = CCV_TENSOR_CPU_MEMORY;
	registry->algorithms = 1;
	registry->exec = _ccv_nnc_masked_fill_forw;
}

REGISTER_COMMAND_BACKEND(CCV_NNC_MASKED_FILL_BACKWARD, CCV_NNC_BACKEND_CPU_REF)(ccv_nnc_cmd_backend_registry_t* const registry)
{
	registry->tensor_formats = CCV_TENSOR_FORMAT_NCHW | CCV_TENSOR_FORMAT_NHWC | CCV_TENSOR_FORMAT_CHWN;
	registry->tensor_datatypes = CCV_32F | CCV_32S;
	registry->tensor_memory = CCV_TENSOR_CPU_MEMORY;
	registry->algorithms = 1;
	registry->exec = _ccv_nnc_masked_fill_back;
}
