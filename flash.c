#include "flash.h"
#define SECTOR_MASK               ((uint32_t)0xFFFFFF07)
void FLASH_SetLatency(uint32_t FLASH_Latency)
{
  *(__IO uint8_t *)ACR_BYTE0_ADDRESS = (uint8_t)FLASH_Latency;
}
void FLASH_PrefetchBufferCmd(FunctionalState NewState)
{
  if(NewState != DISABLE)
  {
    FLASH->ACR |= FLASH_ACR_PRFTEN;
  }
  else
  {
    FLASH->ACR &= (~FLASH_ACR_PRFTEN);
  }
}
void FLASH_InstructionCacheCmd(FunctionalState NewState)
{
  if(NewState != DISABLE)
  {
    FLASH->ACR |= FLASH_ACR_ICEN;
  }
  else
  {
    FLASH->ACR &= (~FLASH_ACR_ICEN);
  }
}
void FLASH_DataCacheCmd(FunctionalState NewState)
{
  if(NewState != DISABLE)
  {
    FLASH->ACR |= FLASH_ACR_DCEN;
  }
  else
  {
    FLASH->ACR &= (~FLASH_ACR_DCEN);
  }
}
void FLASH_InstructionCacheReset(void)
{
  FLASH->ACR |= FLASH_ACR_ICRST;
}
void FLASH_DataCacheReset(void)
{
  FLASH->ACR |= FLASH_ACR_DCRST;
}
void FLASH_Unlock(void)
{
  if((FLASH->CR & FLASH_CR_LOCK) != RESET)
  {
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;
  }  
}
void FLASH_Lock(void)
{
  FLASH->CR |= FLASH_CR_LOCK;
}
FLASH_Status FLASH_EraseSector(uint32_t FLASH_Sector, uint8_t VoltageRange)
{
  uint32_t tmp_psize = 0x0;
  FLASH_Status status = FLASH_COMPLETE;
  if(VoltageRange == VoltageRange_1)
  {
     tmp_psize = FLASH_PSIZE_BYTE;
  }
  else if(VoltageRange == VoltageRange_2)
  {
    tmp_psize = FLASH_PSIZE_HALF_WORD;
  }
  else if(VoltageRange == VoltageRange_3)
  {
    tmp_psize = FLASH_PSIZE_WORD;
  }
  else
  {
    tmp_psize = FLASH_PSIZE_DOUBLE_WORD;
  }
  status = FLASH_WaitForLastOperation();
  if(status == FLASH_COMPLETE)
  {
    FLASH->CR &= CR_PSIZE_MASK;
    FLASH->CR |= tmp_psize;
    FLASH->CR &= SECTOR_MASK;
    FLASH->CR |= FLASH_CR_SER | FLASH_Sector;
    FLASH->CR |= FLASH_CR_STRT;
    status = FLASH_WaitForLastOperation();
    FLASH->CR &= (~FLASH_CR_SER);
    FLASH->CR &= SECTOR_MASK; 
  }
  return status;
}
FLASH_Status FLASH_EraseAllSectors(uint8_t VoltageRange)
{
  uint32_t tmp_psize = 0x0;
  FLASH_Status status = FLASH_COMPLETE;
  status = FLASH_WaitForLastOperation();
  if(VoltageRange == VoltageRange_1)
  {
     tmp_psize = FLASH_PSIZE_BYTE;
  }
  else if(VoltageRange == VoltageRange_2)
  {
    tmp_psize = FLASH_PSIZE_HALF_WORD;
  }
  else if(VoltageRange == VoltageRange_3)
  {
    tmp_psize = FLASH_PSIZE_WORD;
  }
  else
  {
    tmp_psize = FLASH_PSIZE_DOUBLE_WORD;
  }  
  if(status == FLASH_COMPLETE)
  {
     FLASH->CR &= CR_PSIZE_MASK;
     FLASH->CR |= tmp_psize;
     FLASH->CR |= FLASH_CR_MER;
     FLASH->CR |= FLASH_CR_STRT;
    status = FLASH_WaitForLastOperation();
    FLASH->CR &= (~FLASH_CR_MER);
  }   
  return status;
}
FLASH_Status FLASH_ProgramDoubleWord(uint32_t Address, uint64_t Data)
{
  FLASH_Status status = FLASH_COMPLETE;
  status = FLASH_WaitForLastOperation();
  if(status == FLASH_COMPLETE)
  {
    FLASH->CR &= CR_PSIZE_MASK;
    FLASH->CR |= FLASH_PSIZE_DOUBLE_WORD;
    FLASH->CR |= FLASH_CR_PG;
    *(__IO uint64_t*)Address = Data;
    status = FLASH_WaitForLastOperation();
    FLASH->CR &= (~FLASH_CR_PG);
  } 
  return status;
}
FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data)
{
  FLASH_Status status = FLASH_COMPLETE;
  status = FLASH_WaitForLastOperation();
  if(status == FLASH_COMPLETE)
  {
    FLASH->CR &= CR_PSIZE_MASK;
    FLASH->CR |= FLASH_PSIZE_WORD;
    FLASH->CR |= FLASH_CR_PG;
    *(__IO uint32_t*)Address = Data;
    status = FLASH_WaitForLastOperation();
    FLASH->CR &= (~FLASH_CR_PG);
  } 
  return status;
}
FLASH_Status FLASH_ProgramHalfWord(uint32_t Address, uint16_t Data)
{
  FLASH_Status status = FLASH_COMPLETE;
  status = FLASH_WaitForLastOperation();
  if(status == FLASH_COMPLETE)
  {
    FLASH->CR &= CR_PSIZE_MASK;
    FLASH->CR |= FLASH_PSIZE_HALF_WORD;
    FLASH->CR |= FLASH_CR_PG;
    *(__IO uint16_t*)Address = Data;
    status = FLASH_WaitForLastOperation();
    FLASH->CR &= (~FLASH_CR_PG);
  } 
  return status;
}
FLASH_Status FLASH_ProgramByte(uint32_t Address, uint8_t Data)
{
  FLASH_Status status = FLASH_COMPLETE;
  status = FLASH_WaitForLastOperation();
  if(status == FLASH_COMPLETE)
  {
    FLASH->CR &= CR_PSIZE_MASK;
    FLASH->CR |= FLASH_PSIZE_BYTE;
    FLASH->CR |= FLASH_CR_PG;
    *(__IO uint8_t*)Address = Data;
    status = FLASH_WaitForLastOperation();
    FLASH->CR &= (~FLASH_CR_PG);
  } 
  return status;
}
void FLASH_OB_Unlock(void)
{
  if((FLASH->OPTCR & FLASH_OPTCR_OPTLOCK) != RESET)
  {
    FLASH->OPTKEYR = FLASH_OPT_KEY1;
    FLASH->OPTKEYR = FLASH_OPT_KEY2;
  }  
}
void FLASH_OB_Lock(void)
{
  FLASH->OPTCR |= FLASH_OPTCR_OPTLOCK;
}
void FLASH_OB_WRPConfig(uint32_t OB_WRP, FunctionalState NewState)
{ 
  FLASH_Status status = FLASH_COMPLETE;
  status = FLASH_WaitForLastOperation();
  if(status == FLASH_COMPLETE)
  { 
    if(NewState != DISABLE)
    {
      *(__IO uint16_t*)OPTCR_BYTE2_ADDRESS &= (~OB_WRP);
    }
    else
    {
      *(__IO uint16_t*)OPTCR_BYTE2_ADDRESS |= (uint16_t)OB_WRP;
    }
  }
}
void FLASH_OB_RDPConfig(uint8_t OB_RDP)
{
  FLASH_Status status = FLASH_COMPLETE;
  status = FLASH_WaitForLastOperation();
  if(status == FLASH_COMPLETE)
  {
    *(__IO uint8_t*)OPTCR_BYTE1_ADDRESS = OB_RDP;
  }
}
void FLASH_OB_UserConfig(uint8_t OB_IWDG, uint8_t OB_STOP, uint8_t OB_STDBY)
{
  uint8_t optiontmp = 0xFF;
  FLASH_Status status = FLASH_COMPLETE; 
  status = FLASH_WaitForLastOperation();
  if(status == FLASH_COMPLETE)
  { 
    optiontmp =  (uint8_t)((*(__IO uint8_t *)OPTCR_BYTE0_ADDRESS) & (uint8_t)0x0F); 
    *(__IO uint8_t *)OPTCR_BYTE0_ADDRESS = OB_IWDG | (uint8_t)(OB_STDBY | (uint8_t)(OB_STOP | ((uint8_t)optiontmp))); 
  }  
}
void FLASH_OB_BORConfig(uint8_t OB_BOR)
{
  *(__IO uint8_t *)OPTCR_BYTE0_ADDRESS &= (~FLASH_OPTCR_BOR_LEV);
  *(__IO uint8_t *)OPTCR_BYTE0_ADDRESS |= OB_BOR;
}
FLASH_Status FLASH_OB_Launch(void)
{
  FLASH_Status status = FLASH_COMPLETE;
  *(__IO uint8_t *)OPTCR_BYTE0_ADDRESS |= FLASH_OPTCR_OPTSTRT;
  status = FLASH_WaitForLastOperation();
  return status;
}
uint8_t FLASH_OB_GetUser(void)
{
  return (uint8_t)(FLASH->OPTCR >> 5);
}
uint16_t FLASH_OB_GetWRP(void)
{
  return (*(__IO uint16_t *)(OPTCR_BYTE2_ADDRESS));
}
FlagStatus FLASH_OB_GetRDP(void)
{
  FlagStatus readstatus = RESET;

  if ((*(__IO uint8_t*)(OPTCR_BYTE1_ADDRESS) != (uint8_t)OB_RDP_Level_0))
  {
    readstatus = SET;
  }
  else
  {
    readstatus = RESET;
  }
  return readstatus;
}
uint8_t FLASH_OB_GetBOR(void)
{
  return (uint8_t)(*(__IO uint8_t *)(OPTCR_BYTE0_ADDRESS) & (uint8_t)0x0C);
}
void FLASH_ITConfig(uint32_t FLASH_IT, FunctionalState NewState)
{
  if(NewState != DISABLE)
  {
    FLASH->CR |= FLASH_IT;
  }
  else
  {
    FLASH->CR &= ~(uint32_t)FLASH_IT;
  }
}
FlagStatus FLASH_GetFlagStatus(uint32_t FLASH_FLAG)
{
  FlagStatus bitstatus = RESET;
  if((FLASH->SR & FLASH_FLAG) != (uint32_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus; 
}
void FLASH_ClearFlag(uint32_t FLASH_FLAG)
{
  FLASH->SR = FLASH_FLAG;
}
FLASH_Status FLASH_GetStatus(void)
{
  FLASH_Status flashstatus = FLASH_COMPLETE;
  
  if((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY) 
  {
    flashstatus = FLASH_BUSY;
  }
  else 
  {  
    if((FLASH->SR & FLASH_FLAG_WRPERR) != (uint32_t)0x00)
    { 
      flashstatus = FLASH_ERROR_WRP;
    }
    else 
    {
      if((FLASH->SR & (uint32_t)0xEF) != (uint32_t)0x00)
      {
        flashstatus = FLASH_ERROR_PROGRAM; 
      }
      else
      {
        if((FLASH->SR & FLASH_FLAG_OPERR) != (uint32_t)0x00)
        {
          flashstatus = FLASH_ERROR_OPERATION;
        }
        else
        {
          flashstatus = FLASH_COMPLETE;
        }
      }
    }
  }
  return flashstatus;
}
FLASH_Status FLASH_WaitForLastOperation(void)
{ 
  __IO FLASH_Status status = FLASH_COMPLETE;
  status = FLASH_GetStatus();
  while(status == FLASH_BUSY)
  {
    status = FLASH_GetStatus();
  }
  return status;
}
