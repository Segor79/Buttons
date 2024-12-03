#pragma once
#include <CANLibrary.h>

extern CAN_HandleTypeDef hcan;
extern void HAL_CAN_Send(uint16_t id, uint8_t *data_raw, uint8_t length_raw);

namespace CANLib
{
	EasyPinD can_rs(GPIOA, {GPIO_PIN_15, GPIO_MODE_OUTPUT_OD, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW});
	
	
	static constexpr uint8_t CFG_CANObjectsCount = 5;
	static constexpr uint8_t CFG_CANFrameBufferSize = 16;
	static constexpr uint16_t CFG_FIRST_CANID = 0x0210;
	
	
	CANManager<CFG_CANObjectsCount, CFG_CANFrameBufferSize> can_manager(&HAL_CAN_Send);
	
	CANObject<uint8_t, 7> obj_block_info(CFG_FIRST_CANID + 0);
	CANObject<uint8_t, 7> obj_block_health(CFG_FIRST_CANID + 1);
	CANObject<uint8_t, 7> obj_block_features(CFG_FIRST_CANID + 2);
	CANObject<uint8_t, 7> obj_block_error(CFG_FIRST_CANID + 3);
	
	CANObject<uint8_t, 2> obj_buttonled(CFG_FIRST_CANID + 4);
	
	
	void CAN_Enable()
	{
		HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_ERROR | CAN_IT_BUSOFF | CAN_IT_LAST_ERROR_CODE);
		HAL_CAN_Start(&hcan);
		
		can_rs.On();
		
		return;
	}
	
	void CAN_Disable()
	{
		HAL_CAN_DeactivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_ERROR | CAN_IT_BUSOFF | CAN_IT_LAST_ERROR_CODE);
		HAL_CAN_Stop(&hcan);
		
		can_rs.Off();
		
		return;
	}
	
	inline void Setup()
	{
		can_rs.Init();
		
		set_block_info_params(obj_block_info);
		set_block_health_params(obj_block_health);
		set_block_features_params(obj_block_features);
		set_block_error_params(obj_block_error);
		
		can_manager.RegisterObject(obj_block_info);
		can_manager.RegisterObject(obj_block_health);
		can_manager.RegisterObject(obj_block_features);
		can_manager.RegisterObject(obj_block_error);
		
		can_manager.RegisterObject(obj_buttonled);
		
		
		// Передача версий и типов в объект block_info
		obj_block_info.SetValue(0, (About::board_type << 3 | About::board_ver), CAN_TIMER_TYPE_NORMAL);
		obj_block_info.SetValue(1, (About::soft_ver << 2 | About::can_ver), CAN_TIMER_TYPE_NORMAL);

		CAN_Enable();
		
		return;
	}
	
	inline void Loop(uint32_t &current_time)
	{
		can_manager.Process(current_time);

		// Передача UpTime блока в объект block_info
		static uint32_t iter1000 = 0;
		if(current_time - iter1000 > 1000)
		{
			iter1000 = current_time;
			
			uint8_t *data = (uint8_t *)&current_time;
			obj_block_info.SetValue(2, data[0], CAN_TIMER_TYPE_NORMAL);
			obj_block_info.SetValue(3, data[1], CAN_TIMER_TYPE_NORMAL);
			obj_block_info.SetValue(4, data[2], CAN_TIMER_TYPE_NORMAL);
			obj_block_info.SetValue(5, data[3], CAN_TIMER_TYPE_NORMAL);
		}
		
		// При выходе обновляем время
		current_time = HAL_GetTick();
		
		return;
	}
}
