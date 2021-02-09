#include "case.h"
#include "ccv_case.h"
#include "ccv_nnc_case.h"
#include <ccv.h>
#include <nnc/ccv_nnc.h>
#include <nnc/ccv_nnc_easy.h>
#include <3rdparty/dsfmt/dSFMT.h>

TEST_SETUP()
{
	ccv_nnc_init();
}

TEST_CASE("upsample NHWC in float")
{
	GUARD_ELSE_RETURN(ccv_nnc_cmd_ok(CCV_NNC_UPSAMPLE_BILINEAR_FORWARD, CCV_NNC_BACKEND_GPU_REF));
	ccv_dense_matrix_t* image = 0;
	ccv_read("../../../samples/chessbox.png", &image, CCV_IO_ANY_FILE | CCV_IO_RGB_COLOR);
	ccv_dense_matrix_t* fimage = 0;
	ccv_shift(image, (ccv_matrix_t**)&fimage, CCV_32F, 0, 0);
	ccv_nnc_tensor_t* const a = ccv_nnc_tensor_new(0, GPU_TENSOR_NHWC(000, 32F, image->rows, image->cols, 3), 0);
	ccv_nnc_cmd_exec(CMD_DATA_TRANSFER_FORWARD(), ccv_nnc_no_hint, 0, TENSOR_LIST((ccv_nnc_tensor_t*)fimage), TENSOR_LIST(a), 0);
	ccv_matrix_free(fimage);
	ccv_nnc_tensor_t* const b = ccv_nnc_tensor_new(0, GPU_TENSOR_NHWC(000, 32F, image->rows * 2, image->cols * 2, 3), 0);
	ccv_nnc_cmd_exec(CMD_UPSAMPLE_BILINEAR_FORWARD(2, 2), ccv_nnc_no_hint, 0, TENSOR_LIST(a), TENSOR_LIST(b), 0);
	ccv_nnc_tensor_t* const hb = ccv_nnc_tensor_new(0, CPU_TENSOR_NHWC(32F, image->rows * 2, image->cols * 2, 3), 0);
	ccv_nnc_cmd_exec(CMD_DATA_TRANSFER_FORWARD(), ccv_nnc_no_hint, 0, TENSOR_LIST(b), TENSOR_LIST(hb), 0);
	REQUIRE_MATRIX_FILE_EQ((ccv_matrix_t*)hb, "../../unit/nnc/data/upsample.forward.bin", "the forward of upsample should be equal");
	ccv_nnc_tensor_free(a);
	ccv_nnc_tensor_free(b);
	ccv_nnc_tensor_free(hb);
	ccv_matrix_free(image);
}

TEST_CASE("upsample NCHW in float")
{
	GUARD_ELSE_RETURN(ccv_nnc_cmd_ok(CCV_NNC_UPSAMPLE_BILINEAR_FORWARD, CCV_NNC_BACKEND_GPU_REF));
	ccv_dense_matrix_t* image = 0;
	ccv_read("../../../samples/chessbox.png", &image, CCV_IO_ANY_FILE | CCV_IO_RGB_COLOR);
	ccv_dense_matrix_t* fimage = 0;
	ccv_shift(image, (ccv_matrix_t**)&fimage, CCV_32F, 0, 0);
	ccv_nnc_tensor_t* const a = ccv_nnc_tensor_new(0, GPU_TENSOR_NHWC(000, 32F, image->rows, image->cols, 3), 0);
	ccv_nnc_cmd_exec(CMD_DATA_TRANSFER_FORWARD(), ccv_nnc_no_hint, 0, TENSOR_LIST((ccv_nnc_tensor_t*)fimage), TENSOR_LIST(a), 0);
	ccv_matrix_free(fimage);
	ccv_nnc_tensor_t* const at = ccv_nnc_tensor_new(0, GPU_TENSOR_NCHW(000, 32F, 3, image->rows, image->cols), 0);
	ccv_nnc_cmd_exec(CMD_FORMAT_TRANSFORM_FORWARD(), ccv_nnc_no_hint, 0, TENSOR_LIST(a), TENSOR_LIST(at), 0);
	ccv_nnc_tensor_t* const bt = ccv_nnc_tensor_new(0, GPU_TENSOR_NCHW(000, 32F, 3, image->rows * 2, image->cols * 2), 0);
	ccv_nnc_cmd_exec(CMD_UPSAMPLE_BILINEAR_FORWARD(2, 2), ccv_nnc_no_hint, 0, TENSOR_LIST(at), TENSOR_LIST(bt), 0);
	ccv_nnc_tensor_t* const b = ccv_nnc_tensor_new(0, GPU_TENSOR_NHWC(000, 32F, image->rows * 2, image->cols * 2, 3), 0);
	ccv_nnc_cmd_exec(CMD_FORMAT_TRANSFORM_FORWARD(), ccv_nnc_no_hint, 0, TENSOR_LIST(bt), TENSOR_LIST(b), 0);
	ccv_nnc_tensor_t* const hb = ccv_nnc_tensor_new(0, CPU_TENSOR_NHWC(32F, image->rows * 2, image->cols * 2, 3), 0);
	ccv_nnc_cmd_exec(CMD_DATA_TRANSFER_FORWARD(), ccv_nnc_no_hint, 0, TENSOR_LIST(b), TENSOR_LIST(hb), 0);
	REQUIRE_MATRIX_FILE_EQ((ccv_matrix_t*)hb, "../../unit/nnc/data/upsample.forward.bin", "the forward of upsample should be equal");
	ccv_nnc_tensor_free(a);
	ccv_nnc_tensor_free(at);
	ccv_nnc_tensor_free(bt);
	ccv_nnc_tensor_free(b);
	ccv_nnc_tensor_free(hb);
	ccv_matrix_free(image);
}

TEST_CASE("downsample NHWC in float")
{
	GUARD_ELSE_RETURN(ccv_nnc_cmd_ok(CCV_NNC_UPSAMPLE_BILINEAR_FORWARD, CCV_NNC_BACKEND_GPU_REF));
	ccv_dense_matrix_t* image = 0;
	ccv_read("../../../samples/chessbox.png", &image, CCV_IO_ANY_FILE | CCV_IO_RGB_COLOR);
	ccv_dense_matrix_t* fimage = 0;
	ccv_shift(image, (ccv_matrix_t**)&fimage, CCV_32F, 0, 0);
	ccv_nnc_tensor_t* const a = ccv_nnc_tensor_new(0, GPU_TENSOR_NHWC(000, 32F, image->rows, image->cols, 3), 0);
	ccv_nnc_cmd_exec(CMD_DATA_TRANSFER_FORWARD(), ccv_nnc_no_hint, 0, TENSOR_LIST((ccv_nnc_tensor_t*)fimage), TENSOR_LIST(a), 0);
	ccv_matrix_free(fimage);
	ccv_nnc_tensor_t* const b = ccv_nnc_tensor_new(0, GPU_TENSOR_NHWC(000, 32F, image->rows / 2, image->cols / 2, 3), 0);
	ccv_nnc_cmd_exec(CMD_UPSAMPLE_BILINEAR_BACKWARD(2, 2), ccv_nnc_no_hint, 0, TENSOR_LIST(a), TENSOR_LIST(b), 0);
	ccv_nnc_tensor_t* const hb = ccv_nnc_tensor_new(0, CPU_TENSOR_NHWC(32F, image->rows / 2, image->cols / 2, 3), 0);
	ccv_nnc_cmd_exec(CMD_DATA_TRANSFER_FORWARD(), ccv_nnc_no_hint, 0, TENSOR_LIST(b), TENSOR_LIST(hb), 0);
	REQUIRE_MATRIX_FILE_EQ((ccv_matrix_t*)hb, "../../unit/nnc/data/upsample.backward.bin", "the backward of upsample should be equal");
	ccv_nnc_tensor_free(a);
	ccv_nnc_tensor_free(b);
	ccv_nnc_tensor_free(hb);
	ccv_matrix_free(image);
}

TEST_CASE("downsample NCHW in float")
{
	GUARD_ELSE_RETURN(ccv_nnc_cmd_ok(CCV_NNC_UPSAMPLE_BILINEAR_FORWARD, CCV_NNC_BACKEND_GPU_REF));
	ccv_dense_matrix_t* image = 0;
	ccv_read("../../../samples/chessbox.png", &image, CCV_IO_ANY_FILE | CCV_IO_RGB_COLOR);
	ccv_dense_matrix_t* fimage = 0;
	ccv_shift(image, (ccv_matrix_t**)&fimage, CCV_32F, 0, 0);
	ccv_nnc_tensor_t* const a = ccv_nnc_tensor_new(0, GPU_TENSOR_NHWC(000, 32F, image->rows, image->cols, 3), 0);
	ccv_nnc_cmd_exec(CMD_DATA_TRANSFER_FORWARD(), ccv_nnc_no_hint, 0, TENSOR_LIST((ccv_nnc_tensor_t*)fimage), TENSOR_LIST(a), 0);
	ccv_matrix_free(fimage);
	ccv_nnc_tensor_t* const at = ccv_nnc_tensor_new(0, GPU_TENSOR_NCHW(000, 32F, 3, image->rows, image->cols), 0);
	ccv_nnc_cmd_exec(CMD_FORMAT_TRANSFORM_FORWARD(), ccv_nnc_no_hint, 0, TENSOR_LIST(a), TENSOR_LIST(at), 0);
	ccv_nnc_tensor_t* const bt = ccv_nnc_tensor_new(0, GPU_TENSOR_NCHW(000, 32F, 3, image->rows / 2, image->cols / 2), 0);
	ccv_nnc_cmd_exec(CMD_UPSAMPLE_BILINEAR_BACKWARD(2, 2), ccv_nnc_no_hint, 0, TENSOR_LIST(at), TENSOR_LIST(bt), 0);
	ccv_nnc_tensor_t* const b = ccv_nnc_tensor_new(0, GPU_TENSOR_NHWC(000, 32F, image->rows / 2, image->cols / 2, 3), 0);
	ccv_nnc_cmd_exec(CMD_FORMAT_TRANSFORM_FORWARD(), ccv_nnc_no_hint, 0, TENSOR_LIST(bt), TENSOR_LIST(b), 0);
	ccv_nnc_tensor_t* const hb = ccv_nnc_tensor_new(0, CPU_TENSOR_NHWC(32F, image->rows / 2, image->cols / 2, 3), 0);
	ccv_nnc_cmd_exec(CMD_DATA_TRANSFER_FORWARD(), ccv_nnc_no_hint, 0, TENSOR_LIST(b), TENSOR_LIST(hb), 0);
	REQUIRE_MATRIX_FILE_EQ((ccv_matrix_t*)hb, "../../unit/nnc/data/upsample.backward.bin", "the backward of upsample should be equal");
	ccv_nnc_tensor_free(a);
	ccv_nnc_tensor_free(at);
	ccv_nnc_tensor_free(bt);
	ccv_nnc_tensor_free(b);
	ccv_nnc_tensor_free(hb);
	ccv_matrix_free(image);
}

#include "case_main.h"
