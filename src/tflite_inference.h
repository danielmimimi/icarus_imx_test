/* GStreamer i.MX NN Inference demo plugin
 *
 * Copyright 2021 NXP
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef tflite_inference_h
#define tflite_inference_h

#ifdef WITHOUT_TENSORFLOW
#else
#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/model_builder.h"
#endif

#include <vector>
#include <string>

class tflite_inference_t {
public:

	enum {
		OK = 0,
		ERROR = -1,
	};

	tflite_inference_t();
	virtual ~tflite_inference_t();

	int init(
		const std::string& model,
		int use_nnapi,
		int num_threads);

	virtual int inference(void);
	virtual int get_input_tensor_shape(std::vector<int>* shape);
	virtual int get_input_tensor(uint8_t** ptr, size_t* sz);
	virtual int get_input_tensor(float** ptr, size_t* sz);//DKLA

	bool verbose_ = false;

	int setup_input_tensor(int frame_height, int frame_width, int frame_depth, uint8_t* paddr);
	int setup_input_tensor(std::vector<int> framedim, uint8_t* paddr);//DKLA
	int setup_input_tensor_fast(uint8_t* paddr);

protected:


	template <typename T>
	T* typed_tensor(
		int index,
		size_t* length = NULL)
	{
#ifdef WITHOUT_TENSORFLOW
		return NULL;
#else
		if (length) {
			*length = 0;
			TfLiteIntArray* dims = interpreter_->tensor(index)->dims;
			if (dims && dims->size && dims->data[0]) {
				*length = 1;
				for (int i = 0; i < dims->size; i++) {
					*length *= dims->data[i];
				}
			}
		}
		return interpreter_->typed_tensor<T>(index);	
#endif
	}

	template <typename T>
	const T* typed_tensor(
		int index,
		size_t* length = NULL) const
	{

#ifdef WITHOUT_TENSORFLOW
		return NULL;
#else
		if (length) {
			*length = 0;
			TfLiteIntArray* dims = interpreter_->tensor(index)->dims;
			if (dims && dims->size && dims->data[0]) {
				*length = 1;
				for (int i = 0; i < dims->size; i++) {
					*length *= dims->data[i];
				}
			}
		}
		return interpreter_->typed_tensor<T>(index);
#endif
	}

	template <typename T>
	T* typed_input_tensor(
		int index,
		size_t* length = NULL)
	{
#ifdef WITHOUT_TENSORFLOW
		return NULL;
#else
		return typed_tensor<T>(interpreter_->inputs()[index], length);
#endif
	}

	template <typename T>
	const T* typed_input_tensor(
		int index,
		size_t* length = NULL) const
	{
#ifdef WITHOUT_TENSORFLOW
		return NULL;
#else
		return typed_tensor<T>(interpreter_->inputs()[index], length);
#endif
	}

	template <typename T>
	T* typed_output_tensor(
		int index,
		size_t* length = NULL)
	{
#ifdef WITHOUT_TENSORFLOW
		return NULL;
#else
		return typed_tensor<T>(interpreter_->outputs()[index], length);
#endif
	}

	template <typename T>
	const T* typed_output_tensor(
		int index,
		size_t* length = NULL) const
	{
#ifdef WITHOUT_TENSORFLOW
		return NULL;
#else
		return typed_tensor<T>(interpreter_->outputs()[index], length);
#endif
	}

	const std::vector<int>& inputs() const
	{
#ifdef WITHOUT_TENSORFLOW
		return std::vector<int>();
#else
		return interpreter_->inputs();
#endif
	}

	const char* get_input_name(int index) const
	{
#ifdef WITHOUT_TENSORFLOW
		return NULL;
#else
		return interpreter_->tensor(interpreter_->inputs()[index])->name;
#endif
	}

	const std::vector<int>& outputs() const
	{
#ifdef WITHOUT_TENSORFLOW
		return std::vector<int>();
#else
		return interpreter_->outputs();
#endif
	}

	const char* get_output_name(int index) const
	{
#ifdef WITHOUT_TENSORFLOW
		return NULL;
#else
		return interpreter_->tensor(interpreter_->outputs()[index])->name;
#endif
	}
#ifdef WITHOUT_TENSORFLOW
#else
	std::unique_ptr<tflite::Interpreter> interpreter_;
#endif
private:

	int apply_delegate(int use_nnapi);
#ifdef WITHOUT_TENSORFLOW
#else
	std::unique_ptr<tflite::FlatBufferModel> model_;
#endif
};

#endif
