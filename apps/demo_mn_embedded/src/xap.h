/* This file was autogenerated by openCONFIGURATOR-1.3.0 on 18-Mar-2014 13:41:27 */
#ifndef XAP_h
#define XAP_h

# define COMPUTED_PI_OUT_SIZE 12
typedef struct 
{
	unsigned CN1_M00_Digital_Input_8_Bit_Byte_1:8;
	unsigned CN2_M00_Digital_Input_8_Bit_Byte_1:8;
	unsigned CN3_M00_Digital_Input_8_Bit_Byte_1:8;
	unsigned CN4_M00_Digital_Input_8_Bit_Byte_1:8;
	unsigned CN5_M00_Digital_Input_8_Bit_Byte_1:8;
	unsigned CN6_M00_Digital_Input_8_Bit_Byte_1:8;
	unsigned CN7_M00_Digital_Input_8_Bit_Byte_1:8;
	unsigned CN8_M00_Digital_Input_8_Bit_Byte_1:8;
	unsigned CN9_M00_Digital_Input_8_Bit_Byte_1:8;
	unsigned CN10_M00_Digital_Input_8_Bit_Byte_1:8;
	unsigned PADDING_VAR_1:16;
} PI_OUT;

# define COMPUTED_PI_IN_SIZE 12
typedef struct 
{
	unsigned CN1_M00_Digital_Output_8_Bit_Byte_1:8;
	unsigned CN2_M00_Digital_Output_8_Bit_Byte_1:8;
	unsigned CN3_M00_Digital_Output_8_Bit_Byte_1:8;
	unsigned CN4_M00_Digital_Output_8_Bit_Byte_1:8;
	unsigned CN5_M00_Digital_Output_8_Bit_Byte_1:8;
	unsigned CN6_M00_Digital_Output_8_Bit_Byte_1:8;
	unsigned CN7_M00_Digital_Output_8_Bit_Byte_1:8;
	unsigned CN8_M00_Digital_Output_8_Bit_Byte_1:8;
	unsigned CN9_M00_Digital_Output_8_Bit_Byte_1:8;
	unsigned CN10_M00_Digital_Output_8_Bit_Byte_1:8;
	unsigned PADDING_VAR_1:16;
} PI_IN;

#endif