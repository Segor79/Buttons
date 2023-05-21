#pragma once

#include <CANLibrary.h>
#include <CANLibLight.h>

void HAL_CAN_Send(can_object_id_t id, uint8_t *data, uint8_t length);

extern CAN_HandleTypeDef hcan;
extern UART_HandleTypeDef huart1;

namespace CANLib
{


	CANLibLight lib_light(&HAL_CAN_Send);







	//*********************************************************************
	// CAN Library settings
	//*********************************************************************

	/// @brief Number of CANObjects in CANManager
	static constexpr uint8_t CFG_CANObjectsCount = 12;

	/// @brief The size of CANManager's internal CAN frame buffer
	static constexpr uint8_t CFG_CANFrameBufferSize = 16;

	//*********************************************************************
	// CAN Manager & CAN Object configuration
	//*********************************************************************

	// structure for all data fields of CANObject
	// rear_light_can_data_t light_ecu_can_data;

	CANManager<CFG_CANObjectsCount, CFG_CANFrameBufferSize> can_manager(&HAL_CAN_Send);

	// ******************** common blocks ********************
	// 0x0120	BlockInfo
	// request | timer:15000
	// byte	1 + 7	{ type[0] data[1..7] }
	// Основная информация о блоке. См. "Системные параметры".
	CANObject<uint8_t, 7> obj_block_info(0x0120, 15000, 300);

	// 0x0121	BlockHealth
	// request | event
	// byte	1 + 7	{ type[0] data[1..7] }
	// Информация о здоровье блока. См. "Системные параметры".
	CANObject<uint8_t, 7> obj_block_health(0x0121, CAN_TIMER_DISABLED, 300);

	// 0x0122	BlockCfg
	// request
	// byte	1 + 1 + X	{ type[0] param[1] data[2..7] }
	// Чтение и запись настроек блока. См. "Системные параметры".
	CANObject<uint8_t, 7> obj_block_cfg(0x0122, CAN_TIMER_DISABLED, CAN_ERROR_DISABLED);


	// 0x0123	BlockError
	// request | event
	// byte	1 + X	{ type[0] data[1..7] }
	// Ошибки блока. См. "Системные параметры".
	CANObject<uint8_t, 7> obj_block_error(0x0123, CAN_TIMER_DISABLED, 300);



	// ******************** specific blocks ********************

	// 0x00E4	SideBeam
	// set | request | event
	// uint8_t	0..255	1 + 1	{ type[0] val[1] }
	// Управление габаритами.
	CANObject<uint8_t, 1> obj_side_beam1(0x00C4, CAN_TIMER_DISABLED, CAN_ERROR_DISABLED);
	CANObject<uint8_t, 1> obj_side_beam2(0x00E4, CAN_TIMER_DISABLED, CAN_ERROR_DISABLED);








/*
	// 0x00E5	BrakeLight
	// set | request | event
	// uint8_t	0..255	1 + 1	{ type[0] val[1] }
	// Управление стоп-сигналами.
	CANObject<uint8_t, 1> obj_brake_light(0x00E5, CAN_TIMER_DISABLED, 300);


	// 0x00E6	ReverseLight
	// set | request | event
	// uint8_t	0..255	1 + 1	{ type[0] val[1] }
	// Управление задним ходом.
	CANObject<uint8_t, 1> obj_reverse_light(0x00E6, CAN_TIMER_DISABLED, 300);


	// 0x00E7	LeftIndicator
	// set | request | event
	// uint8_t	0..255	1 + 1	{ type[0] val[1] }
	// Управление левым поворотником.
	CANObject<uint8_t, 1> obj_left_indicator(0x00E7, CAN_TIMER_DISABLED, 300);


	// 0x00E8	RightIndicator
	// set | request | event
	// uint8_t	0..255	1 + 1	{ type[0] val[1] }
	// Управление правым поворотником.
	CANObject<uint8_t, 1> obj_right_indicator(0x00E8, CAN_TIMER_DISABLED, 300);


	// 0x00E9	HazardBeam
	// set | request | event
	// uint8_t	0..255	1 + 1	{ type[0] val[1] }
	// Управление аварийным сигналом.
	CANObject<uint8_t, 1> obj_hazard_beam(0x00E9, CAN_TIMER_DISABLED, 300);


	// 0x00EA	CustomBeam
	// set | request | event
	// uint8_t	0..255	1 + 1	{ type[0] val[1] }
	// Управление пользовательским светом.
	CANObject<uint8_t, 1> obj_custom_beam(0x00EA, CAN_TIMER_DISABLED, 300);


	// 0x00EB	CustomImage
	// set | request | event
	// uint8_t	0..255	1 + 1	{ type[0] val[1] }
	// Управление пользовательскими изображениями ( для WS2812b ).
	CANObject<uint8_t, 1> obj_custom_image(0x00EB, CAN_TIMER_DISABLED, 300);

	// 0x00EC	ImageTransfer
	// send raw
	// Link 1 + X	{ type[0] data[1..7] }
	// Для передачи изображений
	// TODO: this function has been put on hold.
*/

/*
	can_result_t void_set_handler(can_frame_t &can_frame, can_error_t &error)
	{
		if(can_frame.function_id == CAN_FUNC_SET_IN) return CAN_RESULT_IGNORE;


	}
*/
	// вызывается, если по CAN пришла команда включения/выключения габаритов
	can_result_t side_beam_set_handler(can_frame_t &can_frame, can_error_t &error)
	{
		// light_ecu_can_data.side_beam.brightness = can_frame.data[0];

		// if (light_ecu_can_data.side_beam.brightness == 0)
		if (can_frame.data[0] == 0)
		{
			//Matrix::matrixObj.HideLayer(2);
			//Outputs::outObj.SetOff(2);
		}
		else
		{
			//Matrix::matrixObj.ShowLayer(2);
			//Outputs::outObj.SetOn(2);
		}

		can_frame.initialized = true;
		//can_frame.function_id = CAN_FUNC_SET_OUT_OK;
		// can_frame.data[0] doesn't change
		// TODO: может читать установленное значение с порта и его присваивать в can_frame.data[0]?
		// читать вот этой функцией: HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9)
		can_frame.raw_data_length = 2;

		return CAN_RESULT_CAN_FRAME;
	}

/*
	// вызывается, если по CAN пришла команда включения/выключения стоп-сигналов
	can_result_t brake_light_set_handler(can_frame_t &can_frame, can_error_t &error)
	{
		// light_ecu_can_data.brake_light.brightness = can_frame.data[0];

		// if (light_ecu_can_data.brake_light.brightness == 0)
		if (can_frame.data[0] == 0)
		{
			Matrix::matrixObj.HideLayer(4);
			Outputs::outObj.SetOff(4);
		}
		else
		{
			Matrix::matrixObj.ShowLayer(4);
			Outputs::outObj.SetOn(4);
		}

		can_frame.initialized = true;
		can_frame.function_id = CAN_FUNC_SET_OUT_OK;
		// can_frame.data[0] doesn't change
		can_frame.raw_data_length = 2;

		return CAN_RESULT_CAN_FRAME;
	}

	// вызывается, если по CAN пришла команда включения/выключения заднего хода
	can_result_t reverse_light_set_handler(can_frame_t &can_frame, can_error_t &error)
	{
		// light_ecu_can_data.reverse_light.brightness = can_frame.data[0];

		// if (light_ecu_can_data.reverse_light.brightness == 0)
		if (can_frame.data[0] == 0)
		{
			Matrix::matrixObj.HideLayer(3);
			Outputs::outObj.SetOff(3);
		}
		else
		{
			Matrix::matrixObj.ShowLayer(3);
			Outputs::outObj.SetOn(3);
		}

		can_frame.initialized = true;
		can_frame.function_id = CAN_FUNC_SET_OUT_OK;
		// can_frame.data[0] doesn't change
		can_frame.raw_data_length = 2;

		return CAN_RESULT_CAN_FRAME;
	}

	// вызывается, если по CAN пришла команда включения/выключения левого поворотника
	can_result_t turn_left_set_handler(can_frame_t &can_frame, can_error_t &error)
	{
		// light_ecu_can_data.left_indicator.brightness = can_frame.data[0];

		// if (light_ecu_can_data.left_indicator.brightness == 0)
		if (can_frame.data[0] == 0)
		{
			Matrix::matrixObj.HideLayer(5);
			Outputs::outObj.SetOff(5);
		}
		else
		{
			Matrix::matrixObj.ShowLayer(5);
			Outputs::outObj.SetOn(5, 750, 750);
		}

		can_frame.initialized = true;
		can_frame.function_id = CAN_FUNC_SET_OUT_OK;
		// can_frame.data[0] doesn't change
		can_frame.raw_data_length = 2;

		return CAN_RESULT_CAN_FRAME;
	}

	// вызывается, если по CAN пришла команда включения/выключения правого поворотника
	can_result_t turn_right_set_handler(can_frame_t &can_frame, can_error_t &error)
	{
		// light_ecu_can_data.right_indicator.brightness = can_frame.data[0];

		// if (light_ecu_can_data.right_indicator.brightness == 0)
		if (can_frame.data[0] == 0)
		{
			Matrix::matrixObj.HideLayer(6);
			Outputs::outObj.SetOff(6);
		}
		else
		{
			Matrix::matrixObj.ShowLayer(6);
			Outputs::outObj.SetOn(6, 750, 750);
		}

		can_frame.initialized = true;
		can_frame.function_id = CAN_FUNC_SET_OUT_OK;
		// can_frame.data[0] doesn't change
		can_frame.raw_data_length = 2;

		return CAN_RESULT_CAN_FRAME;
	}

	// вызывается, если по CAN пришла команда включения/выключения аварийного сигнала
	can_result_t hazard_beam_set_handler(can_frame_t &can_frame, can_error_t &error)
	{
		// light_ecu_can_data.hazard_beam.brightness = can_frame.data[0];

		// if (light_ecu_can_data.hazard_beam.brightness == 0)
		if (can_frame.data[0] == 0)
		{
			Matrix::matrixObj.HideLayer(7);
			Outputs::outObj.SetOff(5);
			Outputs::outObj.SetOff(6);
		}
		else
		{
			Matrix::matrixObj.ShowLayer(7);
			Outputs::outObj.SetOn(5, 750, 750);
			Outputs::outObj.SetOn(6, 750, 750);
		}

		can_frame.initialized = true;
		can_frame.function_id = CAN_FUNC_SET_OUT_OK;
		// can_frame.data[0] doesn't change
		can_frame.raw_data_length = 2;

		return CAN_RESULT_CAN_FRAME;
	}

	// вызывается, если по CAN пришла команда включения/выключения пользовательского света
	can_result_t custom_beam_set_handler(can_frame_t &can_frame, can_error_t &error)
	{
		// light_ecu_can_data.custom_beam.brightness = can_frame.data[0];

		// if (light_ecu_can_data.custom_beam.brightness == 0)
		if (can_frame.data[0] == 0)
		{
			Outputs::outObj.SetOff(1);
		}
		else
		{
			Outputs::outObj.SetOn(1);
		}

		can_frame.initialized = true;
		can_frame.function_id = CAN_FUNC_SET_OUT_OK;
		// can_frame.data[0] doesn't change
		can_frame.raw_data_length = 2;

		return CAN_RESULT_CAN_FRAME;
	}

	// вызывается, если по CAN пришла команда включения/выключения пользовательского изображения на панели
	can_result_t custom_image_set_handler(can_frame_t &can_frame, can_error_t &error)
	{
		// light_ecu_can_data.custom_image.brightness = can_frame.data[0];

		// if (light_ecu_can_data.custom_image.brightness == 0)
		if (can_frame.data[0] == 0)
		{
			Matrix::matrixObj.HideLayer(1);
		}
		else
		{
			char filename[12];
			sprintf(filename, "user%03d.pxl", can_frame.data[0]);
			Matrix::matrixObj.RegLayer(filename, 1);
			Matrix::matrixObj.ShowLayer(1);
		}

		can_frame.initialized = true;
		can_frame.function_id = CAN_FUNC_SET_OUT_OK;
		// can_frame.data[0] doesn't change
		can_frame.raw_data_length = 2;

		return CAN_RESULT_CAN_FRAME;
	}
*/

	inline void Setup()
	{
		// set CAN data structure to zero
		// memset(&light_ecu_can_data, 0, sizeof(light_ecu_can_data));

		//obj_side_beam1.RegisterFunctionSet(&void_set_handler);
		//obj_side_beam2.RegisterFunctionSet(&void_set_handler);
		
		//obj_brake_light.RegisterFunctionSet(&brake_light_set_handler);
		//obj_reverse_light.RegisterFunctionSet(&reverse_light_set_handler);
		//obj_left_indicator.RegisterFunctionSet(&turn_left_set_handler);
		//obj_right_indicator.RegisterFunctionSet(&turn_right_set_handler);
		//obj_hazard_beam.RegisterFunctionSet(&hazard_beam_set_handler);
		//obj_custom_beam.RegisterFunctionSet(&custom_beam_set_handler);
		//obj_custom_image.RegisterFunctionSet(&custom_image_set_handler);

		// common blocks
		can_manager.RegisterObject(obj_block_info);
		can_manager.RegisterObject(obj_block_health);
		can_manager.RegisterObject(obj_block_cfg);
		can_manager.RegisterObject(obj_block_error);

		// specific blocks
		can_manager.RegisterObject(obj_side_beam1);
		can_manager.RegisterObject(obj_side_beam2);
		//can_manager.RegisterObject(obj_brake_light);
		//can_manager.RegisterObject(obj_reverse_light);
		//can_manager.RegisterObject(obj_left_indicator);
		//can_manager.RegisterObject(obj_right_indicator);
		//can_manager.RegisterObject(obj_hazard_beam);
		//can_manager.RegisterObject(obj_custom_beam);
		//can_manager.RegisterObject(obj_custom_image);

		//*************************************************************
		// TODO: CANManager experiments
		// init normal timer for Block_Info
		obj_block_info.SetValue(0, 0x66, CAN_TIMER_TYPE_NORMAL);
		//*************************************************************

		return;
	}

	inline void Loop(uint32_t &current_time)
	{
		can_manager.Process(current_time);

		//*************************************************************
		// TODO: CANManager experiments
		static uint32_t iter = 0;
		if (current_time - iter > 1000)
		{
			iter = current_time;

			uint8_t *data = (uint8_t *)&current_time;
			obj_block_info.SetValue(3, data[0], CAN_TIMER_TYPE_NORMAL);
			obj_block_info.SetValue(4, data[1], CAN_TIMER_TYPE_NORMAL);
			obj_block_info.SetValue(5, data[2], CAN_TIMER_TYPE_NORMAL);
			obj_block_info.SetValue(6, data[3], CAN_TIMER_TYPE_NORMAL);
		}
		//*************************************************************

		current_time = HAL_GetTick();

		return;
	}
}
