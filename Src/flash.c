#include "main.h"

#include <stdio.h>
#include <string.h>

/**************************************************
     Flash module organization(STM32F411)
                               512KBytes

Name        Block base address              size
====      ==================   =======
Sector 0    0x8000000-0x8003FFF           16K bytes
Sector 1    0x8004000-0x8007FFF           16K bytes
Sector 2    0x8008000-0x800BFFF           16K bytes
Sector 3    0x800C000-0x800FFFF           16K bytes
Sector 4    0x8010000-0x801FFFF           64K bytes
Sector 5    0x8020000-0x803FFFF          128K bytes
Sector 6    0x8040000-0x805FFFF          128K bytes
Sector 7    0x8060000-0x807FFFF          128K bytes

******************************************************/

// 0x8060000-0x807FFFF 의 빈공간에 사용자 데이터를 flash programming
// 하도록 설정 한다.
#define ADDR_FLASH_SECTOR7      ((uint32_t) 0x8060000)
#define FLASH_USER_START_ADDR   ((uint32_t) 0x8060000)
#define USER_DATA_ADDRESS        0x8060000
#define FLASH_USER_END_ADDR     ((uint32_t) 0x807FFFF)
#define FLASH_INIT_STATUS       0xFFFFFFFF        // flash 초기 상태
#define FLASH_NOT_INIT_STATUS   0xAAAAAAAA        // flash 초기 상태가 아니다
#define DATA_32                 ((uint32_t) 0x00000001)

HAL_StatusTypeDef flash_read(uint32_t *addr32, int size);
HAL_StatusTypeDef flash_write (uint32_t *data32, int size);
HAL_StatusTypeDef flash_erase();

uint32_t flash_read_value=0;

HAL_StatusTypeDef flash_write (uint32_t *data32, int size)
{
	uint32_t *mem32 = data32;

	// mem32 = data32;

  /* Unlock to control */
  HAL_FLASH_Unlock();

  uint32_t Address = FLASH_USER_START_ADDR;

//  printf("size: %d\n", size);

  /* Writing data to flash memory */
  for (int i=0; i < size; )
  {
	  if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, *mem32) == HAL_OK)
	  {
//		  printf("mem32: %0x\n", *mem32);
		  mem32++;
		  Address = Address + 4;
		  i += 4;
	  }
	  else
	  {
		  uint32_t errorcode = HAL_FLASH_GetError();
		  return HAL_ERROR;
	  }
  }
  /* Lock flash control register */
  HAL_FLASH_Lock();

  return HAL_OK;
}

HAL_StatusTypeDef flash_read(uint32_t *addr32, int size)
{
  uint32_t *data32 = addr32;
  uint32_t address = FLASH_USER_START_ADDR;
  uint32_t end_address = FLASH_USER_START_ADDR + size;

  while(address < end_address)
  {
    *data32 = *(uint32_t*) address;
    data32++;
    address = address + 4;
  }

  return HAL_OK;

}


HAL_StatusTypeDef flash_erase()
{
	uint32_t SectorError = 0;

	/* Unlock to control */
	HAL_FLASH_Unlock();

	/* Calculate sector index */
	uint32_t UserSector = 7;     // sector 번호
	uint32_t NbOfSectors = 1;    // sector 수

	/* Erase sectors */
	FLASH_EraseInitTypeDef EraseInitStruct;
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
	EraseInitStruct.Sector = UserSector;
	EraseInitStruct.NbSectors = NbOfSectors;

	if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
	{
		uint32_t errorcode = HAL_FLASH_GetError();
		return HAL_ERROR;
	}

	/* Lock flash control register */
	HAL_FLASH_Lock();

	return HAL_OK;
}
