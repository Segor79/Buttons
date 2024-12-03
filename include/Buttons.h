#pragma once
#include <inttypes.h>

namespace ButtonsLeds
{
	
	// Вызывается при изменении состояния любой из кнопок
	void OnButtonsUpdate(uint8_t device, uint8_t pin, bool state)
	{
		DEBUG_LOG_TOPIC("HC165", "device: %d, pin: %d, state: %d\n", device, pin, state);
		
		uint8_t btn_number = ((device * 8) + (pin + 1));
		uint8_t btn_state = state ? 0xFF : 0x00;
		
		CANLib::obj_buttonled.SetValue(0, btn_number);
		CANLib::obj_buttonled.SetValue(1, btn_state, CAN_TIMER_TYPE_NONE, CAN_EVENT_TYPE_NORMAL);
		
		return;
	}
	
	// Вызывается при отправка команды Set серез CAN
	can_result_t OnButtonSet(can_frame_t &can_frame, can_error_t &error)
	{
		// Временная заглушка. Отвечаем такими-же данными как получили
		CANLib::obj_buttonled.SetValue(0, can_frame.data[0], CAN_TIMER_TYPE_NONE, CAN_EVENT_TYPE_NORMAL);
		CANLib::obj_buttonled.SetValue(1, can_frame.data[1], CAN_TIMER_TYPE_NONE, CAN_EVENT_TYPE_NORMAL);
		
		return CAN_RESULT_IGNORE;
	}
	
	inline void Setup()
	{
		SPI::hc165.SetCallback(OnButtonsUpdate);
		CANLib::obj_buttonled.RegisterFunctionSet(OnButtonSet);
		
		return;
	}
	
	inline void Loop(uint32_t &current_time)
	{
		
		
		// При выходе обновляем время
		current_time = HAL_GetTick();
		
		return;
	}
}
