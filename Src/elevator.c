/*
 * elevator.c
 *
 *  Created on: Apr 10, 2025
 *      Author: microsoft
 */
#include "elevator.h"
#define MAX_FLOOR 4
#define MIN_FLOOR 1

/*
 *	함수 설명
 *	
 *	우선순위 초기화 함수
 *	void init_priority(void)
 *	
 *	엘리베이터 구조체 초기화 함수, main에서 태스크 생성 전 호출됨
 *	void init_elevator(void)
 *	
 * 	엘리베이터 제어 태스크 메인 함수
 *	void elevator_main(void)
 *	
 *	우선순위 업데이트 함수, 버튼 이벤트 혹은 층 도착 이벤트에서 해당 함수 호출
 *	void update_priority(void)
 *	
 *	목표 층 업데이트 함수, 버튼 이벤트 혹은 층 도착 이벤트에서 해당 함수 호출
 *	void update_goal(void)
 *	
 *	진행방향 업데이트 함수, 목표 층 업데이트 시 호출됨
 *	void update_direction(void)
 *	
 *	버튼 입력 이벤트 처리 함수, 엘리베이터 제어 태스크 메인 함수에서 호출됨
 *	void btn_event(void)
 *	
 *	층 도착 이벤트 처리 함수, 엘리베이터 제어 태스크 메인 함수에서 호출됨
 *	void floor_event(void)
 *	
 *	목표 층 도착시 엘리베이터를 정지시키기 위한 함수
 *	void pause(void)
 *	
 *	pause_flag가 set 된 상태이면 호출되는 함수
 *	void resume(void)
 *	
 *	스텝모터 제어 함수, 엘리베이터의 진행방향을 넘겨받아 진행방향대로 스텝모터 제어하여 엘리베이터 운행
 *	void stepmotor_driver(uint8_t direction)
 *
 * 	엘리베이터 구조체 설명
 * 	typedef struct
 *	{
 *		uint8_t direction;
 *		uint8_t curr_floor;
 *		uint8_t goal;
 *		uint8_t priority[4];
 *	}t_elevator;
 *	
 *	엘리베이터 구조체는 위와 같음
 *	direction 	-> 	엘리베이터의 현재 진행 방향, FORWARD(상승), BACKWORD(하강), STOP 세가지 상태가 있음
 *					목표 층 업데이트시, 해당 층으로 이동하기 위해 업데이트됨
 *
 * 	curr_floor 	-> 	엘리베이터의 현재 층, 3층에서 4층으로 진행 중이면 현재 층은 3층임
 * 					층 도착(포토센서)에 의한 외부 인터럽트 핸들러에서 업데이트됨
 * 
 *  goal 		-> 	엘리베이터의 현재 목표 층, 현재 층과 진행방향을 기반으로 설정되며 엘리베이터는 항상 목표층 방향으로 이동
 * 					state 업데이트 시(버튼 이벤트 혹은 층 도착 시) 업데이트됨
 * 					목표 층 업데이트 시 진행방향도 업데이트해야 하며, update_goal()에서 update_direction() 호출
 * 
 * 	priority	->	엘리베이터의 목표 층 설정을 위한 우선순위 배열
 * 					우선순위가 높은 순서대로 정렬된 층 숫자의 배열이며, 우선순위는 현재 층과 진행방향을 기반으로 설정됨
 * 					현재 층 업데이트 시 우선순위도 업데이트해야 하며, 층 도착 이벤트 처리 함수에서 update_priority() 호출
 * 					또한 안정성을 위해 버튼 이벤트 처리 함수에서도 update_priority() 호출
 * 
 * 	변수 설명
 * 	state 		->	현재 예약된 층들을 나타내는 uint8 변수
 * 					state의 n번 비트가 set되면 (n+1)층이 예약되어 언젠가는 (n+1)층에 도착해야 한다는 의미
 * 					state의 각 비트는 버튼에 의한 외부 인터럽트 핸들러에서 토글되며, 목표 층 도착 시 해당 층에 해당하는 비트가 clear됨
 * 					state 업데이트 시 새로 업데이트된 state를 기반으로 목표층을 업데이트 해야함
 */


/* 우선순위 초기화 함수 */
void init_priority(void)
{
	elevator.priority[0] = 1;
	elevator.priority[1] = 2;
	elevator.priority[2] = 3;
	elevator.priority[3] = 4;
}

/* 엘리베이터 구조체 초기화 함수, main에서 태스크 생성 전 호출됨 */
void init_elevator(void)
{
	elevator.direction = STOP;
	elevator.curr_floor = 1;
	elevator.goal = NO_GOAL;
	init_priority();
	state = 0x00;
}

/* 엘리베이터 제어 태스크 메인 함수 */
void elevator_main(void)
{
	/* 버튼 이벤트 플래그 set 시, 버튼 이벤트 처리 */
	btn_event();
	/* 층 도착 이벤트 플래그 set 시, 층 도착 이벤트 처리 */
	floor_event();

	/*  
	 *	pasue_flag set 상태 시 resume() 호출
	 *  pause_flag는 목표층 도착 시 set됨	
	 * 	pause_flag set 이후 3초 지나면 resume() 내부에서 pause_flag clear 
	 */
	if (pause_flag) resume();
	else
		/* pase_flag가 clear된 상태이면 stepmotor_driver() 호출 */
		stepmotor_driver(elevator.direction);
}

/* 우선순위 업데이트 함수, 버튼 이벤트 혹은 층 도착 이벤트에서 해당 함수 호출 */
void update_priority(void)
{
	/* 엘리베이터의 진행방향이 FORWARD(상승)일 때 */
	if(elevator.direction == FORWARD)
	{
		/* 우선순위를 할당하기 위한 임시 변수에 현재 층 저장, 현재 층은 층 도착 시 인터럽트 핸들러에서 업데이트됨 */
		uint8_t temp = elevator.curr_floor;
		/* 상승 방향일 때는 초기 증가값: +1 */
		int increament = 1;

		/* 최대 층 수만큼 loop */
		for(int i=0; i<MAX_FLOOR; i++)
		{
			/*
			 * 	priority[i]에 층(temp) 할당
			 * 	priority는 우선순위가 높은 순서대로 정렬된 층 숫자의 배열
			 *	(curr_floor+1)부터 MAX_FLOOR 까지 증가하며 우선순위 할당 이후 curr_floor부터 1까지 감소하며 우선순위 할당
			 */
			temp += increament;
			/* MAX FLOOR까지 우선순위 할당 후 curr_floor부터 감소하며 우선순위 할당 */
			if(temp > MAX_FLOOR)
			{
				temp = elevator.curr_floor;
				increament = -1;
			}
			elevator.priority[i] = temp;
		}
	}
	/* 엘리베이터의 진행방향이 BACKWARD(하강)일 때 */
	else if(elevator.direction == BACKWARD)
	{
		/* 우선순위를 할당하기 위한 임시 변수에 현재 층 저장, 현재 층은 층 도착 시 인터럽트 핸들러에서 업데이트됨 */
		uint8_t temp = elevator.curr_floor;
		/* 하강 방향일 때는 초기 증가값: -1 */
		int increament = -1;
		for(int i=0; i<MAX_FLOOR; i++)
		{
			temp += increament;
			/* MIN_FLOOR까지 우선순위 할당 후 curr_floor부터 증가하며 우선순위 할당 */
			if(temp < MIN_FLOOR)
			{
				temp = elevator.curr_floor;
				increament = 1;
			}
			elevator.priority[i] = temp;
		}
	}
	else
	{
		/*
		 *	direction이 STOP일 시 우선순위 배열 초기화
		 * 	direction이 STOP일 때는 엘리베이터가 움직이지 않아 우선순위의 영향을 받지 않음
		 *  버튼 입력 시 버튼 이벤트 처리 함수에서 우선순위 재할당됨 
		 */
		init_priority();
	}
}

/* 목표 층 업데이트 함수, 버튼 이벤트 혹은 층 도착 이벤트에서 해당 함수 호출 */
void update_goal(void)
{
	/* 우선순위 배열 순회, 우선순위 배열은 우선순위가 높은 순서대로 정렬된 층 숫자의 배열 */
	for (int i=0; i<MAX_FLOOR; i++)
	{
		/* 
		 *	priority[i]에 해당하는 층이 예약된 상태이면, 목표층을 해당 층으로 설정 및 진행방향 업데이트 후 return
		 *	결과적으로 예약된 층들 중 우선순위가 가장 높은 층이 목표 층으로 설정됨
		*/
		if (state & 1 << (elevator.priority[i] - 1) )
		{
			elevator.goal = elevator.priority[i];
			update_direction();
			return;
		}
	}

	/*
	 *	예약된 층이 없을 시 for문 내에서 return되지 않아 해당 코드가 실행됨
	 *	goal에 NO_GOAL 할당 및 direction STOP으로 설정 후, 우선순위 배열 초기화
	 */
	elevator.goal = NO_GOAL;
	elevator.direction = STOP;
	init_priority();
}

/* 진행방향 업데이트 함수, 목표 층 업데이트 시 호출됨 */
void update_direction(void)
{
	/* 
	 *	목표 층이 없을 시 진행방향 STOP으로 설정
	 *	목표 층 없을 때는 update_direction을 호출하지 않아 해당코드 실행되진 않음
	 */
	if (elevator.goal == NO_GOAL)
		elevator.direction = STOP;
	/* 목표 층이 현재 층보다 클 시 진행방향 FORWARD(상승)로 설정 */
	else if (elevator.goal > elevator.curr_floor)
	elevator.direction = FORWARD;
	/* 목표 층이 현재 층보다 작을 시 진행방향 BACKWARD(하강)로 설정 */
	else if (elevator.goal < elevator.curr_floor)
		elevator.direction = BACKWARD;
}

/* 버튼 입력 이벤트 처리 함수, 엘리베이터 제어 태스크 메인 함수에서 호출됨 */
void btn_event(void)
{
	/* 외부 인터럽트 핸들러에 의해 btn_event_flag가 set 된 상태라면, 버튼 이벤트 처리 */
	if (btn_event_flag)
	{
		/* btn_event_flag clear */
		btn_event_flag = 0;
		/* 직전 진행방향 저장 */
		uint8_t prev_direction = elevator.direction;
		/* 	
		 *	우선순위 배열은 층 도착 이벤트에서만 처리하면 되나, 목표 층 업데이트 전 우선순위를 최신화하여 안정성 확보
		 *	버튼 이벤트 처리중 층 도착 인터럽트 핸들러에서 curr_floor 업데이트 시 현재 층의 우선순위를 후순위로 하기 위함
		 */
		update_priority();
		/* 버튼 입력 시 예약된 층들을 저장하는 변수 state가 업데이트되며, 업데이트된 state에 따라 목표 층을 업데이트해야함 */
		update_goal();

		/* update_goal 함수에서 예약된 층이 없어 NO_GOAL 상태가 되면 if문 내부 코드 실행 */
		if (elevator.goal == NO_GOAL && !pause_flag)
		{
			/* 진행방향이 FORWARD(상승)면 현재 층의 위층으로 이동하여 정지(엘리베이터는 층과 층 사이에 멈춰있지 않는다) */
			if (prev_direction == FORWARD) elevator.goal = elevator.curr_floor + 1;
			/* 진행방향이 BACKWARD(하강)면 현재 층의 아래층으로 이동하여 정지(엘리베이터는 층과 층 사이에 멈춰있지 않는다) */
			else if (prev_direction == BACKWARD) elevator.goal = elevator.curr_floor - 1;
		}
	}
}

/* 층 도착 이벤트 처리 함수, 엘리베이터 제어 태스크 메인 함수에서 호출됨 */
void floor_event(void)
{
	/* 외부 인터럽트 핸들러에 의해 floor_event_flag가 set 된 상태라면, 층 도착 이벤트 처리 */
	if (floor_event_flag)
	{
		/* 도트매트릭스 제어를 위한 플래그 set */
		floor_event_flag_dotmatrix = 1;
		/* floor_event_flag clear */
		floor_event_flag = 0;
		/* 인터럽트 핸들러에서 업데이트된 현재 층이 목표 층이라면 if문 내부 코드 실행 */
		if (elevator.curr_floor == elevator.goal)
		{
			/* 예약된 층들을 저장하는 변수 state에서 도착한 층에 해당하는 비트 clear */
			state &= ~(1 << (elevator.curr_floor - 1));
			/* 업데이트된 curr_floor에 따라 우선순위를 업데이트해야함  */
			update_priority();
			/* 업데이트된 우선순위 및 state에 따라 목표 층 및 direction을 업데이트해야함 */
			update_goal();
			/* 목표 층 도착 시 3초 정지 */
			pause();
		}
		/* 목표층이 아닐 시 우선순위만 업데이트 */
		else update_priority();
	}
}

/* 목표 층 도착시 엘리베이터를 정지시키기 위한 함수 */
void pause(void)
{
	/* pause_flag set */
	pause_flag = 1;
	/* buzzer_play_flag set */
	buzzer_play_flag = 1;
	/* elevator_pause_counter 초기화(3초 후 운행 재개를 위함) */
	elevator_pause_counter = 0;
	/* buzzer_counter 초기화 */
	buzzer_counter = 0;
}

/* pause_flag가 set 된 상태이면 호출되는 함수 */
void resume(void)
{
	/* pause 호출 이후 3초 경과 시 elevator_pause_counter 초기화 및 pause_flag clear */
	if (elevator_pause_counter >= 3000)
	{
		elevator_pause_counter = 0;
		pause_flag = 0;
	}
}

/* 스텝모터 제어 함수, 엘리베이터의 진행방향을 넘겨받아 진행방향대로 스텝모터 제어하여 엘리베이터 운행 */
void stepmotor_driver(uint8_t direction)
{
	static int step = 0;
	/* 진행방향이 STOP이면 스텝모터 제어 없이 return */
	if (direction == STOP) return;

	/* static 변수 step에 따라 순차적으로 스텝모터 제어 */
	switch (step)
	{
		case 0:
			stepmotor_all_off();
			GPIOC->ODR |= STEPMOTOR_IN1_Pin;
			break;
		case 1:
			stepmotor_all_off();
			GPIOC->ODR |= STEPMOTOR_IN1_Pin | STEPMOTOR_IN2_Pin;
			break;
		case 2:
			stepmotor_all_off();
			GPIOC->ODR |= STEPMOTOR_IN2_Pin;
			break;
		case 3:
			stepmotor_all_off();
			GPIOC->ODR |= STEPMOTOR_IN2_Pin | STEPMOTOR_IN3_Pin;
			break;
		case 4:
			stepmotor_all_off();
			GPIOC->ODR |= STEPMOTOR_IN3_Pin;
			break;
		case 5:
			stepmotor_all_off();
			GPIOC->ODR |= STEPMOTOR_IN3_Pin | STEPMOTOR_IN4_Pin;
			break;
		case 6:
			stepmotor_all_off();
			GPIOC->ODR |= STEPMOTOR_IN4_Pin;
			break;
		case 7:
			stepmotor_all_off();
			GPIOC->ODR |= STEPMOTOR_IN4_Pin | STEPMOTOR_IN1_Pin;
			break;
	}
	/* 진행방향에 따라 static 변수 step 증가 혹은 감소 */
	if (direction == FORWARD) 		step = (step + 1) % 8;
	else if (direction == BACKWARD)	if(--step < 0) step = 7;
}

void stepmotor_all_off(void)
{
	GPIOC->ODR &= ~(STEPMOTOR_IN1_Pin | STEPMOTOR_IN2_Pin | STEPMOTOR_IN3_Pin | STEPMOTOR_IN4_Pin);
}

